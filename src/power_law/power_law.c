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
 *  This program samples a degree sequence from a discrete power-law
 *  distribution with a given exponent.
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <errno.h>


void usage(char *argv[]){
  printf("********************************************************************\n"
         "**                                                                **\n"
         "**                   -*-    power_law    -*-                      **\n"
         "**                                                                **\n"
         "**  Sample 'N' elements from a power law degree distribution      **\n"
         "**                      P(k) ~= k^{gamma}                         **\n"
         "**  with degrees in the range [k_min, k_max], and print them      **\n"
         "**  on STDOUT.                                                    **\n"
         "**                                                                **\n"
         "**  If the obtained degree sequence is even, i.e., if the sum     **\n"
         "**  of all the sampled degrees is even, the program returns 0     **\n"
         "**  (zero), otherwise it returns 1.                               **\n"
         "**                                                                **\n"
         "********************************************************************\n"
         " This is Free Software - You can use and distribute it under \n"
         " the terms of the GNU General Public License, version 3 or later\n\n"
         " (c) Vincenzo Nicosia 2010-2017 (v.nicosia@qmul.ac.uk)\n\n"
         "********************************************************************\n\n"
         );
  printf("Usage: %s <gamma> <k_min> <k_max> <N>\n" , argv[0]);
}



void create_distr(double *v, double gamma, int k_min, int k_max){
  int n, i;
  double sum, new_sum;
  
  n = k_max-k_min + 1;
  
  sum = 0;
  for(i=0; i<n; i++){
    v[i] = pow((k_min + i), gamma);
    sum += v[i];
  }
  new_sum = 0;
  /* Now we normalize the array*/
  for(i=0; i<n; i++){
    v[i]/= sum;
    new_sum += v[i];
  }
  /* Now we compute the cumulative distribution*/
  for(i=1; i<n; i++){
    v[i] += v[i-1];
  }
}


int find_degree(double *v, int dim, double xi){
  int i;
  
  i=0;
  while(xi > v[i])
    i++;
  return i;
  
}


int main(int argc, char *argv[]){
  
  double gamma,  xi, val, q;
  unsigned int N, i, distr_num, k, k_min, k_max, K;
  double *distr;
    
  if (argc < 5){
    usage(argv);
    exit(1);
  }
  
  srand(time(NULL));

  gamma = atof(argv[1]);
  k_min = atoi(argv[2]);
  k_max = atoi(argv[3]);
  N = atoi(argv[4]);

  K = 0;
  
  distr_num = k_max - k_min + 1;
  distr = malloc(distr_num * sizeof(double));

  create_distr(distr, gamma, k_min, k_max);
  
  for(i=0; i<N;){
    xi = rand()* 1.0 / RAND_MAX;
    k = find_degree(distr, distr_num, xi);
    val = rand()*1.0/RAND_MAX;
    q = k_min + xi * distr_num;
    q = q / (floor(q) + 1);
    q = pow(q, gamma);
    if (val <= q){
      printf("%d\n", k+k_min);
      K += k+k_min;
      i++;
    }
  }
  free(distr);
  /* Return 0 if the degree sequence is even, or 1 otherwise */
  return K%2;
}
