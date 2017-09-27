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
 *  This program finds the minimum/maximum spanning tree of a graph
 *  given as input, using the Kruskal's algorithm 
 *
 *
 *  References:
 *  
 * [1] J. B. Kruskal. "On the shortest spanning subtree of a graph and
 *     the traveling sales-man problem". P. Am. Math. Soc. 7 (1956),
 *     48-48.  
 *
 */



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dset.h"
#include "gen_heap.h"
#include "utils.h"
#include "edge_w_funs.h"


void usage(int argc, char *argv[]){

  printf("********************************************************************\n"
         "**                                                                **\n"
         "**                        -*-   kruskal  -*-                      **\n"
         "**                                                                **\n"
         "**   Find the minimum/maximum spanning tree of a weighted graph   **\n"
         "**   'graph_in' provided as input.                                **\n"
         "**                                                                **\n"
         "**   The input file 'graph_in' is a weighted edge-list:           **\n"
         "**                                                                **\n"
         "**                        I_1 J_1 W_1                             **\n"
         "**                        I_2 J_2 W_2                             **\n"
         "**                        I_3 J_3 W_3                             **\n"
         "**                        ... ...                                 **\n"
         "**                        I_K J_K W_K                             **\n"
         "**                                                                **\n"
         "**    The program computes by default the minimum spanning tree   **\n"
         "**    of 'graph_in', unless the second parameter 'MAX' is         **\n"
         "**    specified.                                                  **\n"
         "**                                                                **\n"
         "**    The program prints on STDOUT the weighted edge-list of the  **\n"
         "**    minimum/maximum spanning tree of 'graph_in'.                **\n"
         "**                                                                **\n"
         "**    If 'graph_in' is an unweighted graph, the program prints    **\n"
         "**    on output one of the spanning trees of the graph.           **\n"
         "**                                                                **\n"
         "********************************************************************\n"
         " This is Free Software - You can use and distribute it under \n"
         " the terms of the GNU General Public License, version 3 or later\n\n"
         " Please visit http://www.complex-networks.net for more information\n\n"
         " (c) Vincenzo Nicosia 2009-2017 (v.nicosia@qmul.ac.uk)\n"
         "********************************************************************\n\n"
         );
  printf("Usage: %s <graph_in> [MAX]\n", argv[0]);
}


void kruskal(gen_heap_t *h, unsigned int N){
  
  dset_t *nodes, set1, set2;
  edge_w_t *elem;
  unsigned int i;
  
  
  nodes = malloc(N * sizeof(dset_t));
  
  for(i=0; i<N; i++){
    nodes[i] = NULL;
    dset_makeset(nodes + i);
  }
  
  while(! gen_heap_delete(h, (void**)(&elem))){
    /* get the next edge */
    set1 = dset_find(nodes[elem->i]);
    set2 = dset_find(nodes[elem->j]);
    /* if i and j do not belong to the same disjoint set...*/
    if (set1 != set2){
      /* ... the edge (i,j) belongs to the spanning tree, */
      /* so we print it...*/
      print_elem_edge_w(elem);
      /* ...then merge the two sets... */
      dset_union(nodes[elem->i], nodes[elem->j]);
    }
    free(elem);
  }
  /* We now destroy all the disjoint sets... */
  for (i=0;i<N;i++){
    dset_destroy(nodes[i]);
  }
  free(nodes);
  
}

void load_edges_into_heap(FILE *filein, gen_heap_t *h){
  
  char buff[256];
  char *ptr;
  edge_w_t *elem;
  
  
  while(fgets(buff, 256, filein)){
    if (buff[0] == '#')
      continue;
    elem = malloc(sizeof(edge_w_t));
    ptr = strtok(buff, " "); /* read the first node */
    VALID_PTR_OR_EXIT(ptr, 7);
    elem->i = atoi(ptr);
    ptr = strtok(NULL, " "); /* read the second node */
    VALID_PTR_OR_EXIT(ptr, 7);
    elem->j = atoi(ptr);
    ptr = strtok(NULL, " "); /* read the weight */
    if (!ptr)
      /* if no weight is specified, assume it is set to 1.0 */
      elem->w = 1.0;
    else
      elem->w = atof(ptr);
    /* put the edge in the heap */
    gen_heap_insert(h, elem);
  }
  
}

int count_num_lines(FILE *filein){
  
  int i, ch ;

  i = 0;
  
  while ((ch = fgetc(filein)) != EOF){
      if (ch == '\n')
        i ++;
  }
  rewind(filein);
  return i;
}


int main(int argc, char *argv[]){
  
  gen_heap_t *h;
  unsigned int N;
  FILE *filein;
  char htype;
  gen_heap_func_t  *funs;
  
  if(argc < 2){
    usage(argc, argv);
    exit(1);
  }

  htype = MIN_HEAP;
  if (argc > 2 && !my_strcasecmp(argv[2], "MAX")){
    htype = MAX_HEAP;
  }

  /* Initialisation of functions for gen_heap*/
  funs = malloc(sizeof(gen_heap_func_t));
  (*funs).compare = compare_edge_w;
  (*funs).alloc_vector = alloc_vector_edge_w;
  (*funs).dealloc_vector = dealloc_vector_edge_w;
  (*funs).dealloc_elem = dealloc_elem_edge_w;
  (*funs).print_elem = print_elem_edge_w;


  
  filein = openfile_or_exit(argv[1], "r", 2);
  
  N = count_num_lines(filein);

  h = gen_heap_init(N, htype, funs);
  
  load_edges_into_heap(filein, h);

  fclose(filein);
  
  kruskal(h, N);
  gen_heap_destroy(h);
  free(funs);
}
