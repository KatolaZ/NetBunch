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
 *  This file contains general utilities to handle input/output of
 *  graphs, convert between different sparse matrix representations,
 *  and other ancillary functions. It is linked against most of the
 *  programs in NetBunch.
 *
 */



#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"



/* Read a degree distribution -- OBSOLETE */
int read_deg_distr(FILE *filein, unsigned int **degs, unsigned int **Nk, double **p){
  
  int n_degs = 0;
  int size = 10;
  char buff[256];
  int k_i, num_i;
  double p_i;
  char *ptr;


  *degs = realloc(*degs, size*sizeof(unsigned int));
  *Nk = realloc(*Nk, size*sizeof(unsigned int));
  *p = realloc(*p, size*sizeof(double));
  

  while(fgets(buff, 256, filein)){
    ptr = strtok(buff, " ");
    VALID_PTR_OR_EXIT(ptr, 7);
    if (ptr[0] == '#')
      continue;
    k_i = atoi(ptr);
    ptr = strtok(NULL, " " );
    VALID_PTR_OR_EXIT(ptr, 7);
    num_i = atoi(ptr);
    ptr = strtok(NULL, " \n");
    VALID_PTR_OR_EXIT(ptr, 7);
    p_i = atof(ptr);
    if (n_degs == size){
      size += 10;
      *degs = realloc(*degs, size*sizeof(unsigned int));
      *Nk = realloc(*Nk, size*sizeof(unsigned int));
      *p = realloc(*p, size*sizeof(double));
    }
    (*degs)[n_degs] = k_i;
    (*Nk)[n_degs] = num_i;
    (*p)[n_degs] = p_i;
    n_degs += 1;
  }
  if (n_degs > 0){
    *degs = realloc(*degs, n_degs*sizeof(unsigned int));
    *Nk = realloc(*Nk, n_degs*sizeof(unsigned int));
    *p = realloc(*p, n_degs*sizeof(double));
  }
  return n_degs;
}


int read_deg_seq(FILE *filein, unsigned int **nodes){

  int size, N, k;
  char buff[256];
  char *ptr;

  N = 0;
  size = 10;
  
  *nodes = (unsigned int*)malloc(size * sizeof(unsigned int));

  while(fgets(buff, 256, filein)){
    ptr = strtok(buff, " ");
    VALID_PTR_OR_EXIT(ptr, 7);
    if (ptr[0] == '#')
      continue;
    k = atoi(ptr);
    
    if (N == size){
      size += 10;
      *nodes = realloc(*nodes, size*sizeof(unsigned int));
    }
    (*nodes)[N] = k;
    N += 1;
  }
  if (N > 0)
    *nodes = realloc(*nodes, N * sizeof(unsigned int));
  return N;
}

int read_stubs(FILE *filein, unsigned int **S){
  
  int size, K;
  char buff[256];
  char *ptr;
  
  K=0;
  size = 20;
  *S = malloc(size * sizeof(unsigned int));
  
  while(fgets(buff, 256, filein)){
    if (K == size){
      size += 20;
      *S = realloc(*S, size*sizeof(unsigned int));
    }
    ptr = strtok(buff, " "); /* read the first node */
    VALID_PTR_OR_EXIT(ptr, 7);
    (*S)[K++] = atoi(ptr);
    ptr = strtok(NULL, " "); /* read the second node */
    VALID_PTR_OR_EXIT(ptr, 7);
    (*S)[K++] = atoi(ptr);
  }
  if (K > 0)
    *S = realloc(*S, K * sizeof(unsigned int));
  return K;
}

/*
 * Read a file in ij format 
 */
int read_ij(FILE *filein, unsigned int **I, unsigned int **J){
  
  unsigned int size, K;
  char buff[256];
  char *ptr;
  
  size = 20;
  K = 0;
  
  *I = malloc(size * sizeof(unsigned int));
  *J = malloc(size * sizeof(unsigned int));
  while(fgets(buff, 256, filein)){
    if (buff[0] == '#')
      continue;
    if (K == size){
      size += 20;
      *I = realloc(*I, size*sizeof(unsigned int));
      *J = realloc(*J, size*sizeof(unsigned int));
    }
    ptr = strtok(buff, " "); /* read the first node */
    VALID_PTR_OR_EXIT(ptr, 7);
    (*I)[K] = atoi(ptr);
    ptr = strtok(NULL, " "); /* read the second node */
    VALID_PTR_OR_EXIT(ptr, 7);
    (*J)[K] = atoi(ptr);
    K += 1;
  }
  if (K > 0){
    *I = realloc(*I, K * sizeof(unsigned int));
    *J = realloc(*J, K * sizeof(unsigned int));
  }
  return K;
}


/*
 * Read a file in ij format -- weighted graphs -- if the input file is
 * unweighted (i.e., no weights are provided), all the edges are
 * assumed to have weight equal to 1.0
 */
int read_ij_w(FILE *filein, unsigned int **I, unsigned int **J, 
              double **W){
  
  unsigned int size, K;
  char buff[256];
  char *ptr;
  
  size = 20;
  K = 0;
  
  *I = malloc(size * sizeof(unsigned int));
  *J = malloc(size * sizeof(unsigned int));
  *W = malloc(size * sizeof(double));
  while(fgets(buff, 256, filein)){
    if (buff[0] == '#')
      continue;
    if (K == size){
      size += 20;
      *I = realloc(*I, size*sizeof(unsigned int));
      *J = realloc(*J, size*sizeof(unsigned int));
      *W = realloc(*W, size*sizeof(double));
    }
    ptr = strtok(buff, " "); /* read the first node */
    VALID_PTR_OR_EXIT(ptr, 7);
    (*I)[K] = atoi(ptr);
    ptr = strtok(NULL, " "); /* read the second node */
    VALID_PTR_OR_EXIT(ptr, 7);
    (*J)[K] = atoi(ptr);
    ptr = strtok(NULL, " "); /* read the weight */
    if (!ptr)
      (*W)[K] = 1.0;
    else
      (*W)[K] = atof(ptr);
    K += 1;
  }
  if (K > 0){
    *I = realloc(*I, K * sizeof(unsigned int));
    *J = realloc(*J, K * sizeof(unsigned int));
    *W = realloc(*W, K * sizeof(double));
  }
  return K;
}



void read_slap(FILE *filein, unsigned int *K, unsigned int *N, 
              unsigned int **J_slap, unsigned int **r_slap){
  
  unsigned int *I=NULL, *J=NULL;
  unsigned int i, k;
  
  k = read_ij(filein, &I, &J);
  *K = 2 * k;
  I = realloc(I, 2*k * sizeof(unsigned int));
  J = realloc(J, 2*k * sizeof(unsigned int));
  for (i=k; i<2*k; i ++){
    I[i] = J[i-k];
    J[i] = I[i-k];
  }
  
  *N = convert_ij2slap(I, J, 2*k, r_slap, J_slap);
  free(I);
  free(J);
  return;
}

void read_slap_w(FILE *filein, unsigned int *K, unsigned int *N, 
                 unsigned int **J_slap, unsigned int **r_slap, double **W_slap){
  
  unsigned int *I=NULL, *J=NULL;
  double *W=NULL;
  unsigned int i, k;
  
  k = read_ij_w(filein, &I, &J, &W);
  *K = 2 * k;
  if (*K > 0){
    I = realloc(I, (*K) * sizeof(unsigned int));
    J = realloc(J, (*K) * sizeof(unsigned int));
    W = realloc(W, (*K) * sizeof(double));
  }
  for (i=k; i<2*k; i ++){
    I[i] = J[i-k];
    J[i] = I[i-k];
    W[i] = W[i-k];
  }
  
  *N = convert_ij2slap_w(I, J, W, 2*k, r_slap, J_slap, W_slap);
  free(I);
  free(J);
  free(W);
  return;
}

/**
 *
 * Read an I-J (directed) edge list, and transform it in SLAP
 * notation, where the members of J_slap will be the outgoing
 * neighbours
 *
 */
void read_slap_dir(FILE *filein, unsigned int *K, unsigned int *N, 
              unsigned int **J_slap, unsigned int **r_slap){
  
  unsigned int *I=NULL, *J=NULL;
  unsigned int k;
  
  k = read_ij(filein, &I, &J);
  *K = k;
  
  *N = convert_ij2slap(I, J, k, r_slap, J_slap);
  free(I);
  free(J);
  return;
}

/**
 *
 * Read an I-J (directed) edge list, and transform it in SLAP
 * notation, where the members of J_slap will be the incoming
 * neighbours
 *
 */
void read_slap_dir_incoming(FILE *filein, unsigned int *K, unsigned int *N, 
              unsigned int **J_slap, unsigned int **r_slap){
  
  unsigned int *I=NULL, *J=NULL;
  unsigned int k;
  
  k = read_ij(filein, &I, &J);
  *K = k;
  
  *N = convert_ij2slap(J, I, k, r_slap, J_slap);
  free(I);
  free(J);
  return;
}




unsigned int find_max(unsigned int *v, unsigned int N){

  unsigned int i, max;
  
  max = v[0];
  i = 0;
  while(++i < N){
    if (v[i] > max)
      max = v[i];
  }
  return max;
}


int convert_ij2slap(unsigned int *I, unsigned int *J, unsigned int K, 
                    unsigned int ** r_slap, unsigned int **J_slap){
  
  unsigned int tmp, max;
  unsigned int i, pos;
  unsigned int *p;

  if (K < 1){
    return 0;
  }
  
  max = find_max(I, K) + 1;
  tmp = find_max(J, K) + 1;
  if (tmp > max){
    max = tmp ;
  }
  
  *r_slap = malloc( (max+1) * sizeof(unsigned int));
  p = malloc(max * sizeof(unsigned int));
  
  *J_slap = malloc(K * sizeof(unsigned int));
  
  memset(*r_slap, 0, (max+1) * sizeof(unsigned int));
  for(i=0; i<max + 1; i++)
    (*r_slap)[i] = 0;
  memset(p, 0, max * sizeof(unsigned int));
  (*r_slap)[0] = 0;
  for(i=0; i<K; i++){
    (*r_slap)[ I[i] + 1] += 1;
  }
  for(i=1; i<=max; i++){
    (*r_slap)[i] += (*r_slap)[i-1];
  }
  for(i=0; i<K; i++){
    pos = (*r_slap) [ I[i] ] + p[ I[i] ];
    (*J_slap)[pos] = J[i];
    p[ I[i] ] += 1;
  }
  free(p);
  return max;
}



int convert_ij2slap_w(unsigned int *I, unsigned int *J, double *W,  unsigned int K, 
                      unsigned int ** r_slap, unsigned int **J_slap, 
                      double **W_slap){
  
  unsigned int tmp, max;
  unsigned int i, pos;
  unsigned int *p;

  max = find_max(I, K) + 1;
  tmp = find_max(J, K) + 1;
  if (tmp > max){
    max = tmp ;
  }
  if (K<1){
    return 0;
  }
  
  *r_slap = malloc( (max+1) * sizeof(unsigned int));
  p = malloc(max * sizeof(unsigned int));
  
  *J_slap = malloc(K * sizeof(unsigned int));
  *W_slap = malloc(K * sizeof(double));
  
  memset(*r_slap, 0, (max+1) * sizeof(unsigned int));
  for(i=0; i<max + 1; i++)
    (*r_slap)[i] = 0;
  memset(p, 0, max * sizeof(unsigned int));
  (*r_slap)[0] = 0;
  for(i=0; i<K; i++){
    (*r_slap)[ I[i] + 1] += 1;
  }
  for(i=1; i<=max; i++){
    (*r_slap)[i] += (*r_slap)[i-1];
  }
  for(i=0; i<K; i++){
    pos = (*r_slap) [ I[i] ] + p[ I[i] ];
    (*J_slap)[pos] = J[i];
    (*W_slap)[pos] = W[i];
    p[ I[i] ] += 1;
  }
  free(p);
  return max;
}


int convert_ij2slap_N(unsigned int *I, unsigned int *J, unsigned int K,   
                      unsigned int N, unsigned int ** r_slap, 
                      unsigned int **J_slap){
  
  unsigned int  max;
  unsigned int i, pos;
  unsigned int *p;

  max = N;
  
  *r_slap = malloc( (max+1) * sizeof(unsigned int));
  p = malloc(max * sizeof(unsigned int));
  
  *J_slap = malloc(K * sizeof(unsigned int));
  
  memset(*r_slap, 0, (max+1) * sizeof(unsigned int));
  for(i=0; i<max + 1; i++)
    (*r_slap)[i] = 0;
  memset(p, 0, max * sizeof(unsigned int));
  (*r_slap)[0] = 0;
  for(i=0; i<K; i++){
    (*r_slap)[ I[i] + 1] += 1;
  }
  for(i=1; i<=max; i++){
    (*r_slap)[i] += (*r_slap)[i-1];
  }
  for(i=0; i<K; i++){
    pos = (*r_slap) [ I[i] ] + p[ I[i] ];
    (*J_slap)[pos] = J[i];
    p[ I[i] ] += 1;
  }
  free(p);
  return max;
}



/* RIVEDERE QUESTA  FUNZIONE...... PASSARE UN FILE COME ARGOMENTO E
   USARE fprintf */
void dump_deg_distr(unsigned int *degs, double *p, int n){

  int i;

  for(i=0; i<n; i++){
    printf("%d %2.6f\n", degs[i], p[i]);
  }
}



/* RIVEDERE QUESTA  FUNZIONE...... PASSARE UN FILE COME ARGOMENTO E
   USARE fprintf */
void dump_deg_seq(unsigned int *nodes, int N){

  int i;
  for(i=0; i<N; i++){
    printf("%d: %d\n", i, nodes[i]);
  }
}


FILE* openfile_or_exit(char *filename, char *mode, int exitcode){
  
  FILE *fileout;
  char error_str[256];
  
  fileout = fopen(filename, mode);
  if (!fileout){
    sprintf(error_str, "Error opening file %s", filename);
    perror(error_str);
    exit(exitcode);
  }
  return fileout;
}

int compare_int(const void *x1, const void *x2){
  return *((unsigned int*)x1) - *((unsigned int*)x2);
}

int compare_double(const void *elem1, const void *elem2){
  
  double *l1, *l2;

  l1 = (double*)elem1;
  l2 = (double*)elem2;
  return (*l1 < *l2 ? -1 : (*l1 > *l2 ? 1 : 0));
}

void print_int(void *e){

  int d;

  d = *((int*)e);
  printf("%d ", d);
}

void print_double(void *e){
  
  double d;

  d = *((double*)e);
  printf("%g ", d);
}



void write_edges(FILE *fileout, unsigned int *J_slap, 
                 unsigned int *r_slap, unsigned int N){
  
  unsigned int i, j;
  
  for(i=0; i<N; i++){
    for (j=r_slap[i]; j<r_slap[i+1]; j++){
      if (J_slap[j] > i){
        fprintf(fileout, "%d %d\n", i, J_slap[j]);
      }
    }
  }
}


void write_edges_dir(FILE *fileout, unsigned int *J_slap, 
                 unsigned int *r_slap, unsigned int N){
  
  unsigned int i, j;
  
  for(i=0; i<N; i++){
    for (j=r_slap[i]; j<r_slap[i+1]; j++){
      fprintf(fileout, "%d %d\n", i, J_slap[j]);
    }
  }
}


  

/* Check if j is a neighbour of i */
int is_neigh(unsigned int *J_slap, unsigned int *r_slap, unsigned int N, 
             unsigned int i, unsigned int j){
  
  unsigned int l;
  unsigned int count;
  count = 0;
  if (i >=N || j >=N)
    return 0;
  for(l=r_slap[i]; l<r_slap[i+1]; l++){
    if (J_slap[l] == j)
      count ++;
  }
  return count;
}


/* Check if j is a neighbour of i, using bsearch. 

   BE CAREFUL!!!! THIS WORKS ONLY IF THE LIST OF NEIGHBOURS HAS BEEN
   PREVIOUSLY SORTED APPROPRIATELY, E.G. BY CALLING
   sort_neighbours() 
   
*/
int is_neigh_bs(unsigned int *J_slap, unsigned int *r_slap, unsigned int N, 
                          unsigned int i, unsigned int j){
  
  unsigned int *ptr;

  ptr = bsearch(&j, & (J_slap[r_slap[i]]), r_slap[i+1] - r_slap[i], sizeof(unsigned int),
                compare_int);
  return (ptr == NULL ? 0: 1);
}

int find_neigh_in_Jslap(unsigned int *J_slap, unsigned int *r_slap, unsigned int N, 
                        unsigned int i, unsigned int j, unsigned int *ret){

  unsigned int *ptr;

  ptr = bsearch(&j, & (J_slap[r_slap[i]]), r_slap[i+1] - r_slap[i], sizeof(unsigned int),
                compare_int);
  if (ptr == NULL){
    return 0;
  }
  else{
    *ret = (ptr - J_slap);
    return 1;
  }
}


double get_neigh_weight(unsigned int *J_slap, unsigned int *r_slap, double *W_slap, 
                        unsigned int N,  unsigned int i, unsigned int j){
  
  unsigned int l;
  double w = 0.0;
  
  for(l=r_slap[i]; l<r_slap[i+1]; l++){
    if (J_slap[l] == j)
      w += W_slap[l];
  }
  return w;
}





void sort_neighbours(unsigned int *J_slap, unsigned int *r_slap, unsigned int N){
  
  unsigned int i, *base;

  for(i=0; i<N; i++){
    base = J_slap + r_slap[i];
    qsort(base, r_slap[i+1] - r_slap[i], sizeof(unsigned int), compare_int);
  }
}

double strength(unsigned int *r_slap,  double *W_slap, int i){

  double s = 0;
  int j;

  for(j = r_slap[i]; j<r_slap[i+1]; j++){
    s += W_slap[j];
  }
  return s;

}


void convert_slap2ij(unsigned int *J_slap, unsigned int *r_slap, int N, unsigned int *I, unsigned int *J){
  
  int i, j, num;
  
  num = 0;
  for(i=0; i<N; i++){
    for(j=r_slap[i]; j< r_slap[i+1]; j++){
      I[num] = i;
      J[num] = J_slap[j];
      num += 1;
    }
  }
}

void show_progress(FILE *fout, char *s, unsigned int progress, unsigned int total){
  
  char str[21]; /* 20 spaces */
  double frac;
  int num_char, i;

  if (total){
    frac = 1.0*progress/total;
  }
  else{
    frac = 0.0;
  }

  num_char = (int)(frac * 20) - 1;
  i = 20;
  while(--i >= 0){
    if (i > num_char) 
      str[i] = ' ';
    else
      str[i] = '.';
  }
  str[20] = '\0';
  fprintf(fout, "\r%s [%s] %3d%%\r", s, str, (int)(frac * 100));
}


void shuffle_vector(unsigned int *v, unsigned int N){
  
  int i, pos;

  for(i=N-1; i>=0; i--){
    pos = rand() % N;
    if (pos != i){
      v[i] ^= v[pos];
      v[pos] ^= v[i];
      v[i] ^= v[pos];
    }
  }
}


unsigned int read_partition(FILE *fin, unsigned int N, unsigned int *part){

  unsigned int i;
  char *ptr;
  char buff[256];
  unsigned int max_part = 0, val;
   
  
  while(fgets(buff, 256, fin)){
    if (buff[0] == '#')
      continue;
    ptr = strtok(buff, " "); /* read the  node */
    VALID_PTR_OR_EXIT(ptr, 7);
    i = atoi(ptr);
    if(i >= N){
      fprintf(stderr, "Index %d out of bounds (0, %d) in read_partition (%s: %d)\n", 
              i, N, __FILE__, __LINE__);
    }
    ptr = strtok(NULL, " "); /* read the partition number */
    VALID_PTR_OR_EXIT(ptr, 7);
    val = atoi(ptr);
    if (val > max_part){
      max_part = val;
    }
    part[i] = val;
  }
  return max_part;
}

int degree(unsigned int *r_slap, unsigned int i){

  return r_slap[i+1] - r_slap[i];
}

int my_strcasecmp(const char *s1, const char *s2){

  char *c1, *c2;
  int l1, l2;
  int res, i;

  l1 = strlen(s1);
  l2 = strlen(s2);
  
  c1 = malloc((1 + l1) * sizeof(char));
  c2 = malloc((1 + l2) * sizeof(char));
  
  for (i=0; i<l1; i++){
    c1[i] = tolower(s1[i]);
  }
  c1[i] = '\0';

  for (i=0; i<l2; i++){
    c2[i] = tolower(s2[i]);
  }
  c2[i] = '\0';

  res = strcmp(c1, c2);
  free(c2);
  free(c1);
  return res;
}

