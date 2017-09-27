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
 *  This program creates a network using the Watts-Strogatz
 *  small-world network model.
 *
 *  References: 
 *
 *  D. J. Watts and S. H. Strogatz. "Collective dynamics of
 *  'small-world' networks".  Nature 393 (1998), 440–442.
 *  
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
         "**                        -*-     ws     -*-                      **\n"
         "**                                                                **\n"
         "**    Create a graph with 'N' nodes, using the Watts-Strogatz     **\n"
         "**    small-world network model. The algorithm starts from a      **\n"
         "**    circle graph with 'N' nodes and m*N edges, i.e. by putting  **\n"
         "**    the 'N' nodes around a circle, so that each node is         **\n"
         "**    connected to the 'm' closest nodes in the circle on each    **\n"
         "**    direction. Then, each edge is rewired at random with        **\n"
         "**    probability equal to 'p'.                                   **\n"
         "**                                                                **\n"
         "**    The output is an edge list in the format:                   **\n"
         "**                                                                **\n"
         "**                            I_1 J_1                             **\n"
         "**                            I_2 J_2                             **\n"
         "**                            I_3 J_3                             **\n"
         "**                            ... ...                             **\n"
         "**                            I_K J_K                             **\n"
         "**                                                                **\n"
         "**    If 'SHOW' is specified as a fourth parameter, the program   **\n"
         "**    prints on STDERR the number of edges that were actually     **\n"
         "**    rewired.                                                    **\n"
         "**                                                                **\n"
         "********************************************************************\n"
         " This is Free Software - You can use and distribute it under \n"
         " the terms of the GNU General Public License, version 3 or later\n\n"
         " (c) Vincenzo Nicosia 2010-2017 (v.nicosia@qmul.ac.uk)\n\n"
         "********************************************************************\n\n"
         );
  printf("Usage: %s <N> <m> <p> [SHOW]\n\n" , argv[0]);
}



/**
 *
 * This function checks if j is a neighbour of i, looking into the 
 * m positions of J starting at i*m 
 * 
 * In practice, the assumption is that J contains the m neighbours of
 * node 0 in the first m positions, then the m neighbours of node 1,
 * and to forth
 *
 */

int __ws_is_neigh(unsigned int i, unsigned int j, unsigned int *J, unsigned int m){
  
  int k;

  for(k = i*m; k< (i+1)*m; k++){
    if (J[k] == j)
      return 1;
  }
  return 0;
}


int create_circle(unsigned int N, unsigned int m, unsigned int **J){

  int K;
  int i, j, l;
  
  K = N *m;
  
  *J = malloc(K * sizeof(unsigned int));

  K = 0;
  
  for(i=0; i<N; i++){
    for(j=0; j < m; j++){
      l = (i + j + 1) % N;
      (*J)[K] = l;
      K += 1;
    }
  }
  return K;
}



int ws(unsigned int *J, unsigned int N, unsigned int m, double p){

  unsigned int i, j, l, l1, num_rewire;
  
  double xi;

  num_rewire = 0;
  
  for(i=0; i<N; i++){
    for(j=0; j<m; j++){
      l = (i+j) % N;
      xi = 1.0 * rand() / RAND_MAX;
      if (xi < p){
        l1 = (int)(rand() % N);
        if( (l1 != i) &&
            (l1 != l) &&
            !(__ws_is_neigh(i, l1, J, m))){
          /* replace (i,l) with (i,l1) */
          J[m * i + j] = l1;
          num_rewire += 1;
        }
        else{
          /* do nothing */
        }
      }
    }
  }
  return num_rewire;
}


void dump_edges(unsigned int *J, unsigned int N, unsigned int m){
  
  int i, j;
  
  for(i=0; i<N; i++){
    for(j=0; j<m; j++){
      printf("%d %d\n", i, J[(m*i)+j]);
    }
  }
}


int main(int argc, char *argv[]){

  int N, m, num_rewire;
  unsigned int  *J;
  double p;

  if(argc < 4){
    usage(argv);
    exit(1);
  }

  J = NULL;
  
  N = atoi(argv[1]);
  m = atoi(argv[2]);
  p = atof(argv[3]);

  create_circle(N, m, &J);
  
  
  srand(time(NULL));
  
  num_rewire = ws(J, N, m, p);
  if (argc > 4 && !my_strcasecmp("SHOW", argv[4])){
    fprintf(stderr, "Rewired edges: %d\n", num_rewire);
  }
  dump_edges(J, N, m);
  free(J);
}
