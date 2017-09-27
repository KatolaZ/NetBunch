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
 *  This program implements the Girvan-Newman algorithm for community
 *  detection, based on the removal of edges with largest betweenness.
 *
 *
 *  References: 
 *
 * [1] M. Girvan and M. E. J. Newman. "Community structure in social
 *     and biological networks". P. Natl. Acad. Sci. USA 99 (2002),
 *     7821--7826.  
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "utils.h"

void usage(char *argv[]){

  printf("********************************************************************\n"
         "**                                                                **\n"
         "**                         -*-   gn   -*-                         **\n"
         "**                                                                **\n"
         "**   Find the communities of the input graph 'graph_in' using     **\n"
         "**   the Girvan-Newman algorithm (successive removal of edges     **\n"
         "**   with high betweeneess).                                      **\n"
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
         "**    The program prints on STDOUT the partition corresponding    **\n"
         "**    to the largest value of modularity, in the format:          **\n"
         "**                                                                **\n"
         "**        node_1 comm_1                                           **\n"
         "**        node_2 comm_2                                           **\n"
         "**        node_3 comm_3                                           **\n"
         "**       .....                                                    **\n"
         "**                                                                **\n"
         "**    where 'comm_1' is the community to which 'node_1' belongs.  **\n"
         "**                                                                **\n"
         "**    The program prints on STDERR the number of communities and  **\n"
         "**    the value of modularity obtained after the removal of each  **\n"
         "**    edge, in the format:                                        **\n"
         "**                                                                **\n"
         "**                                                                **\n"
         "**        ## nc: NUM_COMM Q_max: Q_MAX                            **\n"
         "**        nc_1 Q_1                                                **\n"
         "**        nc_2 Q_2                                                **\n"
         "**        nc_3 Q_3                                                **\n"
         "**        ...                                                     **\n"
         "**                                                                **\n"
         "**    where 'nc_1', 'nc_2', 'nc_3', etc. is the number of         **\n"
         "**    communities (connected components) remaining after the      **\n"
         "**    1st, 2nd, 3rd, etc. edge has been removed, and 'Q_1',       **\n"
         "**    'Q_2', 'Q_3', etc. are the value of the modularity          **\n"
         "**    function of the corresponding node partition. The first     **\n"
         "**    output line reports the number of communities NUM_COMM      **\n"
         "**    and corresponding value of modularity Q_MAX of the best     **\n"
         "**    partition found.                                            **\n"
         "**                                                                **\n"
         "********************************************************************\n"
         " This is Free Software - You can use and distribute it under \n"
         " the terms of the GNU General Public License, version 3 or later\n\n"
         " Please visit http://www.complex-networks.net for more information\n\n"
         " (c) Vincenzo Nicosia 2009-2017 (v.nicosia@qmul.ac.uk)\n"
         "********************************************************************\n\n"
         );
  printf("Usage: %s <graph_in>\n", argv[0]);
  exit(1);
}


void add_predecessor(unsigned int **pred, unsigned int k){
  
  (*pred)[0] += 1;
  *pred = realloc(*pred, ((*pred)[0] + 1)  * sizeof(unsigned int));
  (*pred)[ (*pred)[0] ] = k;
}



/*
 *
 * Compute node and edge betweenness, based on shortest paths
 * originating on the "num" nodes specified in "nlist".  "edge_bet"
 * should be an appropriately allocated (and initialised to zero!!!!)
 * vector of length equal to J_slap, and will contain the values of
 * edge betweenness.
 *
 */
double* compute_bet_dependency_active(unsigned int N, unsigned int *J_slap, unsigned int *r_slap, 
                                      double *edge_bet, char *active){
  
  static unsigned int  *marked, **preds, *dist, *nj;
  static double  *delta, *cB;
  int i, j, k, w, idx, cur_node;
  double val;
  unsigned int d;
  unsigned int n, nd, ndp;
  unsigned int edge_pos;

  if (!dist)
    dist = malloc(N * sizeof(unsigned int));
  if (!marked)
    marked = malloc(N * sizeof(unsigned int));
  if (!preds)
    preds = malloc(N * sizeof(unsigned int *));
  if (!nj)
    nj = malloc(N * sizeof(unsigned int));
  if (!delta)
    delta = malloc(N * sizeof(double));
  if (!cB)
    cB = malloc(N * sizeof(double));
  
  for (i=0; i<N; i++){
    cB[i] = 0;
    preds[i] = NULL;
  }

  for (j=0; j<N; j++){
    for(i=0; i<N; i++){
      dist[i] = N;
      if (preds[i] == NULL){
        preds[i] = malloc(sizeof(unsigned int));
      }
      preds[i][0] = 0; /* The list of predecessors is now empty! */
      
      nj[i] = 0;
      delta[i]= 0;
    }
    dist[j] = 0;
    nj[j] = 1;
    marked[0] = j;
    d = 0;
    n = 0;
    nd = 1;
    ndp = 0;
    while (d<N && nd > 0){
      for(i = n; i< n+nd; i ++){
        cur_node = marked[i];
        for (k=r_slap[cur_node]; k<r_slap[cur_node +1] ; k++){
          w = J_slap[k];
          if (!active[k])
            /* discard inactive links */
            continue;
          
          if ( dist[w] == d+1){
            add_predecessor((unsigned int **)(preds + w), cur_node);
            nj[w] += nj[cur_node];
          }
          if ( dist[w] == N){
            dist[w] = d+1;
            marked[n + nd + ndp] = w;
            add_predecessor(preds + w, cur_node);
            ndp +=1;
            nj[w] += nj[cur_node];
          }
        }
      }
      n = n + nd;
      nd = ndp;
      ndp = 0;
      d += 1; 
    }
    for (k= n-1; k>=1; k--){
      w = marked[k];
      for (idx=1; idx <= preds[w][0]; idx ++ ){
        i = preds[w][idx];
        val = 1.0 * nj[i] / nj[w] * (1 + delta[w]);
        delta[i] += val;
        /* Now we should update the betweenness of the edge (i,w) in
           the appropriate position of the vector  edge_bet*/
        find_neigh_in_Jslap(J_slap, r_slap, N, i, w, &edge_pos);
        edge_bet[edge_pos] += val;
        find_neigh_in_Jslap(J_slap, r_slap, N, w, i, &edge_pos);
        edge_bet[edge_pos] += val;
      }
      cB[w] += delta[w];
    }
  }
  //free(marked);
  return cB;
}



/**
 *
 * Depth-First search on the node i.... 
 *
 */
int dfs_active(unsigned int i, unsigned int *J_slap, unsigned int *r_slap, 
               unsigned int N, unsigned int nc, unsigned int *ic, unsigned int *f, 
               char reset, char *active){
  
  static unsigned int time;
  unsigned int j, s;
  
  if(reset){
    time = 0;
  }
  
  ic[i] = nc;
  s = 1;
  time += 1;
  
  for(j=r_slap[i]; j<r_slap[i+1]; j++){
    if (ic[J_slap[j]] == 0 && active[j]){
      s += dfs_active(J_slap[j], J_slap, r_slap, N, nc, ic, f, 0, active);
    }
  }
  time += 1;
  f[i] = time;
  return s;
}

/**
 * 
 * Find all the components of the given graph
 *
 */

int components(unsigned int *J_slap, unsigned int *r_slap, unsigned int N, 
               unsigned int *ic, unsigned int *f, unsigned int *sizes, 
               char *active){

  unsigned int nc, s;
  unsigned int i;
  
  
  for(i=0; i<N; i++){
    ic[i] = 0;
    f[i] = 0;
  }
  nc = 0;
  for(i=0; i<N; i++){
    while( i<N && ic[i] != 0)
      i += 1;
    if (i == N)
      break;
    nc += 1;
    s = dfs_active(i, J_slap, r_slap, N, nc, ic, f, 1, active);
    sizes[nc] = s;
  }
  return nc;
}


/**
 * find the position of the element in v with maximal value. If there
 * are more than one element with the same value, return the position
 * of the last one found. In order to introduce randomness, the search
 * starts from a position sampled uniformly at random
 *
 */
unsigned int find_pos_max_rand(double *v, unsigned int K, char *active){
  
  unsigned int i;
  double max;
  unsigned int base, pos_max;
  
  base = rand() % K;
  while(! active[base] )
    base = (base + 1) % K;
  max = v[base];
  pos_max = base;
  
  for(i=base; i<base + K; i++){
    if (v[i % K] >= max && active[i % K]){
      max = v[i % K];
      pos_max = i % K;
    }
  }
  return pos_max;
}


unsigned int find_pos_max(double *v, unsigned int K, char *active){
  
  unsigned int i;
  double max;
  unsigned int base, pos_max;
  
  base = 0;
  while(! active[base] )
    base = (base + 1) % K;
  max = v[base];
  pos_max = base;
  
  for(i=base; i<base + K; i++){
    if (v[i % K] >= max && active[i % K]){
      max = v[i % K];
      pos_max = i % K;
    }
  }
  return pos_max;
}


/* This function compute the modularity function of the partition
   'part'...*/
double compute_modularity(unsigned int *J_slap, unsigned int *r_slap, unsigned int N, 
                          unsigned int *part, unsigned int nc){
  static double *e, *a;
  
  unsigned int i, j, n, K;
  unsigned int ci, cj;
  double Q;

  if(!e)
    e = malloc((N+1) * sizeof(double));
  if(!a)
    a = malloc((N+1) * sizeof(double));

  memset(e, 0, (N+1) * sizeof(double));
  memset(a, 0, (N+1) * sizeof(double));

  K = r_slap[N];
  
  for (i=0; i<N; i++){
    ci = part[i];
    a[ci] += (r_slap[i+1] - r_slap[i]); 
    for(j=r_slap[i]; j< r_slap[i+1]; j++){
      cj = part[J_slap[j]];
      if (ci == cj){
        e[ci] += 1;
      }
    }
  }
  
  Q = 0.0;
  for (n=1; n<=nc; n++){
    Q += 1.0 * e[n]/(1.0 * K ) - pow(1.0 * a[n]/K, 2);
  }
  return Q;
}


unsigned int* girvan_newman(unsigned int *J_slap, unsigned int *r_slap, unsigned int N){
  
  unsigned int K, nc, pos;
  double *edge_bet = NULL;
  char *active = NULL;
  double Q, Q_max;

  unsigned int *ic, *f, *sizes;
  unsigned int *best_part;
  
  int i, j, nc_max;
  
  K = r_slap[N];

  ic = malloc((N) * sizeof(unsigned int));
  f = malloc(N * sizeof(unsigned int));
  sizes = malloc((N+1) * sizeof(unsigned int));
  best_part = malloc(N * sizeof(unsigned int));
  

  /* We initialise the vector "active" which idicates active edges */
  
  active = malloc(K * sizeof(char));
  for (i=0; i<K; i++){
    active[i] = 1;
  }

  edge_bet = malloc(K * sizeof(double));
  nc_max = 0;
  Q_max = -1000;
  
  for(j=0; j<K; j++){
    memset(edge_bet, 0, K * sizeof(double));
    /* compute edge betweenness */
    compute_bet_dependency_active(N, J_slap, r_slap, edge_bet, active);
    /* find edge with maximal betweenness... */
    pos = find_pos_max_rand(edge_bet, K, active);
    /* and knock it down */
    active[pos] = 0;
    /* compute connected components */
    nc = components(J_slap, r_slap, N, ic, f, sizes, active);
    /* compute the modularity of the partition induced by components */
    Q = compute_modularity(J_slap, r_slap, N, ic, nc);
    fprintf(stderr, "%d %g\n", nc, Q);
    /* if Q is maximal, save the current partition */
    if (j > 0){
      if (Q > Q_max){
        Q_max = Q;
        nc_max = nc;
        memcpy (best_part, ic, N * sizeof(unsigned int));
      }
    }
    else{
      Q_max = Q;
      memcpy(best_part, ic, N*sizeof(unsigned int));
    }
  }
  /* Return the best partition */
  fprintf(stdout, "### nc: %d Q_max: %g\n", nc_max, Q_max);
  free(f);
  free(ic);
  free(sizes);
  free(edge_bet);
  free(active);
  return best_part;
}



void dump_partition(unsigned int *p, unsigned int N){
  
  unsigned int i;
  for(i=0; i<N; i++){
    fprintf(stdout, "%d %d\n", i, p[i]);
  }
}


int main(int argc, char *argv[]){
  

  unsigned int *J_slap=NULL, *r_slap=NULL;
  unsigned int K, N;
  FILE *filein;
  unsigned int *part;
  
  
  if(argc < 2){
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
  
  read_slap(filein, &K, &N, &J_slap, &r_slap);
  sort_neighbours(J_slap, r_slap, N);
  fclose(filein);
  
  part = girvan_newman(J_slap, r_slap, N);
  dump_partition(part, N);
  free(J_slap);
  free(r_slap);
  free(part);

}
