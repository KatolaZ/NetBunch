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
 *  This program implements the Depth-First Search algorithm to find
 *  the connected components of a graph provided as input.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"



void usage(char *argv[]){
  printf("********************************************************************\n"
         "**                                                                **\n"
         "**                      -*-  components  -*-                      **\n"
         "**                                                                **\n"
         "**   Find the connected components of an undirected graph         **\n"
         "**   and print on output their size and/or the list of nodes      **\n"
         "**   belonging to each component. The first parameter 'graph_in'  **\n"
         "**   is the name of the file containing the edge list of the      **\n"
         "**   graph.                                                       **\n"
         "**                                                                **\n"
         "**   If 'graph_in' is equal to '-' (dash), read the edge list     **\n"
         "**   from standard input (STDIN).                                 **\n"
         "**                                                                **\n"
         "**   The program prints on output the SIZE of each connected      **\n"
         "**   component of the graph, one per line:                        **\n"
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



/**
 *
 * Depth-First search on node i....
 *
 */
int dfs(unsigned int i, unsigned int *J_slap, unsigned int *r_slap, 
        unsigned int N, unsigned int nc, 
        unsigned int *ic, unsigned int *f, 
        char reset){
  
  static unsigned int time;
  unsigned int j, s;
  
  if(reset){
    time = 0;
  }
  
  ic[i] = nc;
  s = 1;
  time += 1;
  
  for(j=r_slap[i]; j<r_slap[i+1]; j++){
    if (ic[J_slap[j]] == 0){
      s += dfs(J_slap[j], J_slap, r_slap, N, nc, ic, f, 0);
    }
  }
  time += 1;
  f[i] = time;
  return s;
}

/**
 * 
 * Find all the components of the given graph, using the Depth-First
 * Search algorithm
 *
 */

int components(unsigned int *J_slap, unsigned int *r_slap,
                unsigned int N, unsigned int **ic, 
                unsigned int **f, unsigned int **sizes){

  unsigned int nc, s;
  unsigned int i;

  *ic = malloc(N * sizeof(unsigned int));
  *f = malloc(N * sizeof(unsigned int));
  *sizes = malloc((N+1) * sizeof(unsigned int));
  
  for(i=0; i<N; i++){
    (*ic)[i] = 0;
    (*f)[i] = 0;
  }
  nc = 0;
  for(i=0; i<N; i++){
    while( i<N && (*ic) [i] != 0 )
      i += 1;
    if (i == N)
      break;
    nc += 1;
    s = dfs(i, J_slap, r_slap, N, nc, *ic, *f, 1);
    (*sizes)[nc] = s;
  }
  return nc;
}


int main(int argc, char *argv[]){


  FILE *filein;
  unsigned int N, K;
  unsigned int *J_slap, *r_slap;
  unsigned int *ic, *f, *sizes;
  unsigned int nc, i, j;
  char show = 0;


  if (argc < 2){
    usage(argv);
    exit(1);
  }
  if (argc >2 && !my_strcasecmp("SHOW", argv[2])){
    show = 1;
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
  
  nc = components(J_slap, r_slap, N, &ic, &f, &sizes);  
  for(i=1; i<=nc; i++){
    printf("%d", sizes[i]);
    if (show){
      printf(":");
        for(j=0; j<N; j++){
          if (ic[j] == i){
            printf(" %d", j);
          }
        }
      printf("\n");
    }
    else{
      printf("\n");
    }
  }
  free(f);
  free(ic);
  free(sizes);
  free(J_slap);
  free(r_slap);
}
