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
 *  This program implements the fitness model proposed by Bianconi and
 *  Barabasi, where the attachment probability is:
 *
 *     \Pi_{i->j} \propto a_j * k_j 
 *
 *  where a_j is the actractiveness of node j. 
 *
 *
 *  References:
 *  
 * [1] G. Bianconi, A.-L. Barabasi, " Competition and multiscaling in
 *     evolving networks". EPL-Europhys. Lett. 54 (2001), 436.
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "utils.h"
#include "cum_distr.h"


void usage(char *argv[]){
  printf("********************************************************************\n"
         "**                                                                **\n"
         "**                        -*- bb_fitness -*-                      **\n"
         "**                                                                **\n"
         "**   Grow a network of 'N' nodes using the fitness model proposed **\n"
         "**   by Bianconi and Barabasi.                                    **\n"
         "**                                                                **\n"
         "**   The initial network is a clique of 'n0' nodes, and each new  **\n"
         "**   node creates 'm' edges. The attachment probability is of     **\n"
         "**   the form:                                                    **\n"
         "**                                                                **\n"
         "**            P(i->j) ~ a_j * k_j                                 **\n"
         "**                                                                **\n"
         "**   where a_j is the attractiveness of node j. The values of     **\n"
         "**   node attractiveness are sampled uniformly at random in       **\n"
         "**   [0,1].                                                       **\n"
         "**                                                                **\n"
         "**   The program prints on STDOUT the edge-list of the final      **\n"
         "**   graph.                                                       **\n"
         "**                                                                **\n"
         "**   If 'FIT' is specified as a fourth parameter, the values      **\n"
         "**   of node attractiveness are printed on STDERR.                **\n"
         "**                                                                **\n"
         "********************************************************************\n"
         " This is Free Software - You can use and distribute it under \n"
         " the terms of the GNU General Public License, version 3 or later\n\n"
         " (c) Vincenzo Nicosia 2009-2017 (v.nicosia@qmul.ac.uk)\n\n"
         "********************************************************************\n\n"
         );
  printf("Usage: %s <N> <m> <n0> [SHOW]\n", argv[0]);
}




int init_network(unsigned int *I, unsigned int *J, int n0, 
                 double *a, cum_distr_t *d){
  
  unsigned int n, i,  S_num;
  
  
  S_num = 0;
  for(n=0; n<n0; n++){
    for(i=n+1; i<n0; i++){
      I[S_num] = n;
      J[S_num] = i % n0;
      S_num += 1;
    }
    cum_distr_add(d, n, n0*a[n]);
  }
  return S_num;
}

int already_neighbour(unsigned int *J, int S_num, int j, int dest){

   int i;

   for(i=S_num; i< S_num + j; i ++){
     if (J[i] == dest)
       return 1;
   }
   return 0;
}



int bb_fitness(unsigned int *I, unsigned int *J, unsigned int N, 
               unsigned int m, unsigned int n0,  double* a){
  
  cum_distr_t *d = NULL;
  unsigned int n, j, dest, S_num;
  
  d = cum_distr_init(N * m);
  
  S_num = init_network(I, J, n0, a, d);
  
  
  n = n0;
  while (n<N){
    for(j=0; j<m; j++){
      I[S_num+j] = n;
      dest = cum_distr_sample(d);
      while(already_neighbour(J, S_num, j, dest)){
        dest = cum_distr_sample(d);
      }
      J[S_num + j] = dest;
    }
    cum_distr_add(d, n, m*a[n]);
    for (j=0; j<m; j++){
      cum_distr_add(d, J[S_num + j], a[ J[S_num + j] ]);
    }
    S_num += m;
    n += 1;
  }
  cum_distr_destroy(d);
  return S_num;
}

void dump_graph(unsigned int *I, unsigned int *J, unsigned int K){
  
  unsigned int i;

  for(i=0; i<K; i++){
    printf("%d %d\n", J[i], I[i]);
  }
  
}


void init_fitness_uniform(double *a, unsigned int N){
  
  unsigned int i;

  for(i=0; i<N; i++){
    a[i] = 1.0 * rand() / RAND_MAX;
  }
  
}


void dump_fitness(double *a, unsigned int N){
  
  int i;

  for(i=0; i<N; i++){
    fprintf(stderr, "%g\n", a[i]);
  }
}


int main(int argc, char *argv[]){
  
  int N, m, n0, K;
  unsigned int *I, *J;
  double *a;
  
  if (argc < 4){
    usage(argv);
    exit(1);
  }
  
  N = atoi(argv[1]);
  m = atoi(argv[2]);
  n0 = atoi(argv[3]);
  
  a = malloc(N * sizeof(double));
  
  srand(time(NULL));

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
  
  I = malloc(N * m * sizeof(unsigned int));
  J = malloc(N * m * sizeof(unsigned int));
  a = malloc(N * sizeof(double));

  init_fitness_uniform(a, N);
  
  K = bb_fitness(I, J, N, m, n0, a);
  
  dump_graph(I, J, K);
  if (argc > 4 && !my_strcasecmp(argv[4], "SHOW")){
    dump_fitness(a, N);
  }
  free(a);
  free(I);
  free(J);
  
}
