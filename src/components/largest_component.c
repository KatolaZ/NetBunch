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
 *  Compute the largest connected component of a graph (i.e., the
 *  connected component that contains the largest number of nodes).
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
         "**                  -*- largest_component -*-                     **\n"
         "**                                                                **\n"
         "**   Find the largest connected component of 'graph_in'.          **\n"
         "**                                                                **\n"
         "**   The input file 'graph_in' is an edge-list:                   **\n"
         "**                                                                **\n"
         "**                            I_1 J_1                             **\n"
         "**                            I_2 J_2                             **\n"
         "**                            I_3 J_3                             **\n"
         "**                            ... ...                             **\n"
         "**                            I_K J_K                             **\n"
         "**                                                                **\n"
         "**    If 'graph_in' is equal to '-' (dash), read the file from    **\n" 
         "**    the standard input (STDIN).                                 **\n"
         "**                                                                **\n"
         "**    The program prints on STDOUT the edge list of the largest   **\n"
         "**    connected component of 'graph_in'.                          **\n"
         "**                                                                **\n"
         "**                                                                **\n"
         "********************************************************************\n"
         " This is Free Software - You can use and distribute it under \n"
         " the terms of the GNU General Public License, version 3 or later\n\n"
         " Please visit http://www.complex-networks.net for more information\n\n"
         " (c) Vincenzo Nicosia 2009-2017 (v.nicosia@qmul.ac.uk)\n"
         "********************************************************************\n\n"
         );
  
  printf("Usage: %s <graph_in>\n", argv[0]);
}



/**
 *
 * Depth-First search on the node i.... 
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
  unsigned int nc, i, j, max_c;
  
  
  if (argc < 2){
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
  nc = components(J_slap, r_slap, N, &ic, &f, &sizes);
  
  max_c = 1;
  for(i=2; i<=nc; i++){
    if (sizes[i] > sizes[max_c])
      max_c = i;
  }
  for(i=0; i<N; i++){
    if(ic[i] == max_c){
      for(j=r_slap[i]; j<r_slap[i+1]; j++){
        if (J_slap[j] > i)
          fprintf(stdout, "%d %d\n", i, J_slap[j]);
      }
    }
  }
  free(J_slap); 
  free(r_slap);
  free(ic);
  free(f);
  free(sizes);
}
