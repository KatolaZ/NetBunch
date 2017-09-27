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
 *  This program computes the betweenness dependency of all the nodes
 *  of a graph, using Brandes' algorithm, and counting all the
 *  shortest paths originating from a set of nodes (potentially the
 *  whole set of vertices).
 *
 *  References: 
 *     U. Brandes. "A Faster Algorithm for Betweenness 
 *     Centrality". J. Math. Sociol. 25 (2001), 163-177.  
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"


void usage(char *argv[]){
  printf("********************************************************************\n"
         "**                                                                **\n"
         "**                  -*-  bet_dependency  -*-                      **\n"
         "**                                                                **\n"
         "**  Compute the betweenness dependency of all the nodes of a      **\n"
         "**  network due to the shortest paths originating from a set      **\n"
         "**  of initial nodes. If no start node is specified, compute      **\n"
         "**  the dependency due to all the nodes of the graph. If no end   **\n"
         "**  node is specified, compute the dependency due to the nodes    **\n"
         "**  between node_start and the last node of the graph.            **\n"
         "**                                                                **\n"
         "**  The dependency of each node is printed on standard output.    **\n"
         "**                                                                **\n"
         "**                                                                **\n"
         "********************************************************************\n"
         "\n\n"
         " This is Free Software - You can use and distribute it under \n"
         " the terms of the GNU General Public License, version 3 or later\n\n"
         " (c) Vincenzo Nicosia 2009-2017 (katolaz@yahoo.it)\n\n");
  printf("Usage: %s <graph_in> [<node_start> [<node_end>]]\n\n" , argv[0]);
}

void add_predecessor(unsigned int **pred, unsigned int k){
  
  (*pred)[0] += 1;
  *pred = realloc(*pred, ((*pred)[0] + 1)  * sizeof(unsigned int));
  (*pred)[ (*pred)[0] ] = k;
}



/*
 * Returns a list of dependencies
 */
double* compute_bet_dependency(unsigned int N, unsigned int *J_slap, unsigned int *r_slap, 
                                      unsigned int n_start, unsigned int n_end){
  
  int i, j, k, w, idx, cur_node;
  unsigned int  *marked, **preds, *dist, *nj;
  double  *delta, *cB;
  unsigned int d;
  unsigned int n, nd, ndp;
  
  dist = malloc(N * sizeof(unsigned int));
  marked = malloc(N * sizeof(unsigned int));
  preds = malloc(N * sizeof(unsigned int *));
  
  nj = malloc(N * sizeof(unsigned int));
  delta = malloc(N * sizeof(double));
  cB = malloc(N * sizeof(double));
  
  for (i=0; i<N; i++){
    cB[i] = 0;
    preds[i] = NULL;
  }

  for (j=n_start; j<=n_end && j<N; j++){
    for(i=0; i<N; i ++){
      dist[i] = N;
      if (! preds[i]){
        preds[i] = malloc(sizeof(unsigned int));
      }
      preds[i][0] = 0; /* The list of predecessors is now empty! */
      
      nj[i] = 0;
      delta[i]= 0;
    }
    dist[j] = 0;
    nj[j] = 1;
    marked[0] = j;
    d = 0;
    n = 0;
    nd = 1;
    ndp = 0;
    while (d<N && nd > 0){
      for(i = n; i< n+nd; i ++){
        cur_node = marked[i];
        for (k=r_slap[cur_node]; k<r_slap[cur_node +1] ; k++){
          w = J_slap[k];
          if ( dist[w] == d+1){
            add_predecessor((unsigned int **)(preds + w), cur_node);
            nj[w] += nj[cur_node];
          }
          if ( dist[w] == N){
            dist[w] = d+1;
            marked[n + nd + ndp] = w;
            add_predecessor(preds + w, cur_node);
            ndp +=1;
            nj[w] += nj[cur_node];
          }
        }
      }
      n = n + nd;
      nd = ndp;
      ndp = 0;
      d += 1; 
    }
    for (k= n-1; k>=1; k--){
      w = marked[k];
      for (idx=1; idx <= preds[w][0]; idx ++ ){
        i = preds[w][idx];
        delta[i] += 1.0 * nj[i] / nj[w] * (1 + delta[w]);
      }
      cB[w] += delta[w];
    }
  }
  free(dist);
  free(marked);
  for (i=0; i<N; i++){
    free(preds[i]);
  }
  free(preds);
  free(nj);
  free(delta);
  
  return cB;
}



void dump_cB(double *cB, unsigned int N){

  unsigned int i;
  for (i=0; i<N; i++){
    printf("%g\n", cB[i]);
  }
}


int main(int argc, char *argv[]){
  
  unsigned int *J_slap=NULL, *r_slap=NULL;
  unsigned int K, N;
  unsigned int n_start, n_end;
  double *cB;
  FILE *filein;
  
  n_start = 0;
  n_end = -1;

  switch (argc){

  case 4:
    n_end = atoi(argv[3]);
  case 3:
    n_start = atoi(argv[2]);
  case 2:
    break;
  default:     
    usage(argv);
    exit(1);
    break;
  }
  
  if (!strcmp(argv[1], "-")){
    /* take the input from STDIN */
    filein = stdin;
  }
  else {
    filein = openfile_or_exit(argv[1], "r", 2);
  }

  read_slap(filein, &K, &N, &J_slap, &r_slap);
  if (n_end == -1)
    n_end = N-1;
  cB = compute_bet_dependency(N, J_slap, r_slap, n_start, n_end);
  dump_cB(cB, N);

  free(cB);
  free(J_slap);
  free(r_slap);
  
}
