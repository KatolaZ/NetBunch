/**
 *  This program is free software: you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 *  (c) Vincenzo Nicosia 2009-2017 -- <v.nicosia@qmul.ac.uk>
 * 
 *  This file is part of NetBunch, a package for complex network
 *  analysis and modelling. For more information please visit:
 *
 *             http://www.complex-networks.net/
 *
 *  If you use this software, please add a reference to 
 *
 *               V. Latora, V. Nicosia, G. Russo             
 *   "Complex Networks: Principles, Methods and Applications"
 *              Cambridge University Press (2017) 
 *                ISBN: 9781107103184
 *
 ***********************************************************************
 *
 * Compute knn(k) for a given unweighted graph. If requested, the
 * average is computed for ranges of degrees falling into a number of
 * equal bins or into bins or exponentially increasing width.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "utils.h"
#include "knntree.h"

#define NO_BIN   0x01
#define BIN_LIN  0x02
#define BIN_EXP  0x04

void usage(char *argv[]){

  printf("********************************************************************\n"
         "**                                                                **\n"
         "**                        -*-     knn    -*-                      **\n"
         "**                                                                **\n"
         "**   Compute the average nearest-neighbour degree function knn(k) **\n"
         "**   of the unweighted graph provided as input.                   **\n"
         "**                                                                **\n"
         "**   The input file 'graph_in' is an edge-list:                   **\n"
         "**                                                                **\n"
         "**                            I_1 J_1                             **\n"
         "**                            I_2 J_2                             **\n"
         "**                            I_3 J_3                             **\n"
         "**                            ... ...                             **\n"
         "**                            I_K J_K                             **\n"
         "**                                                                **\n"
         "**    If 'graph_in' is equal to '-' (dash), read the file from    **\n" 
         "**    the standard input (STDIN).                                 **\n"
         "**                                                                **\n"
         "**    If no second parameter is given (or the second parameter    **\n"
         "**    is equal to 'NO'), the program prints on output the value   **\n"
         "**    of knn(k) for each degree in the graph, in the format:      **\n"
         "**                                                                **\n"
         "**          k1  knn(k1)                                           **\n"
         "**          k2  knn(k2)                                           **\n"
         "**          .....                                                 **\n"
         "**                                                                **\n"
         "**     If the second parameter is 'LIN', the values of knn(k)     **\n"
         "**     will be grouped into 'bin_param' equal bins.               **\n"
         "**                                                                **\n"
         "**     If the second parameter is 'EXP;, the values of knn(k)     **\n"
         "**     will be grouped into bins whose width increases            **\n"
         "**     exponentially with exponent 'bin_param' (also known as     **\n"
         "**     'logarithmic binning').                                    **\n"
         "**                                                                **\n"
         "********************************************************************\n"
         " This is Free Software - You can use and distribute it under \n"
         " the terms of the GNU General Public License, version 3 or later\n\n"
         " Please visit http://www.complex-networks.net for more information\n\n"
         " (c) Vincenzo Nicosia 2010-2017 (v.nicosia@qmul.ac.uk)\n"
         "********************************************************************\n\n"
         );
  printf("Usage: %s <graph_in> [<NO|LIN|EXP> <bin_param>]\n", argv[0]);
}



typedef struct{
  unsigned int num_bins;
  unsigned int *degs;
  unsigned int *num;
  double *values;
} knn_hist_t;


/* compute the sum of the degrees of the neighbours of the node with
   label 'idx' */
double neigh_degs_sum(unsigned int *J_slap, unsigned int *r_slap, unsigned int idx){

  double sum = 0;
  unsigned j, k;
  
  for(j=r_slap[idx]; j < r_slap[idx+1]; j++){
    k = J_slap[j];
    sum += r_slap[k+1] - r_slap[k];
  }
  return sum;
}

/* compute knn(k) for each degree k */
void compute_knn(unsigned int *J_slap, unsigned int *r_slap, unsigned int N, 
                 knntree_t t){

  knnsum_t elem, *ptr;
  unsigned int i;

  for(i=0; i<N; i++){
    elem.k = r_slap[i+1] - r_slap[i];
    elem.knnsum = neigh_degs_sum(J_slap, r_slap, i);
    ptr = iltree_lookup(t, &elem);
    if (!ptr){
      elem.Nk = 1;
      iltree_insert(t, &elem);
    }
    else{
      ptr->Nk +=1;
      ptr->knnsum += elem.knnsum;
    }
  }
}

/* aggregate all the values of knn(k) for the degrees falling in a
   certain bin */
void aggregate_knn(void *elem, void *h){

  knn_hist_t *hist;
  knnsum_t *s;
  unsigned int cur, low, high;

  hist = (knn_hist_t*)h;
  s = (knnsum_t*)elem;
  

  cur = hist->num_bins / 2;
  
  low = 0;
  high = hist->num_bins - 1;
  
  while (low < high){
    if (hist->degs[cur] < s->k){
      low = cur+1;
    }
    else if(hist->degs[cur] >= s->k){
      high = cur;
    }
    cur = (high + low) / 2;
  }

  hist->num[cur] += s->Nk * s->k;
  hist->values[cur] += s->knnsum;
}


/* print knn(k) for each value of k in the graph (i.e., without any
   binning) */
void print_knn_nobin(knntree_t t){
  
  iltree_view_pre(t);
}



/* print the values of knn(k) averaging over num_bins of equal size */
void print_knn_lin(knntree_t t, unsigned int num_bins){
  
  unsigned int kmin, kmax, step, i;
  knn_hist_t hist;
  
  if (!t){
    return;
  }


  kmin = ((knnsum_t*)iltree_getmin(t))->k;
  kmax = ((knnsum_t*)iltree_getmax(t))->k;
  
  hist.num_bins = num_bins;
  
  step = (kmax - kmin)/num_bins + 1;
  
  hist.degs = malloc(num_bins * sizeof(unsigned int));
  hist.num = malloc(num_bins * sizeof(unsigned int));
  hist.values = malloc(num_bins * sizeof(double));


  hist.degs[0] = kmin + step;
  hist.values[0] = 0;
  hist.num[0] = 0;

  for(i=1; i<num_bins; i++){
    hist.degs[i] = hist.degs[i-1] + step;
    hist.values[i] = 0;
    hist.num[i] = 0;
  }
  
  iltree_map_args(t, aggregate_knn, &hist);

  for(i=0; i<hist.num_bins; i++){
    if (hist.num[i])
      printf("%d %2.8g\n", hist.degs[i], hist.values[i] / hist.num[i] );
  }

  free(hist.degs);
  free(hist.num);
  free(hist.values);
  
}


/* print knn(k) using exponential (a.k.a. logarithmic) binning */

void print_knn_exp(knntree_t t, double alpha){
  
  unsigned int kmin, kmax, i;
  double width;
  knn_hist_t hist;
  
  if (!t){
    return;
  }

  
  kmin = ((knnsum_t*)iltree_getmin(t))->k;
  kmax = ((knnsum_t*)iltree_getmax(t))->k;
  
  
  hist.num_bins = (int)ceil(log(kmax)/log(alpha)) + 1;
  
  
  hist.degs = malloc(hist.num_bins * sizeof(unsigned int));
  hist.num = malloc(hist.num_bins * sizeof(unsigned int));
  hist.values = malloc(hist.num_bins * sizeof(double));

  width = 2;
  
  hist.degs[0] = kmin + width;
  hist.values[0] = 0;
  hist.num[0] = 0;

  for(i=1; i<hist.num_bins; i++){
    hist.values[i] = 0;
    hist.num[i] = 0;
    width *= alpha;
    hist.degs[i] = (int)ceil(hist.degs[i-1] + width);
  }
  
  iltree_map_args(t, aggregate_knn, &hist);

  for(i=0; i<hist.num_bins; i++){
    if(hist.num[i])
      printf("%d %2.8g\n", hist.degs[i], hist.values[i] / hist.num[i] );
  }
  
  free(hist.degs);
  free(hist.num);
  free(hist.values);
  
}



int main(int argc, char *argv[]){
  
  unsigned int N, K;
  unsigned int *J_slap=NULL, *r_slap=NULL;
  FILE *filein;
  char bin_type;
  unsigned int num_bins;
  double alpha;
  knntree_t t = NULL;
  
  

  if(argc < 2){
    usage(argv);
    exit(1);
  }
  
  if(!strcmp(argv[1], "-")){
    filein = stdin;
  }
  else{
    filein = openfile_or_exit(argv[1], "r", 2);
  }

  t = iltree_create(t);

  bin_type = NO_BIN;
  if (argc > 2){ /* the user has specified a binning type */
    if (!my_strcasecmp(argv[2], "lin")){
      bin_type = BIN_LIN;
      if(argc < 4){
        fprintf(stderr, "you must provide a number of bins for linear binning\n");
        exit(3);
      }
      num_bins = atoi(argv[3]);
    }
    else if (!my_strcasecmp(argv[2], "exp")){
      bin_type = BIN_EXP;
      if(argc < 4){
        fprintf(stderr, "you must provide an exponent for exponential binning\n");
        exit(4);
      }
      alpha = atof(argv[3]);
    }
  }
  
  read_slap(filein, &K, &N, &J_slap, &r_slap);
  fclose(filein);
  
  t = knntree_init(t, stdout);
  compute_knn(J_slap, r_slap, N, t);
  
  
  switch(bin_type){
  case BIN_LIN:
    print_knn_lin(t, num_bins);
    break;
  case BIN_EXP:
    print_knn_exp(t, alpha);
    break;
  default:
    print_knn_nobin(t);
    break;
  }
  iltree_destroy(t);
  free(J_slap);
  free(r_slap);
}
