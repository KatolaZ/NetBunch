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
 *  This program finds the (size of the) strongly connected components
 *  of a directed graph given as input. We use the Kosaraju-Sharir
 *  algorithm, which performs two DFS scans of the graph, the first
 *  time using the original DFS algorithm on the graph, and the second
 *  time using the transposed graph and a slightly modified version of
 *  "components", which traverses the nodes of the graph in descending
 *  order of finishing time.
 *
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
         "**                      -*-  strong_conn  -*-                     **\n"
         "**                                                                **\n"
         "**   Find the strongly connected components of a directed graph   **\n"
         "**   and print on output their size and/or the list of nodes      **\n"
         "**   belonging to each component. The first parameter 'graph_in'  **\n"
         "**   is the name of the file containing the edge list of the      **\n"
         "**   graph.                                                       **\n"
         "**                                                                **\n"
         "**   If 'graph_in' is equal to '-' (dash), read the edge list     **\n"
         "**   from standard input (STDIN).                                 **\n"
         "**                                                                **\n"
         "**   The program prints on output the SIZE of each strongly       **\n"
         "**   connected component of the graph, one per line:              **\n"
         "**                                                                **\n"
         "**      size_1                                                    **\n"
         "**      size_2                                                    **\n"
         "**      size_3                                                    **\n"
         "**                                                                **\n"
         "**    The sizes are in no particular order.                       **\n"
         "**                                                                **\n"
         "**    If 'SHOW' is given as second argument, the list of nodes    **\n"
         "**    belonging to each component is printed as well, in the      **\n"
         "**    format:                                                     **\n"
         "**                                                                **\n"
         "**      size_1: node_1 node_2 node_3 ....                         **\n"
         "**      size_2: node_1 node_2 node_3 ....                         **\n"
         "**      size_3: node_1 node_2 node_3 ....                         **\n"
         "**                                                                **\n"
         "**                                                                **\n"
         "********************************************************************\n"
         " This is Free Software - You can use and distribute it under \n"
         " the terms of the GNU General Public License, version 3 or later\n\n"
         " Please visit http://www.complex-networks.net for more information\n\n"
         " (c) Vincenzo Nicosia 2010-2017 (v.nicosia@qmul.ac.uk)\n"
         "********************************************************************\n\n"
         );
  printf("Usage: %s <graph_in> [SHOW]\n\n" , argv[0]);
}

int dfs(unsigned int i, unsigned int *J_slap, unsigned int *r_slap, 
        unsigned int N, unsigned int nc, 
        unsigned int *ic, unsigned int *f, 
        char reset){
  
  static unsigned int time = 0;
  unsigned int j, s;
  
  if(reset){
    time = 0;
  }
  
  ic[i] = nc;
  s = 1;
  
  for(j=r_slap[i]; j<r_slap[i+1]; j++){
    if (ic[J_slap[j]] == 0){
      s += dfs(J_slap[j], J_slap, r_slap, N, nc, ic, f, 0);
    }
  }
  f[time] = i;
  time += 1;
  return s;
}



/**
 * 
 * Find all the components of the given graph
 *
 */

int components(unsigned int *J_slap, unsigned int *r_slap,
                unsigned int N, unsigned int **ic, 
                unsigned int **f, unsigned int **sizes){

  unsigned int nc, s;
  unsigned int i;

  *ic = malloc(N * sizeof(unsigned int));
  *f = malloc(N * sizeof(unsigned int));
  *sizes = malloc(N * sizeof(unsigned int));
  
  for(i=0; i<N; i++){
    (*ic)[i] = 0;
    (*f)[i] = 0;
  }
  nc = 0;
  for(i=0; i<N; i++){
    while( (*ic) [i] != 0 && i < N)
      i += 1;
    if (i == N)
      break;
    nc += 1;
    if (nc ==1){
      s = dfs(i, J_slap, r_slap, N, nc, *ic, *f, 1);
    }
    else{
      s = dfs(i, J_slap, r_slap, N, nc, *ic, *f, 0);
    }
    //printf("s: %d\n", s);
    (*sizes)[nc] = s;
  }
  return nc;
}


/**
 * 
 * Find the components of the transposed graph, where the nodes are
 * visited in descending order of their finishing time 
 *
 */

int components_rev(unsigned int *J_slap, unsigned int *r_slap,
                   unsigned int N, unsigned int **ic, 
                   unsigned int *f, unsigned int **f_T, unsigned int **sizes){
  
  unsigned int nc, s;
  unsigned int  idx;
  int i;

  *ic = malloc(N * sizeof(unsigned int));
  *f_T = malloc(N * sizeof(unsigned int));
  *sizes = malloc((N+1) * sizeof(unsigned int));
  
  for(i=0; i<N; i++){
    (*ic)[i] = 0;
  }
  nc = 0;
  for(i=N-1; i>=0; i--){
    idx = f[i];
    while( i >0 && (*ic) [idx] != 0 ){
      i -= 1;
      idx = f[i];
    }
    if (i < 0)
      break;
    nc += 1;
    if (nc == 1){
      s = dfs(idx, J_slap, r_slap, N, nc, *ic, *f_T, 1);
    }
    else{
      s = dfs(idx, J_slap, r_slap, N, nc, *ic, *f_T, 1);
    }
    (*sizes)[nc] = s;
  }
  return nc;
}



int main(int argc, char *argv[]){


  FILE *filein;
  unsigned int N, K, N1, N2;
  unsigned int *I, *J, *J_slap, *r_slap, *J_slap_T, *r_slap_T;
  unsigned int *ic, *f, *sizes, *ic_T, *f_T, *sizes_T;
  unsigned int i, j, nc_T;
  char show = 0;

  if (argc < 2){
    usage(argv);
    exit(1);
  }
  if (argc == 3 && !my_strcasecmp("SHOW", argv[2])){
    show = 1;
  }  
  
  if (!strcmp(argv[1], "-")){
    /* take the input from STDIN */
    filein = stdin;
  }
  else {
    filein = openfile_or_exit(argv[1], "r", 2);
  }


  I = J = NULL;
  K = read_ij(filein, &I, &J);
  J_slap = J_slap_T = NULL;
  r_slap = r_slap_T = NULL;
  /* obtain the SLAP representation of the graph */
  N1 = convert_ij2slap(I, J, K, &r_slap, &J_slap);
  /* obtain the SLAP representation of the transposed graph */
  N2 = convert_ij2slap(J, I, K, &r_slap_T, &J_slap_T);
  N = N1 >= N2 ? N1 : N2;
  components(J_slap, r_slap, N, &ic, &f, &sizes);

  fclose(filein);

  
  nc_T = components_rev(J_slap_T, r_slap_T, N, &ic_T, f, &f_T, &sizes_T);

  for(i=1; i<=nc_T; i++){
    printf("%d", sizes_T[i]);
    if (show){
      printf(":");
        for(j=0; j<N; j++){
          if (ic_T[j] == i){
            printf(" %d", j);
          }
        }
      printf("\n");
    }
    else{
      printf("\n");
    }
  }
  free(J_slap);
  free(r_slap);
  free(J_slap_T);
  free(r_slap_T);
  free(ic);
  free(f); 
  free(sizes); 
  free(ic_T);
  free(f_T); 
  free(sizes_T); 
  free(I);
  free(J);
}
