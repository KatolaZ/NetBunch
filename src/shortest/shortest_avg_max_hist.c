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
 *  This program computes the distance from a given node to all the
 *  other nodes of an undirected graph, using the Breadth-First Search
 *  algorithm. 
 *
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"


void usage(char *argv[]){
  printf("********************************************************************\n"
         "**                                                                **\n"
         "**                -*-  shortest_avg_max_hist  -*-                 **\n"
         "**                                                                **\n"
         "**  Compute the distance from the given 'node' to all the other   **\n"
         "**  nodes of an undirected graph. The first parameter 'graph_in'  **\n"
         "**  is the name of the file containing the edge list of the       **\n"
         "**  graph. The second parameter 'node' is the label of the node   **\n"
         "**  for which we want to compute the distances to all the other   **\n"
         "**  nodes.                                                        **\n"
         "**                                                                **\n"
         "**  If 'graph_in' is equal to '-' (dash), read the edge list      **\n"
         "**  from standard input (STDIN)                                   **\n"
         "**                                                                **\n"
         "**  The program prints on output a row containing:                **\n"
         "**                                                                **\n"
         "**   - The average shortest path length between 'node' and        **\n"
         "**     all the other nodes                                        **\n"
         "**   - The maximum distance to any other node (eccentricity)      **\n" 
         "**   - The number of nodes at distance 1, 2, 3, .... from 'node'  **\n" 
         "**                                                                **\n"         
         "********************************************************************\n"
         " This is Free Software - You can use and distribute it under \n"
         " the terms of the GNU General Public License, version 3 or later\n\n"
         " Please visit http://www.complex-networks.net for more information\n\n"
         " (c) Vincenzo Nicosia 2010-2017 (v.nicosia@qmul.ac.uk)\n"
         "********************************************************************\n\n"
         );
  printf("Usage: %s <graph_in> <node>\n\n" , argv[0]);
}


/*
 * Add 'k' to a list of predecessors
 */

void add_predecessor(unsigned int **pred, unsigned int k){
  
  (*pred)[0] += 1;
  *pred = realloc(*pred, ((*pred)[0] + 1)  * sizeof(unsigned int));
  (*pred)[ (*pred)[0] ] = k;
}



/*
 *  
 * This is the implementation of the Breadth-First Search algorithm
 * (BFS) to compute the shortest paths, and the distances, between a
 * given node 'i' and all the other nodes of a graph. 
 * 
 */
unsigned int** compute_shortest_paths(unsigned int N, unsigned int *J_slap, unsigned int *r_slap, 
                            unsigned int i, unsigned int **dist){
  
  unsigned int j, k, cur_node;
  unsigned int  *marked, **preds;
  unsigned int d;
  unsigned int n, nd, ndp;
  
  *dist = malloc(N * sizeof(unsigned int));
  marked = malloc(N * sizeof(unsigned int));
  preds = malloc(N * sizeof(unsigned int *));
  
  for(j=0; j<N; j ++){
    (*dist)[j] = N;
    preds[j] = malloc(sizeof(unsigned int));
    preds[j][0] = 0; /* The list of predecessors is empty! */
  }
  (*dist)[i] = 0;
  marked[0] = i;
  d = 0;
  n = 0;
  nd = 1;
  ndp = 0;
  while (d<N && nd > 0){
    for(k = n; k< n+nd; k ++){
      cur_node = marked[k];
      for (j=r_slap[cur_node]; j<r_slap[cur_node +1] ; j++){
        if ( (*dist)[ J_slap[j] ] == d+1){
          add_predecessor((unsigned int **)(preds + J_slap[j]), cur_node);
        }
        if ( (*dist)[ J_slap[j] ] == N){
          (*dist)[ J_slap[j] ] = d+1;
          marked[n + nd + ndp] = J_slap[j];
          add_predecessor(preds + J_slap[j], cur_node);
          ndp +=1;
        }
      }
    
    }
    n = n + nd;
    nd = ndp;
    ndp = 0;
    d += 1; 
  }
  free(marked);
  return preds;
}


void dump_avg_max_hist(unsigned int *dists, unsigned int N){

  unsigned int i;
  double res = 0;
  double max = 0;
  double *hist;

  for (i=0; i<N; i++){
    res += dists[i];
    if (dists[i] > max)
      max = dists[i];
  }
  hist = malloc((max + 1) * sizeof(double));
  for (i=0; i<=max; i++){
    hist[i] = 0;
  }
  for (i=0; i<N; i++){
    if(dists[i]){
      hist[dists[i]] ++;
    }
  }
  
  res = res/(N-1);
  
  printf("%g %g", res, max);
  for (i=1; i<=max; i++){
    printf(" %g", hist[i]);
  }
  printf("\n");
  free(hist);
}




int main(int argc, char *argv[]){
  
  unsigned int *J_slap=NULL, *r_slap=NULL;
  unsigned int K, N, i;
  unsigned int **preds, *dists=NULL;
  FILE *filein;
  
  if (argc < 3){
    usage(argv);
    exit(1);
  }

  if (!strcmp(argv[1], "-")){
    /* take the input from STDIN */
    filein = stdin;
  }
  else {
    filein = openfile_or_exit(argv[1], "r", 2);
  }
  
  read_slap(filein, &K, &N, &J_slap, &r_slap);
  fclose(filein);
  i = atoi(argv[2]);
  if (i>N){
    printf("Node id '%d' does not exist!!!! Exiting....\n", i);
    exit(3);
  }
  preds = compute_shortest_paths(N, J_slap, r_slap, i, &dists);
  dump_avg_max_hist(dists, N);

  /* Cleanup */
  
  for (i=0; i<N; i++){
    free(preds[i]);
  }
  free(preds);
  free(dists);
  free(J_slap);
  free(r_slap);
}
