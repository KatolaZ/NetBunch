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
 *  This program computes the betweenness of all the nodes of a graph,
 *  using Brandes' algorithm, and counting all the shortest paths
 *  originating from a set of nodes (potentially the whole set of
 *  vertices).
 *
 *  References: 
 *     U. Brandes. "A Faster Algorithm for Betweenness 
 *     Centrality". J. Math. Sociol. 25 (2001), 163-177.  
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#include "utils.h"


void usage(char *argv[]){
  printf("********************************************************************\n"
         "**                                                                **\n"
         "**                      -*-  betweenness  -*-                     **\n"
         "**                                                                **\n"
         "**  Compute the betweenness of all the nodes and edges of a       **\n"
         "**  network due to the shortest paths originating from a set      **\n"
         "**  of initial nodes.  The set can be either a sequence of        **\n"
         "**  nodes (if the second argument is 'SEQ') or a random sample    **\n"
         "**  from the set of all the nodes (if it is 'RND').               **\n"
         "**                                                                **\n"
         "**  The input file is an edge-list (use '-' to read from STDIN).  **\n"
         "**                                                                **\n"
         "**  With 'SEQ':                                                   **\n"
         "**  If <node_start> is not specified, computes the betweenness    **\n"
         "**  using shortest paths from *all* the nodes (WARNING: This can  **\n"
         "**  be slow for large graphs!). If <node_end> is not specified,   **\n"
         "**  use shortest paths from all the nodes between <node_start>    **\n"
         "**  and the node with the largest label.                          **\n"
         "**                                                                **\n"
         "**  With 'RND':                                                   **\n"
         "**  Compute the betweenness based on the shortest paths from      **\n"
         "**  <num> nodes sampled uniformly at random.                      **\n"
         "**                                                                **\n"
         "**  When called with just <graph_in>, use all the nodes.          **\n"
         "**                                                                **\n"
         "**  The betweenness of all the nodes is printed on standard       **\n"
         "**  output (STDOUT), while the edge betweenness is printed on     **\n"
         "**  standard error (STDERR)                                       **\n"
         "**                                                                **\n"
         "********************************************************************\n"
         "\n\n"
         " This is Free Software - You can use and distribute it under \n"
         " the terms of the GNU General Public License, version 3 or later\n\n"
         " (c) Vincenzo Nicosia 2009-2017 (v.nicosia@qmul.ac.uk)\n\n");
  printf("Usage: %s <graph_in> [SEQ <node_start> [<node_end>]]\n" , argv[0]);
  printf("Usage: %s <graph_in> [RND <num>]\n" , argv[0]);

}

/*
 * Add a node the a list of predecessors
 */

void add_predecessor(unsigned int **pred, unsigned int k){
  
  (*pred)[0] += 1;
  *pred = realloc(*pred, ((*pred)[0] + 1)  * sizeof(unsigned int));
  (*pred)[ (*pred)[0] ] = k;
}



/*
 *
 * Compute node and edge betweenness, based on shortest paths
 * originating on the "num" nodes specified in "nlist".  "edge_bet"
 * should be an appropriately allocated (and initialised to zero!!!!)
 * vector of length equal to J_slap, and will contain the values of
 * edge betweenness.
 *
 */
double* compute_bet_dependency(unsigned int N, unsigned int *J_slap, unsigned int *r_slap, 
                               unsigned int *nlist, unsigned int num, double *edge_bet){
  
  int i, j, k, w, idx, cur_node, m;
  unsigned int  *marked, **preds, *dist, *nj;
  double  *delta, *cB, val;
  unsigned int d;
  unsigned int n, nd, ndp;
  unsigned int edge_pos;
  
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

  for (m=0; m<num; m++){
    /* We start from the m-th node in nlist*/
    j = nlist[m];

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
        val = 1.0 * nj[i] / nj[w] * (1 + delta[w]);
        delta[i] += val;
        /* Now we should update the betweenness of the edge (i,w) in
           the appropriate position of the vector  edge_bet*/
        find_neigh_in_Jslap(J_slap, r_slap, N, i, w, &edge_pos);
        edge_bet[edge_pos] += val;
        find_neigh_in_Jslap(J_slap, r_slap, N, w, i, &edge_pos);
        edge_bet[edge_pos] += val;
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



/*
 * print of STDOUT the betweenness of each node
 */
void dump_cB(double *cB, unsigned int N){

  unsigned int i;
  for (i=0; i<N; i++){
    printf("%g\n", cB[i]);
  }
}

/*
 * print on STDERR the betweenness of each edge
 */

void dump_edge_bet(unsigned int *J_slap, unsigned int *r_slap, unsigned int N,
                   double *v,  FILE *fout){

  unsigned int i, j;

  for(i=0; i<N; i++){
    for (j=r_slap[i]; j<r_slap[i+1]; j++)
      fprintf(fout, "%d %d %g\n", i, J_slap[j], v[j]);
  }
}



int main(int argc, char *argv[]){
  
  unsigned int *J_slap=NULL, *r_slap=NULL;
  unsigned int K, N, i;
  unsigned int n_start, n_end;
  unsigned int *nlist=NULL;
  unsigned int num;
  double *edge_bet = NULL;
  double *cB;
  FILE *filein;
  

  if(argc < 2){
    usage(argv);
    exit(1);
  }

  srand(time(NULL));

  if (!strcmp(argv[1], "-")){
    /* take the input from STDIN */
    filein = stdin;
  }
  else {
    filein = openfile_or_exit(argv[1], "r", 2);
  }

  read_slap(filein, &K, &N, &J_slap, &r_slap);

  nlist = malloc(N * sizeof(unsigned int));

  for(i=0; i<N; i++){
    nlist[i] = i;
  }

  n_start = 0;
  n_end = N-1;
  num = N;


  if (argc > 3){
    if(!my_strcasecmp(argv[2], "SEQ")){
      n_start = atoi(argv[3]);
      if (n_start > N-1)
        n_start = 0;
      if (argc > 4){
        n_end = atoi(argv[4]);
      }
      else{
        n_end = N-1;
      }
      num = n_end - n_start + 1;
    }
    else if (!my_strcasecmp(argv[2], "RND")){
      num = atoi(argv[3]);
      n_start = 0;
      n_end = num-1;
      if (num > N || num < 1)
        num = N;
      shuffle_vector(nlist, N);
    }
  }
  
  sort_neighbours(J_slap, r_slap, N);
  edge_bet = malloc(K * sizeof(double));
  memset(edge_bet, 0, K * sizeof(double));
  cB = compute_bet_dependency(N, J_slap, r_slap, nlist+n_start, num, edge_bet);

  dump_cB(cB, N);
  dump_edge_bet(J_slap, r_slap, N, edge_bet, stderr);

  free(cB);
  free(J_slap);
  free(r_slap);
  free(edge_bet);
  free(nlist);
  
}
