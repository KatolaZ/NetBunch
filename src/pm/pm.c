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
 *  This program computes the leading eigenvector and the leading
 *  eigenvalue of a given graph, using the power method. The value of
 *  the leading eigenvalue is printed on the standard output, while
 *  the associated eigenvector is reported.
 *
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "utils.h"

#define MAX(x,y) ((x)>(y)? (x) : (y))


void usage(char *argv[]){
  printf("********************************************************************\n"
         "**                                                                **\n"
         "**                        -*-     pm     -*-                      **\n"
         "**                                                                **\n"
         "**   Compute the leading eigenvalue and the leading eigenvector   **\n"
         "**   of a graph, with a relative error smaller than 'eps' using   **\n"
         "**   the power method (Rayleigh iteration).                       **\n"
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
         "**   'is_dir' should be set either to 0 (zero) if the graph is    **\n"
         "**   undirected, or to 1 (one) if the graph is directed.          **\n"
         "**                                                                **\n"
         "**   The value of the leading eigenvalue is printed on the        **\n"
         "**   standard output (STDOUT) while the associated eigenvector    **\n"
         "**   is printed on the standard error (STDERR).                   **\n"
         "**                                                                **\n"
         "**                                                                **\n"
         "********************************************************************\n"
         " This is Free Software - You can use and distribute it under \n"
         " the terms of the GNU General Public License, version 3 or later\n\n"
         " Please visit http://www.complex-networks.net for more information\n\n"
         " (c) Vincenzo Nicosia 2010-2017 (v.nicosia@qmul.ac.uk)\n"
         "********************************************************************\n\n"
         );
  
  printf("Usage: %s <graph_in> <is_dir> <eps>\n", argv[0]);
}


/* Product of a matrix by a vector */

void matrix_vector_product(unsigned int *I, unsigned int *J, unsigned int K,
                           double *src, double *dst, unsigned int N){
  
  int i;

  for(i=0; i<N; i ++){
    dst[i] = 0;
  }
  
  for (i=0; i<K; i++){
    dst[I[i]] += src[J[i]];
  }
  return;
}

/* product between two row vectors (v1 * v2') */

double vector_vector_product(double *v1, double *v2, unsigned int N){
  
  int i;
  double sum = 0;

  for(i=0; i<N; i ++){
    sum += v1[i] * v2[i];
  }
  return sum;
}

/* compute the 2-norm of a vector */

double vector_norm(double *v,unsigned int N){

  double norm = 0.0;
  int i;

  for(i=0; i<N; i++){
    norm += v[i] * v[i];
  }
  norm = sqrt(norm);
  return norm;
}


double compute_relative_error(double *x_new, double *x_old, double lambda, unsigned int N){
  
  double val, num, den;
  int i;
  
  num = den = 0.0;
  for (i=0; i<N; i++){
    val = x_new[i] - lambda * x_old [i];
    num += val * val;
    den += x_new[i] * x_new[i];
  }
  return sqrt(num / den);
}


int main(int argc, char *argv[]){
  
  unsigned int *I, *J;
  unsigned int N, K;
  double *x1, *x2, *tmp;
  double norm, lambda, err, eps;
  int i, is_dir;

  FILE *filein, *fileout;
  
  if(argc < 4){
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
  is_dir = atoi(argv[2]);
  eps = fabs(atof(argv[3]));
  lambda = 0.0;

  
  K = read_ij(filein, &I, &J);
  if (! is_dir){
    K = 2*K;
    I = realloc(I, K * sizeof(unsigned int));
    J = realloc(J, K * sizeof(unsigned int));
    for (i=K/2; i<K; i++){
      I[i] = J[i-K/2];
      J[i] = I[i-K/2];
    }

  }
  N = 1 + MAX(find_max(I, K), find_max(J, K));

  fclose(filein);
  
  x1 = malloc(N * sizeof(double));
  x2 = malloc(N * sizeof(double));
  
  for(i=0; i<N; i++){
    x1[i] = 1;
    x2[i] = 0;
  }
  
  /* The following cycle is the actual implementation of the power
     method (Rayleigh iteration) */ 
  err = 100*eps;
  
  while (err > eps) {
    norm = vector_norm(x1, N);
    for(i=0; i<N; i ++){
      x1[i] /= norm;
    }
    matrix_vector_product(I, J, K, x1, x2, N);
    lambda = vector_vector_product(x2, x1, N);
    /* compute the relative error */
    err = compute_relative_error(x2, x1, lambda, N);
    tmp = x1;
    x1 = x2;
    x2 = tmp;
  }
  
  fileout = stderr;
  norm = vector_norm(x1, N);

  for(i=0; i<N; i++){
    fprintf(fileout, "%d %g\n", i, x1[i]/norm);
  }
  printf("%2.15g\n", lambda);
  free(I);
  free(J);
  free(x1);
  free(x2);
}
