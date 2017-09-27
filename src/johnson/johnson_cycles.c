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
 *  Compute all the simple cycles of an undirected graph provided as
 *  input (optionally, only up to a certain length) using the Johnson
 *  algorithm.
 *
 *  References:
 *
 * [1] D. B. Johnson. "Finding All the Elementary Circuits of a
 *     Directed Graph". SIAM J. Comput. 4 (1975), 77-84.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "stack.h"


#define FALSE 0
#define TRUE  1


void usage(char *argv[]){

  printf("********************************************************************\n"
         "**                                                                **\n"
         "**                    -*- johnson_cycles -*-                      **\n"
         "**                                                                **\n"
         "**  Enumerates all the elementary cycles of a graph on input.     **\n"
         "**                                                                **\n"
         "**  If 'graph_in' is equal to '-' (dash), read the edge list      **\n"
         "**  from standard input (STDIN).                                  **\n"
         "**                                                                **\n"
         "**  If 'max_length' is provided, ignore all cycles whose length   **\n"
         "**  is larger than 'max_length'                                   **\n"
         "**                                                                **\n"
         "**  The program prints on output a row for each cycle length,     **\n"
         "**  in the format:                                                **\n"
         "**                                                                **\n"
         "**        2 N_2                                                   **\n"
         "**        3 N_3                                                   **\n"
         "**        4 N_4                                                   **\n"
         "**        5 N_5                                                   **\n"
         "**        ....                                                    **\n"
         "**                                                                **\n"
         "**  where 2, 3, 4, 5, ... is the length of the cycle, and N_2,    **\n"
         "**  N_3, N_4, N_5, ... is the number of cycles of that length.    **\n"
         "**  Notice that in undirected graphs each cycle is reported       **\n"
         "**  (and counted) twice, once for each possible direction in      **\n"
         "**  which the cycle can be traversed.                             **\n"
         "**                                                                **\n"         
         "**  If the third parameter is equal to 'SHOW', the program will   **\n"         
         "**  dump all the found cycles on STDERR, using the format:        **\n"
         "**                                                                **\n"
         "**           N_(l-1) N_(l-2)... N_0                               **\n"
         "**                                                                **\n"
         "**  where N_(l-i) is the i-th node in the cycle starting at node  **\n"
         "**  N_0. Notice that according to this notation we report only    **\n"
         "**  'l' nodes belonging to the cycle, in reversed traversal       **\n"
         "**  order, avoiding to report the starting node (N_0) twice.      **\n"
         "**                                                                **\n"
         "**  The program shows also  cycles of length '2', that in         **\n"
         "**  undirected graphs correspond to the degenerate cycle obtained **\n"
         "**  by traversing the same edge in the two directions.            **\n"
         "**                                                                **\n"
         "********************************************************************\n"
         " This is Free Software - You can use and distribute it under \n"
         " the terms of the GNU General Public License, version 3 or later\n\n"
         " Please visit http://www.complex-networks.net for more information\n\n"
         " (c) Vincenzo Nicosia 2009-2017 (v.nicosia@qmul.ac.uk)\n"
         "********************************************************************\n\n"
         );
  printf("Usage: %s <graph_in> [<max_length> [SHOW]]\n", argv[0]);

}


typedef struct{
  unsigned int num;
  double *c;
} count_t;


/**
 *
 * This structure contains all the information needed by the algorithm
 * to run:
 *
 * - N is the number of nodes
 *
 * - K is twice the number of edges
 *
 * - J_slap and r_slap are the usual SLAP representation of the
 *    adjacency matrix
 *
 * - B_len is an array with the length of the set B(u) for each node u
 *
 * - B is a vector of character of the same size of J_slap, which
 *   stores the values of the indicator vector B(u) for each node u
 * 
 * - blocked is an indicator vector of size N
 *
 * - v is the stack of nodes
 *
 * - stats maintains the counts for all the cycle lengths
 * 
 */
typedef struct{
  unsigned int N;
  unsigned int K;
  unsigned int *J_slap;
  unsigned int *r_slap;
  my_stack_t *stack;
  int max_depth;
  count_t stats;
  unsigned int *B_len;
  unsigned int *B;
  char *blocked;
  char show;
} algo_info_t;




void unblock(algo_info_t *info, unsigned int u){
  
  unsigned int w, idx;
  
  info->blocked[u] = FALSE;
  
  while(info->B_len[u] >0){
    info->B_len[u] -= 1;
    idx = info->r_slap[u] + info->B_len[u];
    w = info->B[idx];
    if (info->blocked[w] == TRUE){
      unblock(info, w);
    }
  }
}




/* add v to B(w) */
void set_B(algo_info_t *info, unsigned int v, unsigned int w){
  
  unsigned int idx, i;

  idx = info->r_slap[w];
  
  
  for(i=0; i <info->B_len[w]; i ++){
    if (info->B[idx + i] == v)
      return;
  }
  info->B[idx + info->B_len[w]] = v;
  info->B_len[w] += 1;
  
}

char circuit(algo_info_t *info, unsigned int s, unsigned int v){

  unsigned int i, w;
  char f = FALSE;
  
  if (v < s)
    return FALSE;
  
  /* check if maximum depth has been reached */
  if(stack_size(info->stack) == info->max_depth){
    return TRUE;
  }
  
  stack_push(info->stack, v);
  
  info->blocked[v] = TRUE;
  for(i=info->r_slap[v]; i< info->r_slap[v+1]; i++){
    w = info->J_slap[i];
    if (w == s){
      /* output a cycle here */
      if (info->show)
        stack_dump(info->stack, stderr);
      info->stats.c[stack_size(info->stack)] += 1;
      f = TRUE;
    }
    if (w < s)
      continue; /* We only consider nodes with higher IDs */
    else
      if (! (info->blocked[w])){
        if (circuit(info, s, w) == TRUE)
          f = TRUE;
      }
  }
  if (f == TRUE) {
    unblock(info, v);
  }
  else{
    for(i=info->r_slap[v]; i < info->r_slap[v+1]; i++){
      w = info->J_slap[i];
      if (w > s)
        set_B(info, v, w); /* add v to B(w) only if v is not already in B(w) */
    }
  }
  stack_pop(info->stack, &w);
  return f;
}


void johnson_cycles(algo_info_t *info){
  
  unsigned int i, N;
  
  N = info->N;
  
  
  for(i=0; i<N; i++){
    /* clear the vector of blocked nodes */
    memset(info->blocked, 0, N * sizeof(char));
    
    /* clear the indicator vector B */
    
    /* we reset the length of each  set B*/
    memset(info->B_len, 0, N * sizeof(unsigned int)); 
    circuit(info, i, i);
  }
  
  
}


int main(int argc, char *argv[]){
  
  algo_info_t info;
  FILE *filein;
  unsigned int i;

  if (argc < 2){
    usage(argv);
    exit(1);
  }
  
  info.J_slap = NULL;
  info.r_slap = NULL;
  

  if(!strcmp(argv[1], "-")){
    filein = stdin;
  }
  else{
    filein = openfile_or_exit(argv[1], "r", 2);
  }
  
  read_slap(filein, &(info.K), &(info.N), &(info.J_slap), &(info.r_slap));

  fclose(filein);
  
  info.B = malloc(info.r_slap[info.N] * sizeof(unsigned int)); /* This is the indicator vector B(u) */
  info.B_len = malloc(info.N * sizeof(unsigned int)); /* lengths of  the B(u) */
  info.blocked = malloc(info.N * sizeof(char)); /* This is the indicator vector
                                                   of blocked nodes */
  
  info.stack = malloc(sizeof(my_stack_t)); 
  stack_create(info.stack);
  
  info.show = FALSE;
  if (argc > 2){
    info.max_depth = atoi(argv[2]);
    if (argc > 3){
      if (!my_strcasecmp(argv[3], "show")){
        info.show = TRUE;
      }
    }
  }
  else{
    info.max_depth = info.N;
  }
  
  /* Here we initialise the counters */
  info.stats.num = info.max_depth + 1;
  info.stats.c = malloc(info.stats.num * sizeof(double));
  /* Here we set the counters to zero */
  memset(info.stats.c, 0, info.stats.num * sizeof(double));
  johnson_cycles(&info);
  
  for(i=2; i<info.stats.num ; i ++){
    printf("%d %1.20g\n", i, info.stats.c[i]);
  }
  free(info.B);
  free(info.B_len);
  free(info.blocked);
  free(info.stack->v);
  free(info.stack);
  free(info.stats.c);
  free(info.r_slap);
  free(info.J_slap);
}
