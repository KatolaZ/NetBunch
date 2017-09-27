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
 *
 *  This program implements the Dijkstra's algorithm to find the
 *  shortest paths from a given node (source) to all the other nodes
 *  of a weighted graph given as input.
 *
 *  References:
 * 
 *  [1] E. W. Dijkstra. "A Note on Two Problems in Connexion with
 *      Graphs". Num. Math.  1 (1959), 269-271.
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#include "utils.h"
#include "gen_pqueue.h"

#include "node_w_funs.h"

#define INFINITY DBL_MAX


void usage(char *argv[]){
  printf("********************************************************************\n"
         "**                                                                **\n"
         "**                        -*-  dijkstra  -*-                      **\n"
         "**                                                                **\n"
         "**  Compute the distance from the given 'node' to all the other   **\n"
         "**  nodes of a weighted graph. The first parameter 'graph_in'     **\n"
         "**  is the name of the file containing the edge list of the       **\n"
         "**  graph. The second parameter 'node' is the label of the node   **\n"
         "**  for which we want to compute the distances to all the other   **\n"
         "**  nodes.                                                        **\n"
         "**                                                                **\n"
         "**  If 'graph_in' is equal to '-' (dash), read the edge list      **\n"
         "**  from standard input (STDIN)                                   **\n"
         "**                                                                **\n"
         "**  The program prints on output a row of values:                 **\n"
         "**                                                                **\n"
         "**        d0 d1 d2 d3 d4......                                    **\n"
         "**                                                                **\n"
         "**  where d0 is the distance between 'node' and '0', 'd1' is the  **\n"
         "**  distance between 'node' and '1', and so on.  If a node is     **\n"
         "**  not reachable from <node>, the corresponding distance is set  **\n"         
         "**  to 'inf'.                                                     **\n"         
         "**                                                                **\n"         
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



void init_prio_queue(gen_pqueue_t **q, unsigned int N){
  
  gen_pqueue_func_t *funs;
  
  funs = malloc(sizeof(gen_pqueue_func_t));
  (*funs).compare = compare_node_w;
  (*funs).alloc_vector = alloc_vector_node_w;
  (*funs).dealloc_vector = dealloc_vector_node_w;
  (*funs).dealloc_elem = dealloc_elem_node_w;
  (*funs).print_elem = print_elem_node_w;
  (*funs).get_id = get_id_node_w;
  (*funs).set_key = set_key_node_w;
  (*funs).get_key = get_key_node_w;
  (*funs).compare_to_key = compare_to_key_node_w;
  
  *q = gen_pqueue_init(N, MIN_QUEUE, funs);
  free(funs);
}

void flatten_pqueue_to_array(gen_pqueue_t *q, double *v, unsigned int N){
  
  int i;
  double *val;
  
  for(i=0; i<N; i++){
    val = gen_pqueue_get_key(q, gen_pqueue_get_handle(q, i));
    v[i] = *val;
  }
}


void dijkstra(unsigned int N, unsigned int K, unsigned int *r_slap, unsigned int *J_slap, 
              double *W_slap, int s, double *dist, int *prev){
  
  int i, cur_node, n;
  double dist_cur_node, dist_neigh_node, w;
  
  gen_pqueue_t *qdist;
  node_w_t *elem;
  
  
  init_prio_queue(&qdist, N);
  
  for (i=0; i<N; i++){
    elem = malloc(sizeof(node_w_t));
    elem->i = i;
    if (i != s)
      elem->w = INFINITY;
    else
      elem->w = 0.0;
    gen_pqueue_insert(qdist, elem);
    prev[i] = -1;
    dist[i] = INFINITY;
  }
  
  
  while (!gen_pqueue_delete(qdist, (void**)&elem)){ /* get the node with minimum distance */
    cur_node = qdist->funs.get_id(elem);
    dist_cur_node = *((double*) qdist->funs.get_key(elem));
    if (dist_cur_node == INFINITY){ /* No more nodes are reachable from the origin */
      free(elem);
      gen_pqueue_destroy(qdist);
      return;
    }
    dist[cur_node] = dist_cur_node;
    for(i=r_slap[cur_node]; i<r_slap[cur_node + 1]; i++){
      n = J_slap[i];
      w = W_slap[i];
      
      dist_neigh_node = dist_cur_node + w;
      if (dist[n] >= dist_neigh_node){
        /* now we call gen_pqueue_change_key, which updates the key of node n */
        gen_pqueue_change_key(qdist, gen_pqueue_get_handle(qdist, n), &dist_neigh_node);
        prev[n] = cur_node;
      }
    }
    free(elem);
  }
  gen_pqueue_destroy(qdist);
}


void dump_dists(double *v, unsigned int N){
  
  int i;

  for(i=0; i<N-1; i++){
    if (v[i] == INFINITY){
      printf("inf ");
    }
    else{
      printf("%g ", v[i]);
    }
  }
  if (v[N-1] == INFINITY){
    printf("inf\n");
  }
  else{
    printf("%g\n", v[N-1]);
  }
}

int main(int argc, char *argv[]){
  
  
  double *dists;
  int *prev;
  unsigned int N, K, s;
  unsigned int *r_slap = NULL, *J_slap=NULL;
  double *W_slap = NULL;
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
  
  read_slap_w(filein, &K, &N, &J_slap, &r_slap, &W_slap);

  fclose(filein);
  
  s = atoi(argv[2]);
  if (s >= N){
    fprintf(stderr, "Error!!! Source node index should be in the range [0:%d]\n", 
            N-1);
    exit(3);
  }
  
  dists = malloc(N * sizeof(double));
  prev = malloc(N * sizeof(int));

  dijkstra(N, K, r_slap, J_slap, W_slap, s, dists, prev);
  dump_dists(dists, N);
  free(J_slap);
  free(r_slap);
  free(W_slap);
  free(dists);
  free(prev);
}
