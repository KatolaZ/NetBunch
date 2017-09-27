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
 *  This program finds the communities in a graph using the
 *  label-propagation algorithm proposed by Raghavan, Albert, and
 *  Kumara.
 *
 *  References:
 * 
 * [1] U. N. Raghavan, R. Albert, and S. Kumara. "Near linear time
 *     algorithm to detect community structures in large-scale
 *     networks". Phys. Rev. E 76 (2007), 036106.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>


#include "iltree.h"
#include "utils.h"


typedef struct{
  int label;
  int freq;
} label_freq_t;

#define MODE_SYNC  0x0
#define MODE_ASYNC 0x1



/* Usage */
void usage(char *argv[]){
  printf("********************************************************************\n"
         "**                                                                **\n"
         "**                   -*-    label_prop    -*-                     **\n"
         "**                                                                **\n"
         "**   Find the communities in 'graph_in' using the label           **\n"
         "**   propagation algorithm.                                       **\n"
         "**                                                                **\n"
         "**   The first parameter is used to choose between synchronous    **\n"
         "**   (SYNC) and asynchronous (ASYNC) update.                      **\n"
         "**                                                                **\n"
         "**   The input file 'graph_in' is an edge-list.                   **\n"
         "**   If 'graph_in' is equal to '-' (dash), read the file from     **\n" 
         "**   the standard input (STDIN).                                  **\n"
         "**                                                                **\n"
         "**   If 'max_epochs' is specified, the program stops after        **\n"
         "**   'max_epochs' epochs (useful in conjunction with SYNC, to     **\n"
         "**   exit from loops).                                            **\n"
         "**                                                                **\n"
         "**   The program prints on STDOUT the partition obtained when     **\n"
         "**   no more label flips are possible, in the format:             **\n"
         "**                                                                **\n"
         "**        node_1 comm_1                                           **\n"
         "**        node_2 comm_2                                           **\n"
         "**        node_3 comm_3                                           **\n"
         "**       .....                                                    **\n"
         "**                                                                **\n"
         "**   where 'comm_1' is the community to which 'node_1' belongs.   **\n"
         "**                                                                **\n"
         "**   The program prints on STDERR one line for each epoch,        **\n"
         "**   in the format:                                               **\n"
         "**                                                                **\n"
         "**       epoch_1 Q_1 flips_1                                      **\n"
         "**       epoch_2 Q_2 flips_2                                      **\n"
         "**       .....                                                    **\n"
         "**                                                                **\n"
         "**   where 'epoch_i' is the epoch number, 'Q_i' is the modularity **\n"
         "**   of the partition found at that epoch, and 'flips_i' is the   **\n"
         "**   number of label flips occurred in 'epoch_i'.                 **\n"
         "**                                                                **\n"
         "********************************************************************\n"
         " This is Free Software - You can use and distribute it under \n"
         " the terms of the GNU General Public License, version 3 or later\n\n"
         " Please visit http://www.complex-networks.net for more information\n\n"
         " (c) Vincenzo Nicosia 2009-2017 (v.nicosia@qmul.ac.uk)\n"
         "********************************************************************\n\n"
         );
  printf("Usage: %s [SYNC|ASYNC] <graph_in> [<max_epochs>]\n\n" , argv[0]);
}




/* Compare the frequency of two labels and return a value which allows
   to sort them in reverse order (i.e., -v, if v=f1-f2) */

int compare_label_freq_reverse(const void *e1, const void *e2){
  
  label_freq_t v1, v2;

  v1 = *((label_freq_t*)e1);
  v2 = *((label_freq_t*)e2);
  
  return - (v1.freq - v2.freq);

}

/* get the most common label in neighs (that is the list of the k
   neighbours of a node) */
unsigned int get_most_common_label(unsigned int *neighs, unsigned int k, 
                                   unsigned int *labels, unsigned int ref_label, 
                                   int *is_max){
  
  static label_freq_t *neigh_labels = NULL;
  static int size = 0;
  int num, i, j, max_freq;
  
  if (size < k){
    size = k;
    neigh_labels = realloc(neigh_labels, size * sizeof(label_freq_t));
  }
  
  neigh_labels[0].label = labels[neighs[0]];
  neigh_labels[0].freq = 1;
  num = 1;
  
  for (i=1; i<k; i ++){
    for(j=0; j<num; j++){
      if (labels[neighs[i]] == neigh_labels[j].label)
        break;
    }
    if (j == num){ /* new label */
      neigh_labels[j].label = labels[neighs[i]];
      neigh_labels[j].freq = 1;
      num += 1;
    }
    else{/* the label already exists  -> increase the counter */
      neigh_labels[j].freq += 1;
    }
  }

  /* Now we sort the array neigh_labels */
  qsort(neigh_labels, num, sizeof(label_freq_t), compare_label_freq_reverse);
  
  /* we determine how many neighbours have the maximum freq*/
  max_freq = neigh_labels[0].freq;
  i = 1;
  while(i < num && neigh_labels[i].freq == max_freq){
    i ++;
  }

  /* check whether ref_label is one of the most common labels */
  *is_max = 0;
  for (j=0; j<i; j++){
    if (neigh_labels[j].label == ref_label)
      *is_max = 1;
  }
  /* now that we know that there are "i" max_freqs, let's select one
     of them at random */
  j = rand() % i;
  
  return neigh_labels[j].label;
}

/* reassign the labels so that communities are numbered from 1 to NC */
int normalise_labels(unsigned int *labels, unsigned int N, 
                     unsigned int *label_count){
  
  unsigned int *label_map;
  int i, j, num = 0;
  
  
  label_map = malloc(N * sizeof(unsigned int));
  
  label_map[0] = labels[0];
  labels[0] = 0;
  label_count[0] = 1;
  num = 1;
  
  for(i=1; i<N; i ++){
    for(j=0; j<num; j++){
      if (labels[i] == label_map[j])
        break;
    }
    if (j == num){
      label_map[j] = labels[i];
      label_count[j] = 0;
      num +=1;
    }
    labels[i] = j;
    label_count[j] += 1;
  }

  free(label_map);
  return num;
}


void dump_partition(unsigned int *labels, unsigned int *label_count, unsigned int N){
  
  int i;
  
  for(i=0; i<N; i ++){
    fprintf(stdout, "%d %d\n", i, labels[i]);//, label_count[labels[i]]);
  }
}


/* compute the modularity of the current partition */

double modularity(unsigned int *J_slap, unsigned int *r_slap, unsigned int N,
                  unsigned int K, unsigned int *comm, unsigned int NC){

  double Q=0;
  int i, j;
  unsigned int c_i, c_j;
  double *pmm, *am;

  pmm = malloc(NC * sizeof(double));
  am = malloc(NC * sizeof(double));

  for (i=0; i<NC; i++){
    pmm[i] = am[i] = 0;
  }

  for(i=0; i<N; i ++){
    c_i = comm[i];
    am[c_i] += degree(r_slap, i);
    for(j=r_slap[i]; j<r_slap[i+1]; j++){
      c_j = comm[J_slap[j]]; 
      if ( c_j == c_i ){
        pmm[c_i] += 0.5;
      }
    }
  }
  Q = 0.0;
  for(i=0; i < NC; i++){
    Q += (pmm[i]* 2.0 / K - pow((am[i] * 1.0 / K), 2));
  }
  free(am);
  free(pmm);
  return Q;
}



unsigned int* label_propagation(unsigned int *J_slap, unsigned int *r_slap, unsigned int N, 
                                unsigned int K, unsigned int *num_epochs,
                                int max_epochs, char mode){
  
  unsigned int *labels, *next_labels, *tmp_labels, *ids, tmp, new_label;
  int i, epochs, j, k, cont, is_max;
  long long int num_flips;
  double Q;


  
  labels = malloc(N * sizeof(unsigned int));
  ids = malloc(N * sizeof(unsigned int));
  
  if(mode == MODE_ASYNC){
    next_labels = labels;
  }
  else if (mode == MODE_SYNC){
    next_labels = malloc(N * sizeof(unsigned int));
  }

  /* We initialize the list of ids and labels */
  for (i=0; i<N; i ++){
    ids[i] = labels[i] = i;
  }
  
  cont = 1;
  epochs = 0;
  while(cont){
    if (max_epochs > 0 && epochs > max_epochs)
      break;
    cont = 0;
    if (epochs > 0){
      Q= modularity(J_slap, r_slap, N, K, labels, N);
      fprintf(stderr, "%d %g %g\n", epochs, Q, (double)num_flips);
    }
    num_flips = 0;

    epochs += 1;
    for (i=N-1; i>=0; i--){
      j = rand() % (i+1);
      tmp = ids[j]; /* This is the id to be considered */

      ids[j] = ids[i];
      ids[i] = tmp;
      k = r_slap[tmp + 1] - r_slap[tmp];
      new_label = get_most_common_label(J_slap+r_slap[tmp], k,labels, labels[tmp], &is_max);
      
      /* Stop criterion: if the new label is not equal to the
         old one, continue to another epoch */
      
      if (mode == MODE_ASYNC  && labels[tmp] != new_label){
        labels[tmp] = new_label;
        cont = 1;
        num_flips += 1 ;
      }
      if (mode == MODE_SYNC){
        next_labels[tmp] = new_label;
        if (labels[tmp] != next_labels[tmp]){
          cont = 1;
          num_flips += 1;
        }
      }
    }
    if (mode == MODE_SYNC){
      /* Now we can swap labels and next_labels */
      tmp_labels = labels;
      labels = next_labels;
      next_labels = tmp_labels;
    }
  }
  free(ids);
  *num_epochs = epochs - 1;

  if (mode == MODE_SYNC){
    free(next_labels);
  }
  
  return labels;
}






int main(int argc, char *argv[]){
  
  unsigned int N, K, nc;
  unsigned int *J_slap, *r_slap, *labels, *label_count, num_epochs, max_epochs;
  FILE *filein;
  double Q;
  char mode;

  
  if (argc < 3){
    usage(argv);
    exit(1);
  }
  
  srand(time(NULL));
  
  if (!strcmp(argv[1], "-")){
    /* take the input from STDIN */
    filein = stdin;
  }
  else {
    filein = openfile_or_exit(argv[2], "r", 2);
  }

  
  read_slap(filein, &K, &N, &J_slap, &r_slap);

  fclose(filein);
  
  if (!my_strcasecmp(argv[1], "sync")){
    mode = MODE_SYNC;
  }
  else{
    mode = MODE_ASYNC;
  }
  
  if (argc > 3)
    max_epochs = atoi(argv[3]);
  else
    max_epochs = 0;
  
  labels = label_propagation(J_slap, r_slap, N, K, &num_epochs, max_epochs, mode);
  label_count = malloc(N * sizeof(unsigned int));
  
  nc = normalise_labels(labels, N, label_count);
  
  Q= modularity(J_slap, r_slap, N, K, labels, nc);
  
  printf("### nc: %d Q_max: %f Epochs: %d\n", nc, Q, num_epochs);
  dump_partition(labels, label_count,  N);
  free(J_slap);
  free(r_slap);
  free(label_count);
  free(labels);
}



