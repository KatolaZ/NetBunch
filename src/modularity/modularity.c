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
 *  Take a graph and a partition, and compute the modularity function
 *  associated to that partition.
 *
 *  References:
 *  
 *  [1] M. E. J. Newman and M. Girvan. "Finding and evaluating
 *      community structure in networks". Phys. Rev. E 69, (2004),
 *      026113.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "utils.h"

void usage(char *argv[]){

  printf("********************************************************************\n"
         "**                                                                **\n"
         "**                    -*- modularity -*-                          **\n"
         "**                                                                **\n"
         "**  Compute the modularity function associated to a partition     **\n"
         "**  of the nodes of the graph provided as input.                  **\n"
         "**                                                                **\n"
         "**  If 'graph_in' is equal to '-' (dash), read the edge list      **\n"
         "**  from standard input (STDIN). The parameter 'partition' MUST   **\n"
         "**  be a file in the format:                                      **\n"
         "**                                                                **\n"
         "**    node_0 community_0                                          **\n"
         "**    node_1 community_1                                          **\n"
         "**    node_2 community_2                                          **\n"
         "**    .....                                                       **\n"
         "**                                                                **\n"
         "**  where 'node_0', 'node_1', etc. are node labels, and           **\n"
         "**  'community_0', 'community_1', etc. is the label of the        **\n"
         "**  community to which a node belongs. Notice that a node can     **\n"
         "**  belong to exactly one community. This format is compatible    **\n"
         "**  with the output of the programs which compute community       **\n"
         "**  partitions, such as `gn`, `cnm`, `label_prop`, etc.           **\n"
         "**                                                                **\n"
         "**  The program prints on STDOUT the modularity of the partition, **\n"
         "**  and prints on STDERR a single line in the format:             **\n"
         "**                                                                **\n"
         "**    ## nc: NUM_COMMUNITIES                                      **\n"
         "**                                                                **\n"
         "**  where 'NUM_COMMUNITIES' is the number of communities in the   **\n"
         "**  partition given as input.                                     **\n"
         "**                                                                **\n"
         "********************************************************************\n"
         " This is Free Software - You can use and distribute it under \n"
         " the terms of the GNU General Public License, version 3 or later\n\n"
         " Please visit http://www.complex-networks.net for more information\n\n"
         " (c) Vincenzo Nicosia 2009-2017 (v.nicosia@qmul.ac.uk)\n"
         "********************************************************************\n\n"
         );
  printf("Usage: %s <graph_in> <partition>\n", argv[0]);
}


/* This is the function that computes the value of the modularity function */

double compute_modularity(unsigned int *J_slap, unsigned int *r_slap, unsigned int N, 
                          unsigned int *part, unsigned int nc){
  static double *e, *a;
  
  unsigned int i, j, n, K, deg_i;
  unsigned int ci, cj;
  double Q;

  if(!e)
    e = malloc((N+1) * sizeof(double));
  if(!a)
    a = malloc((N+1) * sizeof(double));

  memset(e, 0, (N+1) * sizeof(double));
  memset(a, 0, (N+1) * sizeof(double));

  K = r_slap[N];
  
  for (i=0; i<N; i++){
    ci = part[i];
    deg_i = (r_slap[i+1] - r_slap[i]); 
    if (deg_i == 0)
      continue;
    a[ci] += deg_i;
    for(j=r_slap[i]; j< r_slap[i+1]; j++){
      cj = part[J_slap[j]];
      if (ci == cj){
        e[ci] += 1;
      }
    }
  }
  
  Q = 0.0;
  fprintf(stderr, "### nc: %d\n", nc);
  for (n=0; n<=nc; n++){
    Q += 1.0 * e[n]/(1.0 * K ) - pow(1.0 * a[n]/K, 2);
  }
  free(a);
  free(e);
  return Q;
}


/* 
 * This function remaps the original partition labels into a
 *  consecutive set of integers, starting at 0 (zero) 
 */
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


int main(int argc, char *argv[]){

  unsigned int N, K, nc;
  unsigned int *J_slap=NULL, *r_slap=NULL, *part = NULL;
  unsigned int *label_count;
  FILE *f_net, *f_part;
  double Q;


  if (argc < 3){
    usage(argv);
    exit(1);
  }

  if(!strcmp(argv[1], "-")){
    f_net = stdin;
  }
  else{
    f_net = openfile_or_exit(argv[1], "r", 2);
  }

  f_part = openfile_or_exit(argv[2], "r", 3);
  
  read_slap(f_net, &K, &N, &J_slap, &r_slap);

  part = malloc(N * sizeof(unsigned int));
  memset(part, 0, N * sizeof(unsigned int));

  label_count = malloc(N * sizeof(unsigned int));

  nc = read_partition(f_part, N, part);

  fclose(f_net);
  fclose(f_part);
  
  nc = normalise_labels(part, N, label_count);
  
  Q = compute_modularity(J_slap, r_slap, N, part, nc);

  printf("%g\n", Q);
  free(J_slap);
  free(r_slap);
  free(part);
  free(label_count);
}
