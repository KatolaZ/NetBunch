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
 *  This program grows a network with N nodes using the linear
 *  preferential attachment model proposed by Barabasi and
 *  Albert. Each new node creates m links, and the initial (seed)
 *  network is a ring of n0>=m nodes.
 *
 *
 *  References:
 *
 *  [1] A.-L. Barabasi, R. Albert, "Emergence of scaling in random
 *  networks", Science 286, 509-512 (1999).
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void usage(char *argv[]){
  printf("********************************************************************\n"
         "**                                                                **\n"
         "**                        -*-     ba     -*-                      **\n"
         "**                                                                **\n"
         "**   Grow a scale-free network of 'N' nodes using the linear      **\n"
         "**   preferential attachment model (Barabasi-Albert).             **\n"
         "**   The initial network is a ring of 'n0' nodes, and each new    **\n"
         "**   node creates 'm' edges.                                      **\n"
         "**                                                                **\n"
         "**   The program prints on STDOUT the edge-list of the final      **\n"
         "**   graph.                                                       **\n"
         "**                                                                **\n"
         "********************************************************************\n"
         " This is Free Software - You can use and distribute it under \n"
         " the terms of the GNU General Public License, version 3 or later\n\n"
         " (c) Vincenzo Nicosia 2010-2017 (v.nicosia@qmul.ac.uk)\n\n"
         "********************************************************************\n\n"
         );
  printf("Usage: %s <N> <m> <n0>\n", argv[0]);

}



int init_network(unsigned int **S, unsigned int n0){
  
  int n;

  for(n=0; n<n0; n++){
    S[0][n] = n;
    S[1][n] = (n+1) % n0;
  }
  return n;
}

int select_neighbour(unsigned int **S, unsigned int S_num){
  
  int d;
  
  d = rand()%(S_num * 2);
  if (d < S_num)
    return S[0][d];
  else{
    return S[1][d-S_num];
  }
}

/* check if 'd' is already a neighbour of 'i' */

int already_neighbour(unsigned int **S, unsigned int S_num, unsigned int j, unsigned int d){

   int i;

   for(i=S_num; i< S_num + j; i ++){
     if (S[1][i] == d)
       return 1;
   }
   return 0;
}

unsigned int grow_ba_network(unsigned int **S, unsigned int N,
                             unsigned int m, unsigned int n0, unsigned int S_num){
  
  int i, j;
  int n, d;
  
  for(i=0; i<N-n0; i++){
    /* Let's add a new node */
    n = n0 + i; /* This is the id of the new node */
    for(j=0; j<m; j++){
      S[0][S_num+j] = n;
      d = select_neighbour(S, S_num);
      while(already_neighbour(S, S_num, j, d)){
        d = select_neighbour(S, S_num);
      }
      S[1][S_num + j] = d;
    }
    S_num += m;
  }
  return S_num;
}



int main(int argc, char *argv[]){
  
  unsigned int **S;
  unsigned int S_num, S_size, i;
  int m, n0, N;
  
  if (argc < 4){
    usage(argv);
    exit(1);
  }

  srand(time(NULL));

  N = atoi(argv[1]);
  m = atoi(argv[2]);
  n0 = atoi(argv[3]);
  S_size = (N+n0) * m ;

  if (N < 1){
    fprintf(stderr, "N must be positive\n");
    exit(1);
  }
  if(m > n0){
    fprintf(stderr, "n0 cannot be smaller than m\n");
    exit(1);

  }
  if (n0<1){
    fprintf(stderr, "n0 must be positive\n");
    exit(1);
  }
  
  if (m < 1){
    fprintf(stderr, "m must be positive\n");
    exit(1);
  }
  
  S = malloc(2 * sizeof(unsigned int*));
  S[0] = malloc(S_size * sizeof(unsigned int));
  S[1] = malloc(S_size * sizeof(unsigned int));
  
  S_num = init_network(S, n0);
  S_num = grow_ba_network(S, N, m, n0, S_num);
  for(i=0; i<S_num; i ++){
    printf("%d %d\n",S[0][i], S[1][i]);
  }
  free(S[0]);
  free(S[1]);
  free(S);
}
