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
 * This program implements the Dorogovtsev-Samukhin-Mendes preferential
 * attachment, where the attachment probability is:
 *
 * \Pi_{i->j} \propto k_j + a
 *
 * Here a > -m is a tunable parameter. The resulting network has a
 * powerlaw degree distribution with exponent:
 *
 * \gamma = 3 + a/m
 *
 *  References:
 *  
 * [1] S. N. Dorogovtsev, J. F. F. Mendes, A. N. Samukhin. "Structure
 *     of Growing Networks with Preferential Linking".
 *     Phys. Rev. Lett. 85 (2000), 4633-4636. 
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "cum_distr.h"


void usage(char *argv[]){
  printf("********************************************************************\n"
         "**                                                                **\n"
         "**                        -*-    dms    -*-                       **\n"
         "**                                                                **\n"
         "**   Grow a scale-free network of 'N' nodes using the modified    **\n"
         "**   linear preferential attachment model proposed by             **\n"
         "**   Dorogovtsev-Mendes-Samukhin.                                 **\n"
         "**                                                                **\n"
         "**   The initial network is a clique of 'n0' nodes, and each new  **\n"
         "**   node creates 'm' edges. The attachment probability is of     **\n"
         "**   the form:                                                    **\n"
         "**                                                                **\n"
         "**            P(i->j) ~ k_j + a                                   **\n"
         "**                                                                **\n"
         "**   where a > -m is the fourth parameter. The resulting          **\n"
         "**   network will have a power-law degree distribution with       **\n"
         "**   exponent                                                     **\n"
         "**                                                                **\n"
         "**       gamma = 3 + a/m                                          **\n"
         "**                                                                **\n"
         "**   The program prints on STDOUT the edge-list of the final      **\n"
         "**   graph.                                                       **\n"
         "**                                                                **\n"
         "********************************************************************\n"
         " This is Free Software - You can use and distribute it under \n"
         " the terms of the GNU General Public License, version 3 or later\n\n"
         " (c) Vincenzo Nicosia 2009-2017 (v.nicosia@qmul.ac.uk)\n\n"
         "********************************************************************\n\n"
         );
  printf("Usage: %s <N> <m> <n0> <a>\n", argv[0]);

}



int init_network(unsigned int *I, unsigned int *J, int n0, 
                 double a, cum_distr_t *d){
  
  unsigned int n, i,  S_num;
  
  S_num = 0;
  for(n=0; n<n0; n++){
    for(i=n+1; i<n0; i++){
      I[S_num] = n;
      J[S_num] = i % n0;
      S_num += 1;
    }
    cum_distr_add(d, n, n0+a);
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



int dms(unsigned int *I, unsigned int *J, unsigned int N, 
         unsigned int m, unsigned int n0,  double a){
  
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
    cum_distr_add(d, n, m + a);
    for (j=0; j<m; j++){
      cum_distr_add(d, J[S_num + j], 1);
    }
    S_num += m;
    n += 1;
  }
  cum_distr_destroy(d);
  return S_num;
}



int main(int argc, char *argv[]){
  
  int N, m, n0, K, i;
  unsigned int *I, *J;
  double a;
  
  if (argc < 5){
    usage(argv);
    exit(1);
  }
  
  N = atoi(argv[1]);
  m = atoi(argv[2]);
  n0 = atoi(argv[3]);
  a = atof(argv[4]);

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

  if (a < -m){
    fprintf(stderr, "a must be larger than -m\n");
    exit(1);
  }
  
  
  I = malloc(N * m * sizeof(unsigned int));
  J = malloc(N * m * sizeof(unsigned int));
  
  K = dms(I, J, N, m, n0, a);
  
  for(i=0; i<K; i++){
    printf("%d %d\n", J[i], I[i]);
  }
  free(I);
  free(J);
}
