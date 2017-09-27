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
 *  This program grows a weighted network using the model proposed by
 *  Barrat, Barthelemy, and Vespignani.
 *
 *  References:
 *
 *  [1] A. Barrat, M. Barthelemy, and A. Vespignani. "Weighted
 *     Evolving Networks: Coupling Topology and Weight
 *     Dynamics". Phys. Rev. Lett. 92 (2004), 228701. 
 *
 *  [2] A. Barrat, M. Barthelemy, and A. Vespignani. "Modeling the
 *     evolution of weighted networks". Phys. Rev. E 70 (2004),
 *     066149.  
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "cum_distr.h"

void usage(char *argv[]){
  printf("********************************************************************\n"
         "**                                                                **\n"
         "**                        -*-    bbv    -*-                       **\n"
         "**                                                                **\n"
         "**   Grow a weighted network of 'N' nodes using the model         **\n"
         "**   proposed by Barrat-Barthelemy-Vespignani.                    **\n"
         "**                                                                **\n"
         "**   The initial network is a clique of 'n0' nodes, and each new  **\n"
         "**   node creates 'm' edges. All edges have an initial weight     **\n"
         "**   equal to 'w0', and the attachment probability in of the      **\n"
         "**   form:                                                        **\n"
         "**                                                                **\n"
         "**            P(i->j) ~ s_j                                       **\n"
         "**                                                                **\n"
         "**   where s_j is the strength of node j. The parameter 'delta'   **\n"
         "**   tunes the rearrangement of edge weights due to the           **\n"
         "**   addition of a new edge. The degree, strength, and weight     **\n"
         "**   distributions of the created graphs are power-laws,          **\n"
         "**   whose esponents depend on the values of 'w0' and 'delta'.    **\n"
         "**                                                                **\n"
         "********************************************************************\n"
         " This is Free Software - You can use and distribute it under \n"
         " the terms of the GNU General Public License, version 3 or later\n\n"
         " (c) Vincenzo Nicosia 2009-2017 (v.nicosia@qmul.ac.uk)\n\n"
         "********************************************************************\n\n"
         );
  printf("Usage: %s <N> <m> <n0> <w0> <delta>\n", argv[0]);
}


typedef struct{
  int id;
  double w;
  double delta_w;
} link_t;


typedef struct {
  int id;
  int size;
  int deg;
  double s;
  link_t *neighs;
} node_t;


/* create the initial graph as a clique of n0 nodes */
void init_seed(node_t *G, int n0, double w0, cum_distr_t *d){

  int i, j, n;

  for(i=0; i< n0; i++){
    G[i].neighs = malloc((n0-1) * sizeof(link_t));
    G[i].size = n0-1;
    G[i].deg = n0-1;
    n= 0;
    for (j=0; j<n0; j++){
      if (i != j){
        G[i].neighs[n].id = j;
        G[i].neighs[n].w = w0;
        n += 1;
      }
    }
    G[i].s = (n0-1) * w0;
    cum_distr_add(d, i, G[i].s);
  }
}

/* add j to the neighbourhood of i with a weight w0, and update the
   strength of i */
void add_neigh(node_t *G, unsigned int i, unsigned int j, double w0){
  
  if (G[i].deg == G[i].size){
    G[i].size += 5;
    G[i].neighs= realloc(G[i].neighs, G[i].size * sizeof(link_t));
  }
  G[i].neighs[G[i].deg].id = j;
  G[i].neighs[G[i].deg].w = w0;
  G[i].deg += 1;
  G[i].s += w0;
}

/* Add w to the weight of the edge (i,j) */
void add_weight(node_t *G, int i, int j, double w){

  int k;
  for(k=0; k<G[i].deg; k++){
    if(G[i].neighs[k].id == j){
      G[i].neighs[k].w += w;
    }
  }
}

/* Compute the weight increase for each edge connected to node i */
void compute_delta_weights(node_t *G, int i, double delta){
  int j;
  double s;

  s = G[i].s;

  for(j=0; j< G[i].deg; j++){   /* for each neighbour of i */
    /* compute the delta_weight */
    G[i].neighs[j].delta_w = delta * G[i].neighs[j].w / s;
  }
}

/* set the new weights on the edges connected to node i */
void set_delta_weights(node_t *G, int i, cum_distr_t *distr){

  int j, neigh;

  double delta_w;
  
  for(j=0; j<G[i].deg; j++){
    neigh = G[i].neighs[j].id;
    delta_w = G[i].neighs[j].delta_w;
    /* add delta_w to the weight of (i, neigh) and of (neigh, i) */
    add_weight(G, i, neigh, delta_w);
    add_weight(G, neigh, i, delta_w);
    
    /* update the strength of neigh */
    G[neigh].s += delta_w;
    /* add delta_w to the fraction of cum_distr associated to neigh */
    cum_distr_add(distr, neigh, delta_w);
  }
}

/* return 1 if i is in the array v */

int is_neigh(int *v, int N, int i){
  
  int j;
  
  for(j=0; j<N; j++){
    if (v[j] == i)
      return 1;
  }
  return 0;
}

/* 
 *  print the edges of the undirected graph G, with the corresponding
 *  weight. Each edge is printed only once
 *
 */
void dump_net(node_t *G, int N){

  int i, j;

  double tot_w = 0.0;
  
  for(i=0; i<N; i++){
    for(j=0; j<G[i].deg; j++){
      if(G[i].neighs[j].id > i){
        tot_w += G[i].neighs[j].w;
        printf("%d %d %g\n", i, G[i].neighs[j].id, G[i].neighs[j].w);
      }
    }
  }
}


/* grow a weighted graph using the BBV model */
node_t* bbv(unsigned int N, unsigned int n0, unsigned int m, double w0, double delta){

  node_t *G;
  int t, i, j;
  cum_distr_t *distr = NULL;
  int *tmp_neighs;
  
  distr = cum_distr_init(N * m);

  
  G = malloc(N * sizeof(node_t));
  tmp_neighs = malloc(m * sizeof(int));

  init_seed(G, n0, w0, distr);

  
  for(t=n0; t<N; t++){
    /* Initialize the new node */
    G[t].neighs = malloc(m * sizeof(link_t));
    G[t].size = m;
    G[t].deg = 0;
    /* Sample the m neighbours */
    for(i=0; i<m; i++){
      j = cum_distr_sample(distr);
      while(is_neigh(tmp_neighs, i, j)){
          j = cum_distr_sample(distr);
      }
      tmp_neighs[i] = j;
    }
    /* compute the weight increase for the neighbours of the
       new node t */
    for(i=0; i<m; i++){/* for each neighbour 'l' of the new node t */
      /* compute the weight increase for the edges around 'l' */
      compute_delta_weights(G, tmp_neighs[i], delta);
    }
    /* Now we update the weights */
    for(i=0; i<m; i++){/* for each neighbour 'l' of the new node t */
      set_delta_weights(G, tmp_neighs[i], distr);
      add_neigh(G, t, tmp_neighs[i], w0);
      add_neigh(G, tmp_neighs[i], t, w0);
      /* We need to add delta to the strength of tmp_neighs[i] {notice
         that w0 has been already added by the previous call to
         add_neigh()}*/
      G[tmp_neighs[i]].s += delta;
      cum_distr_add(distr, tmp_neighs[i], delta + w0);
    }
    
    /* Finally, we update the strength of node t */
    G[t].s = w0 * m;
    cum_distr_add(distr, t, G[t].s);
  }
  free(tmp_neighs);
  cum_distr_destroy(distr);
  return G;
}



int main(int argc, char *argv[]){
  
  int N, n0, m, i;
  double w0, delta;
  
  node_t *net;

  if (argc < 6){
    usage(argv);
    exit(1);
  }
  
  N = atoi(argv[1]);
  m = atoi(argv[2]);
  n0 = atoi(argv[3]);
  w0 = atof(argv[4]);
  delta = atof(argv[5]);

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

  if (w0 <= 0.0){
    fprintf(stderr, "w0 must be positive\n");
    exit(1);
  }

  if (delta < 0.0){
    fprintf(stderr, "delta must be positive\n");
    exit(1);
  }


  net = bbv(N, n0, m, w0, delta);
  dump_net(net, N);

  for(i=0; i<N; i++){
    free(net[i].neighs);
  }
  
  free(net);
}
