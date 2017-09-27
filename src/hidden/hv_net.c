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
 *  This in an implementation of the hidden-variable model by Bogunya'
 *  and Pastor-Satorras. The program creates a random graph whose
 *  joint degree distribution is identical to that of a network given
 *  as input.
 *
 *  References:
 *  
 *  [1] M. Boguna and R. Pastor-Satorras. "Class of correlated random
 *  networks with hidden variables". Phys. Rev. E 68 (2003), 036112.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "utils.h"
#include "cum_distr.h"


void usage(char *argv[]){
  printf("********************************************************************\n"
         "**                                                                **\n"
         "**                        -*-   hv_net   -*-                      **\n"
         "**                                                                **\n"
         "**   Create a random graph whose joint degree distribution is     **\n"
         "**   identical to that of a network provided as input.            **\n"
         "**                                                                **\n"
         "**   If 'graph_in' is equal to '-' (dash), read the edge list     **\n"
         "**   from STDIN.                                                  **\n"
         "**                                                                **\n"
         "**   The program prints on STDOUT the edge-list of the sampled    **\n"
         "**   graph.                                                       **\n"
         "**                                                                **\n"
         "**   If 'SHOW' is specified as second parameter, the program      **\n"
         "**   prints on STDERR the values of the hidden variable           **\n"
         "**   associated to each node and of the actual degree they have   **\n"
         "**   in the newly-created random graph, in the format:            **\n"
         "**                                                                **\n"
         "**       h_1 k_1                                                  **\n"
         "**       h_2 k_2                                                  **\n"
         "**       ...                                                      **\n"
         "**       h_N k_N                                                  **\n"
         "**                                                                **\n"
         "********************************************************************\n"
         " This is Free Software - You can use and distribute it under \n"
         " the terms of the GNU General Public License, version 3 or later\n\n"
         " (c) Vincenzo Nicosia 2009-2017 (v.nicosia@qmul.ac.uk)\n\n"
         "********************************************************************\n\n"
         );
  printf("Usage: %s <graph_in> [SHOW]\n", argv[0]);
}



typedef struct{
  unsigned int N;
  unsigned int K;
  unsigned int *J_slap;
  unsigned int *r_slap;
  unsigned int k_min;
  unsigned int k_max;
  double *pk; /* degree distribution -- this is equal to rho!!!! */
  double **pkk; /* joint degree distribution */
  double **f; /* the linking function */
  unsigned int *h; /* The array containing hidden variables for the N nodes */
  unsigned int *new_degs; /* the new degrees of the sampled graph */
} params_t;


/**
 * 
 * sample the hidden degrees of the nodes using the cumulative
 * distribution P(k)
 *
 */
void sample_node_variables(params_t *p, cum_distr_t *P){
  
  unsigned int i;
  unsigned int k;
  

  for(i=0; i<p->N; i++){
    k = cum_distr_sample(P);
    p->h[i] = k;
  }
}

/**
 *
 * Compute the function f(h,h') for each pair of degrees
 *
 */
void compute_fhh(params_t *p){
  
  unsigned int k1, k2;
  double avg_k;

  avg_k = 1.0 * p->K / p->N;
  
  for(k1 = p->k_min; k1 <= p->k_max; k1++){
    for(k2 = k1; k2 <= p->k_max; k2++){
      p->f[k1][k2] = p->f[k2][k1] = avg_k * p->pkk[k1][k2] / (p->N * p->pk[k1] * p->pk[k2]);
    }
  }
}



/**
 * 
 * Sample the edges of the graph
 *
 */
void sample_edges(params_t *p){
 
  unsigned int i, j, ki, kj;
  double val;

  for(i=0; i<p->N; i++){
    ki = p->h[i];
    for (j=i+1; j<p->N; j++){
      kj = p->h[j];
      val = 1.0 * rand() / RAND_MAX;
      /* we create the edge (i,j) with proability f[ki][kj]*/
      if (val < p->f[ki][kj]){
        printf("%d %d\n", i, j);
        p->new_degs[i] += 1;
        p->new_degs[j] += 1;
      }
    }
  }
}


void init_hv_params(params_t *p){
  
  unsigned int  i;

  p->h = malloc(p->N * sizeof(unsigned int));
  p->f = malloc((p->k_max + 1) * sizeof(double*));
  p->new_degs = malloc(p->N * sizeof(unsigned int));

  for(i=0; i<=p->k_max; i++){
    p->f[i] = malloc((p->k_max + 1) * sizeof(double));
  }
  for (i=0; i<p->N; i++){
    p->new_degs[i] = 0;
  }
}



void compute_distributions(params_t *g){
  
  unsigned int i,j, neigh, k1, k2;
  double sumk, sumkk;

  g->k_min = g->N;
  g->k_max = 0;
  
  g->pk = NULL;

  /* Compute (and allocate on the fly) the degree distribution */

  for(i=0; i< g->N; i ++){
    k1 = g->r_slap [i+1] - g->r_slap[i];
    if (k1 > g->k_max){
      g->pk = realloc(g->pk, (k1 + 1) * sizeof(double));
      for(j=g->k_max+1; j<=k1; j++){
        g->pk[j] = 0;
      }
      g->k_max = k1;
    }
    else if(k1 < g->k_min){
      g->k_min = k1;
    }
    g->pk[k1] += 1;
  }
  
  /* Allocate and initialise the joint degree distribution */
  g->pkk = malloc((g->k_max + 1 ) * sizeof(double*));
  for(i=0; i<=g->k_max ; i++){
    g->pkk[i] = malloc((g->k_max + 1) * sizeof(double));
    memset(g->pkk[i], 0, (g->k_max + 1) * sizeof(double));
  }
  
  for(i=0; i<g->N; i++){
    k1 = g->r_slap[i+1] - g->r_slap[i];
    for(j=g->r_slap[i]; j< g->r_slap[i+1]; j++){
      neigh = g->J_slap[j];
      k2 = g->r_slap[neigh+1] - g->r_slap[neigh];
      g->pkk[k1][k2] += 1;
    }
  }

  /* Now we normalise both pk and pkk...*/

  sumk = sumkk = 0.0;
  
  for(i=0; i<=g->k_max; i++){
    g->pk[i] /= g->N;
    sumk += g->pk[i];
    g->pkk[i][i] /= (g->K);
    sumkk += g->pkk[i][i];
    for(j=i+1; j<=g->k_max; j++){
      g->pkk[i][j] /= (g->K);
      g->pkk[j][i] = g->pkk[i][j];
      sumkk += 2* g->pkk[i][j];
    }
  }
    
}


void compute_rho(params_t *g, cum_distr_t *P){
  
  unsigned int k;

  for(k=g->k_min; k<= g->k_max; k++){

    if(g->pk[k] > 0){
      if (cum_distr_add(P, k, g->pk[k]) != 0){
        fprintf(stderr, "Error in the computation of the cumulative degree distribution!!!!"
               " Exiting!!!\n");
        exit(3);
      }
    }
  }
}

void dump_deg_h(params_t *p){

  unsigned int i;

  for(i=0; i<p->N; i++){
    fprintf(stderr, "%d %d\n", p->h[i], p->new_degs[i]);
  }
}


int main(int argc, char *argv[]){
  
  params_t p;
  cum_distr_t *P;
  int i;

  FILE *filein;


  if (argc < 2){
    usage(argv);
    exit(1);
  }

  srand(time(NULL));

  if (!strcmp(argv[1], "-")){
    /* take the input from STDIN */
    filein = stdin;
  }
  else {
    filein = openfile_or_exit(argv[1], "r", 2);
  }
  
  p.J_slap = p.r_slap = NULL;
  
  read_slap(filein, &(p.K), &(p.N), &(p.J_slap), &(p.r_slap));

  fclose(filein);
  
  compute_distributions(&p);

  init_hv_params(&p);
  
  P = cum_distr_init(p.k_max + 1);
  compute_rho(&p, P);
  compute_fhh(&p);
  
  sample_node_variables(&p, P);
  sample_edges(&p);
  
  /* 
   *  If 'SHOW' is provided, print on STDERR the value of the hidden
   *  variable of each node
   */
  if (argc > 2 && !my_strcasecmp(argv[2], "SHOW")){
    dump_deg_h(&p);
  }

  cum_distr_destroy(P);
  free(p.J_slap);
  free(p.r_slap);
  free(p.pk);
  free(p.h);
  free(p.new_degs);
  for(i=0; i<=p.k_max ; i++){
    free(p.pkk[i]);
    free(p.f[i]);
  }
  free(p.pkk);
  free(p.f);
}

