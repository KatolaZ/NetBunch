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
 *  This program fits a set of values provided as input with a
 *  power-law function using the Maximum-Likelihood Estimator (MLE).
 *
 *  References: 
 * 
 * [1] A. Clauset, C. R. Shalizi, and M. E. J. Newman. "Power-law
 *     distributions in empirical data". SIAM Rev. 51, (2007),
 *     661-703.  
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "utils.h"

void usage(char *argv[]){

  printf("********************************************************************\n"
         "**                                                                **\n"
         "**                        -*-   fitmle   -*-                      **\n"
         "**                                                                **\n"
         "**    Fit a set of values with a power-law function using the     **\n"
         "**    Maximum-Likelihood Estimator (MLE). The program implements  **\n"
         "**    the MLE for both continuous and discrete values, and        **\n"
         "**    selects the appropriate one automatically.                  **\n"
         "**                                                                **\n"
         "**    The input file 'data_in' contains one value on each row.    **\n"
         "**    If 'data_in' is '-' (dash), read the values from the        **\n"
         "**    standard input (STDIN).                                     **\n"
         "**                                                                **\n"
         "**    The program prints on output a single row, in the format:   **\n"
         "**                                                                **\n"
         "**       gamma x_min ks                                           **\n"
         "**                                                                **\n"
         "**    where 'gamma' is the exponent of the best fit, 'x_min' is   **\n"
         "**    the smallest of the input values after which the power-law  **\n"
         "**    hypothesis hold, and 'ks' is the value of the KS statistics **\n"
         "**    for the best fit.                                           **\n"
         "**                                                                **\n"
         "**    The second (optional) parameter 'tol' sets the tolerance    **\n"
         "**    on the acceptable statistical error of the fit (set to      **\n"
         "**    0.1 if no value is provided).                               **\n"
         "**                                                                **\n"
         "**    If the third parameter is 'TEST', the program uses the      **\n"
         "**    the Kolmogorov-Smirnov test on a series of bootstrapped     **\n"
         "**    power-law sequences to estimate the p-value of the fit, and **\n"
         "**    the output is in the format:                                **\n"
         "**                                                                **\n"
         "**       gamma x_min ks p_value                                   **\n"
         "**                                                                **\n"
         "**    Higher values of 'p_value' correspond to better fits.       **\n"
         "**                                                                **\n"
         "**    If 'num_test' is provided, use that number of bootstrapped  **\n"
         "**    sequences to compute the p-value. Otherwise, use 100        **\n"
         "**    sequences.                                                  **\n"
         "**                                                                **\n"
         "********************************************************************\n"
         " This is Free Software - You can use and distribute it under \n"
         " the terms of the GNU General Public License, version 3 or later\n\n"
         " (c) Vincenzo Nicosia 2010-2017 (v.nicosia@qmul.ac.uk)\n\n"
         "********************************************************************\n\n"
         );
  printf("Usage: %s <data_in> [<tol> [ TEST [<num_test>]]]\n", argv[0]);
}


void load_data(char *fname, double **data, unsigned int *N, char sort){
  
  FILE *fin;
  char error_str[256];
  char buff[256];
  char *ptr;
  double x;
  unsigned int size;
  
  size = 10;

  *data = malloc(size * sizeof(double));

  *N=0;
  if (!strcmp(fname, "-")){
    fin = stdin;
  }
  else{
    fin = fopen(fname, "r");
  }
  if (!fin){
    sprintf(error_str, "Error opening file %s", fname);
    perror(error_str);
    exit(3);
  }
  
  while(fgets(buff, 256, fin)){
    ptr = strtok(buff, " ");
    if (ptr[0] == '#')
      continue;
    x = atof(ptr);
    if (*N == size){
      size += 10;
      *data = realloc(*data, size*sizeof(double));
    }
    (*data)[*N] = x;
    *N += 1;
  }
  *data = realloc(*data, (*N)*sizeof(double));
  if (sort){
    qsort(*data, *N, sizeof(double), compare_double);
  }
  fclose(fin);
  return;
}

/** 
 *  find the first position at which the element x appears in the
 *  "data" array using binary search (the array is sorted in
 *  increasing order of x)
 */
int find_pos_x(double x,double *data, unsigned int N){
  int H, L, M;

  L = 0;
  H = N-1;
  
  while(L<=H){
    M = (H + L)/2;
    if (x == data[M]){
      while(M >=0 && x == data[M]) M--;
      if (M==-1){
        return 0;
      }
      return M+1; /* we return the index of the first appearance of element x*/
    }
    else if (x < data[M]){
      H = M-1;
    }
    else if (x > data[M]){
      L = M+1;
    }
  }
  return -1; /* the value is not present */
}

double fit_alpha(double *data, unsigned int N, double xmin, unsigned int idx){
  
  double alpha = 0;
  int i;
  
  for(i = idx; i < N; i++){
    alpha += log(data[i]*1.0/(xmin-0.5));
  }
  alpha = 1 + (N - idx) * 1.0 / alpha;
  return alpha;
}


double fit_alpha_continuous(double *data, unsigned int N, double xmin, unsigned int idx){
  
  double alpha = 0;
  int i;
  
  for(i = idx; i < N; i++){
    alpha += log(data[i]*1.0/(xmin));
  }
  alpha = 1 + (N - idx) * 1.0 / alpha;
  return alpha;
}


/*
 *
 * Kolmogorov-Smirnov test
 *
 */

double KS (double *data, unsigned int N, double alpha, int idx){
  
  double c_data, c_theo, val;
  double xmin, x;
  double dist, max_dist = -1;
  int i;

  c_data = c_theo = 0.0;
  xmin = data[idx];

  for (i=idx; i<N;){
    x = data[i];
    while(i<N && data[i] == x){
      val = xmin * 1.0 / data[i];
      c_theo = 1.0 -  pow(val, alpha-1.0);
      dist = fabs(c_theo - c_data);

      if (dist > max_dist){
        max_dist = dist;
      }
      i++;
    }
    c_data = 1.0 * (i- idx)/(N-idx);
  }
  return max_dist;
}

void dump_data_double(double *v, unsigned int N){
  int i;

  if (N < 1){
    return;
  }
  printf("%g",v[0]);
  for(i=1; i<N; i++){
    printf(" %g",v[i]);
  }
  printf("\n");
}


/* This is the same as best_fit, but for continuous-valued time-series */
void best_fit_continuous(double *data, unsigned int N, double *alpha, double *xmin, double tol){
  
  double min_x, max_x, x;
  double cur_alpha, best_alpha, best_x, ks_test, best_ks;
  int idx,i;
  

 

  qsort(data, N, sizeof(double), compare_double);

  min_x = data[0];
  best_x = data[0];
  max_x = data[N-1];
  best_ks = 10000000;
  cur_alpha = 0.0;
  best_alpha = 0.0;
  idx = 0;
  for(x=min_x, i=0; x<=max_x && (cur_alpha)/sqrt(1.0 * (N-idx))< tol; ){
    idx = find_pos_x(x, data, N);
    if (idx <0){ 
      i ++;
      continue;
    }
    cur_alpha = fit_alpha_continuous(data, N, x, idx);
    ks_test = KS(data, N, cur_alpha, idx);
    if(ks_test < best_ks){
      best_ks = ks_test;
      best_alpha=cur_alpha;
      best_x = data[idx];
    }
    while(data[i] == x && i <N){
      i ++;
    }
    x = data[i];
  }

  *alpha = best_alpha;
  *xmin = best_x;
  return;
}

/* Fit a discrete power-law */

void best_fit(double *data, unsigned int N, double *alpha, double *xmin, double tol){
  
  double min_x, max_x, x;
  double cur_alpha, best_alpha, best_x, ks_test, best_ks;
  int idx, i;
  

  
  qsort(data, N, sizeof(double), compare_double);
  min_x = data[0];
  best_x = data[0];
  max_x = data[N-1];
  best_ks = 10000000;
  cur_alpha = 0.0;
  best_alpha = 0.0;
  idx = 0;
  for(x=min_x, i = 0; x<=max_x && (cur_alpha)/sqrt(1.0 * (N-idx))< tol;){
    idx = find_pos_x(x, data, N);

    if (idx<0) {
      i ++;
      continue;
    }
    cur_alpha = fit_alpha(data, N, x, idx);

    ks_test = KS(data, N, cur_alpha, idx);
    if(ks_test < best_ks){
      best_ks = ks_test;
      best_alpha=cur_alpha;
      best_x = data[idx];
    }
    while(data[i] == x && i<N){
      i ++;
    }
    x = data[i];
  }

  *alpha = best_alpha;
  *xmin = best_x;
  return;
}

void sample_powerlaw(double *data, unsigned int N, double alpha, double xmin,double *v){
  int i, last_idx, r;
  double val;
  
  i=0;
  last_idx = 0;
  while(data[i] < xmin){
    last_idx ++;
    i++;
  }
  
  i=0;
  while(i <last_idx){
    r = rand() % (last_idx + 1);
    v[i] = data[r];
    i++;
  }
  
  for(; i<N; i++){
    val = 1.0 * rand()/RAND_MAX;
    v[i] = floor(xmin * pow(1.0-val, -1.0/(alpha-1)));

  }
  
}


double test_powerlaw(double *data, unsigned int N, double alpha, double xmin, 
                     double ks, int num_test, 
                     void (*f)(double *, unsigned int , double *, double *, double)){
  
  int num = 0, i, idx;
  double *v, cur_alpha, cur_xmin, cur_ks;


  v = malloc(N * sizeof(double));

  for(i=0; i<num_test; i++){
    sample_powerlaw(data, N, alpha, xmin, v);
    qsort(v, N, sizeof(double), compare_double);
    f(v, N, &cur_alpha, &cur_xmin, 0.1); 
    idx = find_pos_x(cur_xmin, v, N);
    cur_ks = KS(v, N, cur_alpha, idx);
    if (cur_ks > ks){
      num += 1;
    }
  }
  free(v);
  return 1.0 * num / num_test;
}

/* We assume that the data set has already been sorted in ascending
   order */
int is_continuous(double *data, int N){
  int i;

  for (i=0; i<N; i++){
    if (data[i] - (double)(int)data[i] != 0.0)
      return 1;
  }
  /* return 1 only if we need a real-valued fit....*/
  return 0;
}

/* we assume that the data set has already been sorted in ascending
   order */
double renormalise(double *data, unsigned int N){
  
  int i;
  double scaling = 1.0;
  
  if (data[0] < 1.0 ){
    scaling = 1.0/data[0];
    for (i=0; i<N; i++){
      data[i] *= scaling;
    }
  }
  return scaling;
}


int main(int argc, char *argv[]){
  
  double *data=NULL;
  unsigned int N, i;
  double alpha, xmin, ks, tol, p_value, scaling_fact;
  char test = 0;
  int num_test;
  
  void  (*fit_func)(double *, unsigned int , double *, double *, double);
  

  if (argc < 2){
    usage(argv);
    exit(1);
  }
  
  if (argc > 2)
    tol = atof(argv[2]);
  else
    tol = 0.1;

  if (argc > 3 && !my_strcasecmp(argv[3], "TEST")){
    test = 1;
  }
  
  if(argc > 4){
    num_test = atoi(argv[4]);
  }
  else
    num_test = 100;
  
  srand(time(NULL));
  

  
  load_data(argv[1], &data, &N, 1);

  if(is_continuous(data, N)){
    fprintf(stderr, "Using continuous fit\n");
    fit_func = best_fit_continuous;
  }
  else{
    fprintf(stderr, "Using discrete fit\n");
    fit_func = best_fit;
  }

  scaling_fact = 1.0;

  fit_func(data, N, &alpha, &xmin, tol);
  i = find_pos_x(xmin, data, N);
  ks = KS(data, N, alpha, i);
  if (test){
    p_value = test_powerlaw(data, N, alpha, xmin, ks, num_test, fit_func);
    printf("%g %g %g %g\n", alpha,  xmin/scaling_fact, ks, p_value);
  }
  else{
    printf("%g %g %g\n", alpha, xmin / scaling_fact, ks);
  }
  free(data);
}
