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
 *  Enumerate all the three-nodes subgraphs in a directed network, and
 *  compute the significance of their number with respect to the
 *  corresponding configuration model ensemble.
 * 
 *  References:
 *
 * [1] R. Milo et al. "Network Motifs: Simple Building Blocks of
 *     Complex Networks".  Science 298 (2002), 824-827.  
 * 
 * [2] R. Milo et al. "Superfamilies of evolved and designed
 *     networks." Science 303 (2004), 1538-1542
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
         "**                        -*-  f3m  -*-                           **\n"
         "**                                                                **\n"
         "**  Count all the 3-node subgraphs of a directed graph given as   **\n"
         "**  input, and compute the relevance (z-score) of each motif      **\n"
         "**  with respect to the corresponding configuration model graph   **\n"
         "**  ensemble.                                                     **\n"
         "**                                                                **\n"
         "**  The file 'graph_in' contains the edge list of the graph.      **\n"
         "**                                                                **\n"
         "**  The program prints on STDOUT one line for each of the 13      **\n"
         "**  motifs, in the format                                         **\n"
         "**                                                                **\n"
         "**      motif    count    mean_rnd     std_rnd     z-score        **\n"
         "**                                                                **\n"
         "**  where 'motif' is the motif number (an integer between 1 and   **\n"
         "**  13), 'count' is the number of subgraphs of that type found    **\n"
         "**  in 'graph_in', 'mean_rnd' is the average number of those      **\n"
         "**  subgraphs found in the randomised realisations of the graph,  **\n"
         "**  'std_rnd' is the standard deviation associated to 'avg_rnd',  **\n"
         "**  and 'z-score' is the normalised deviation of 'count' from     **\n"
         "**  'mean_rnd'.                                                   **\n"
         "**                                                                **\n"
         "**  If the (optional) parameter 'num_random' is provided, use     **\n"
         "**  that number of random realisations to compute the z-score.    **\n"
         "**                                                                **\n"
         "********************************************************************\n"
         " This is Free Software - You can use and distribute it under \n"
         " the terms of the GNU General Public License, version 3 or later\n\n"
         " Please visit http://www.complex-networks.net for more information\n\n"
         " (c) Vincenzo Nicosia 2009-2017 (v.nicosia@qmul.ac.uk)\n"
         "********************************************************************\n\n"
         );
  printf("Usage: %s <graph_in> [<num_random>]\n", argv[0]);
}






#define MIN(x, y) ((x) < (y) ? (x) : (y))


typedef struct{
  unsigned int N;
  unsigned int K;
  unsigned int *J_slap;
  unsigned int *r_slap;
} graph_t;


typedef struct{
  double f_count_real[13];
  int num_rnd;
  double **f_count_rnd;
} mstats_t;


char perm12[3][3] = {{0, 1, 0}, 
                    {1, 0, 0},
                    {0, 0, 1}};

char perm13[3][3] = {{0, 0, 1}, 
                    {0, 1, 0},
                    {1, 0, 0}};

char perm23[3][3] = {{1, 0, 0}, 
                    {0, 0, 1},
                    {0, 1, 0}};



void shuffle_list(unsigned int *v, unsigned int K){
  
  int i, pos;
  
  for(i=K-1; i>=0; i--){
    pos = rand() % K;
    if (pos != i){
      v[i] ^= v[pos];
      v[pos] ^= v[i];
      v[i] ^= v[pos];
    }
  }
}

int is_simple_graph(unsigned int *J_slap, unsigned int *r_slap, unsigned int K, 
                    unsigned int N){
  
  int i, j;
  for(i=0; i<N; i++){
    for(j=r_slap[i]; j<r_slap[i+1]; j++){
      if (J_slap[j] == i) /* If there is a self-loop....*/
        return 0;
      if (j > r_slap[i] && J_slap[j] == J_slap[j-1]) /* or a double edge... */
        return 0;
    }
  }
  return 1;
}

int is_loop_free(unsigned int *J_slap, unsigned int *r_slap, unsigned int K, 
                    unsigned int N){
  
  int i, j;
  for(i=0; i<N; i++){
    for(j=r_slap[i]; j<r_slap[i+1]; j++){
      if (J_slap[j] == i) /* There is a self-loop....*/
        return 0;
    }
  }
  return 1;
}



unsigned int* sample_conf_model_smart(unsigned int *J_slap, unsigned int *r_slap, 
                                      unsigned int K, unsigned int N){
  
  unsigned int *new_J;

  new_J = malloc( K * sizeof(unsigned int));
  
  memcpy(new_J, J_slap, K *sizeof(unsigned int));

  while(1){
    shuffle_list(new_J, K);
    sort_neighbours(new_J, r_slap, N);
    if(is_loop_free(new_J, r_slap, K, N))
      break;
  }
  return new_J;
}



void apply_perm_3(char m[3][3], char p[3][3]){

  char res[3][3];
  
  int i, j, k;
  for (i=0; i<3; i++){
    for(j=0; j<3; j++){
      res[i][j] = 0;
      for(k=0; k<3; k++){
        res[i][j] += p[i][k] * m[k][j];
      }
    }
  }

  for (i=0; i<3; i++){
    for(j=0; j<3; j++){
      m[i][j] = 0;
      for(k=0; k<3; k++){
        m[i][j] += res[i][k] * p[k][j];
      }
    }
  }

}

int row_value(char r[3]){ /* The "value" of a row of bits is
                                    equal to the binary representation
                                    of the row, in big-endian (i.e.,
                                    LSB is r[0], MSB is r[2])*/
  
  return r[0] + (r[1]<<1) + (r[2]<<2);
}

int matrix_value(char m[3][3]){ /* The value of a matrix of
                                          bits is equal to the binary
                                          representation of the
                                          matrix, in big endian,
                                          starting from the first row
                                          (LSB is m[0][0], MSB is
                                          m[2][2])*/
  
  return row_value(m[0]) + (row_value(m[1])<<3) + (row_value(m[2])<<6);
}


void permute_matrix_3(char m[3][3], int n1, int n2){
  
  int perm;

  if (n1 == n2){
    return;
  }
  if (n1 > n2){
    n1 ^= n2;
    n2 ^= n1;
    n1 ^= n2;
  }
  
  perm = n1 + (n2<<2);
  
  switch(perm){
  case (1 + (2<<2)): /* permute 1 with 2 */
    apply_perm_3(m, perm12);
    break;
  case (1 + (3<<2)): /* permute 1 with 3 */
    apply_perm_3(m, perm13);
    break;
  case (2 + (3<<2)): /* permute 2 with 3 */
    apply_perm_3(m, perm23);
    break;
  }
}



/* Load the input graph. We construct two versions of the graph,
   i.e. the directed versions G_out ( containing the list of
   out-neighbours of each node) and the underlying undirected graph
   G_u
   
   N.B.: This is quite inefficient at the moment, since it reads the
   file twice, and could be replaced by one call to read_ij and two
   appropriate calls to convert_ij2slap.... */

void load_graph(FILE *fin, graph_t *G_u, graph_t *G_out){

  /*FIXME!!!! WE CANNOT REWIND THE STANDARD OUTPUT !!!!! */
  read_slap(fin, &(G_u->K), &(G_u->N), &(G_u->J_slap), &(G_u->r_slap));
  sort_neighbours(G_u->J_slap, G_u->r_slap, G_u->N);
  rewind(fin);
  read_slap_dir(fin, &(G_out->K), &(G_out->N), &(G_out->J_slap), &(G_out->r_slap));
  sort_neighbours(G_out->J_slap, G_out->r_slap, G_out->N);
  rewind(fin);
  
}


void dump_matrix_3(char m[3][3]){
  
  int i, j;

  for(i=0; i<3; i++){
    for(j=0; j<3; j++){
      printf("%d ", m[i][j]);
    }
    printf("\n");
  }
}


int motif_number(char m[3][3]){
  
  char m0[3][3];
  char m1[3][3];
  char m2[3][3];
  char m3[3][3];
  
  int v, v0, v1, v2, v3, v4, v5;
  int i,j;
  
  for(i=0; i<3; i++){
    for(j=0; j<3; j++){
      m0[i][j] = m[i][j];
    }
  }
  
  if (row_value(m[0]) == 0){
    permute_matrix_3(m0, 1, 2);
  }
  if (row_value(m0[1]) == 0){
    permute_matrix_3(m0, 2, 3);
  }
  
  
  for(i=0; i<3; i++){
    for(j=0; j<3; j++){
      m1[i][j] = m0[i][j];
      m2[i][j] = m0[i][j];
      m3[i][j] = m0[i][j];
    }
  }
  
  /* We consider here all the 6 possible permutations... */
  
  /* {0, 1, 2} */
  v0 = matrix_value(m0);
  /* {1, 0, 2} */
  permute_matrix_3(m1, 1, 2);
  v1 = matrix_value(m1);
  /* {2, 1, 0} */
  permute_matrix_3(m2, 1, 3);
  v2 = matrix_value(m2);
  /* {0, 2, 1} */
  permute_matrix_3(m3, 2, 3);
  v3 = matrix_value(m3);
  /* {1, 2, 0} */
  permute_matrix_3(m2, 1, 2);
  v4 = matrix_value(m2);
  /* {2, 0, 1} */
  permute_matrix_3(m3, 1, 2);
  v5 = matrix_value(m3);
  
  v = MIN (MIN( MIN( MIN( MIN( v0, v1), v2), v3), v4), v5);
  
  switch(v){
  case 6:
    return 0;
  case 12:
    return 1;
  case 14:
    return 2;
  case 36:
    return 3;
  case 38:
    return 4;
  case 46:
    return 5;
  case 74:
    return 6;
  case 78:
    return 7;
  case 98:
    return 8;
  case 102:
    return 9;
  case 108:
    return 10;
  case 110:
    return 11;
  case 238:
    return 12;
  default:
    fprintf(stderr, "No motif with number %d! Exiting\n", v);
    dump_matrix_3(m);
    exit(5);
  }
}

int get_motif_3(int n1, int n2, int n3, graph_t *G_out){
  
  char m[3][3];
  unsigned int n[3] = {n1, n2, n3};
  
  int i, j, v;
  
  for(i=0; i<3; i++){
    for (j=0; j<3; j++){
      if (is_neigh(G_out->J_slap, G_out->r_slap, G_out->N, 
                      n[i], n[j])){
        m[i][j] = 1;
      }
      else{
        m[i][j] = 0;
      }
    }
  }
  v = motif_number(m);
  return v;
}



void find_subgraphs_3(graph_t *G_u, graph_t *G_out, double *f_cnt){
  
  int i, j, k, n1, n2;
  int val;

  for (i=0; i<G_u->N; i++){
    for(n1 = G_u->r_slap[i]; n1<G_u->r_slap[i+1]; n1++){
      /* j is a first-neighbour of i in G_u */
      j = G_u->J_slap[n1];
      /* avoid multiple entries in the J_slap vector */
      if (n1 > G_u->r_slap[i] && j == G_u->J_slap[n1-1])
        continue;
      for(n2 = n1+1; n2 < G_u->r_slap[i+1]; n2++){
        /* and k is another first neighbour of i in G_u */
        k = G_u->J_slap[n2];
      /* avoid multiple entries in the J_slap vector */
        if (n2 > n1+1 && k == G_u->J_slap[n2-1])
          continue;
        /* now, if j and k are connected by an edge, we consider this
           triangle only if i<j<k (in order to avoid multiple counts).
           Otherwise, if i-j-k is an open triad, we have to consider
           it now, because there is no other possibility to discover
           it */
        if((is_neigh(G_u->J_slap, G_u->r_slap, G_u->N, j, k) && 
            (j < i || k < j || k < i)) || (j==k))
          continue;
        val = get_motif_3(i, j, k, G_out);
        f_cnt[val] +=1;
      }
    }
  }
  
}

void init_graph(graph_t *G1){
  G1->J_slap = G1->r_slap = NULL;
}

void init_stats(mstats_t *st, int n_rand){
  int i;

  st->f_count_rnd = malloc(n_rand * sizeof(double*));
  
  st->num_rnd = n_rand;

  for(i=0; i<13; i++){
    st->f_count_real[i] = 0;
  }
  
  for(i=0; i<n_rand; i++){
    st->f_count_rnd[i] = malloc(13 * sizeof(double));
    memset(st->f_count_rnd[i], 0, 13 * sizeof(double));
  }
}


void compute_rnd_st_mean_std(mstats_t *st, double *mean, double *std){
  
  double sum[13], sum2[13];
  double val, n;

  int i, j;

  
  n = st->num_rnd;

  for (i=0; i<13; i++){
    sum[i] = sum2[i] = 0;
  }

  if (n == 0)
    return;
  
  for(i=0; i<n; i++){
    for(j=0; j<13; j++){
      val = st->f_count_rnd[i][j];
      sum[j] += val;
      sum2[j] += val*val;
    }
  }
  
  for(i=0; i<13; i++){
    mean[i] = sum[i] / n;
    if (sum2[i] > 0)
      std[i] = sqrt(sum2[i] * 1.0/(n-1) - 1.0/( n * (n-1)) * sum[i]*sum[i]);
    else
      std[i] = 0.0;
  }
}



 

void dump_stats(mstats_t *st){
  
  int i;
  double v_mean[13], v_std[13], x;

  memset(v_mean, 0, 13 * sizeof(double));
  memset(v_std, 0, 13 * sizeof(double));
  
  compute_rnd_st_mean_std(st, v_mean, v_std);
  for(i=0; i<13; i++){
    x = st->f_count_real[i];
    if (v_std[i] > 0)
      printf("%-2d %12.0f %15.2f %10.3f %+10.3f\n", i+1, x, 
             v_mean[i], v_std[i], 1.0 * (x - v_mean[i])/v_std[i] );
    else
      printf("%-2d %12.0f %15.2f %10.3f %+10.3f\n", i+1, x, 
             v_mean[i], v_std[i], 0.0);
    
  }
}

void randomise_graph(graph_t *G_out, graph_t *RNDG_out, graph_t *RNDG_u){
  
  static unsigned int *I, *J; 
  static unsigned int I_size, J_size;
  unsigned int *tmp;

  if (!I || I_size < 2*G_out->K){
    tmp = realloc(I, G_out -> K * 2 * sizeof(unsigned int)); 
    VALID_PTR_OR_EXIT(tmp, 3);
    I = tmp;
    I_size = 2*G_out->K;
  }


  if (!J || J_size < 2*G_out->K){
    tmp = realloc(J, G_out -> K * 2 * sizeof(unsigned int)); 
    VALID_PTR_OR_EXIT(tmp, 3);
    J = tmp;
    J_size = 2*G_out->K;
  }
  
  if (RNDG_out->J_slap){
    free(RNDG_out->J_slap);
    RNDG_out->J_slap = NULL;
  }
  
  
  RNDG_out->J_slap = sample_conf_model_smart(G_out->J_slap, G_out->r_slap, G_out->K, G_out->N);

  tmp = realloc(RNDG_out->r_slap, (G_out->N + 1) * sizeof(unsigned int));
  VALID_PTR_OR_EXIT(tmp, 19);
  RNDG_out->r_slap = tmp;
  memcpy(RNDG_out->r_slap, G_out->r_slap, (G_out->N + 1) * sizeof(unsigned int));
  RNDG_out->N = G_out->N;
  RNDG_out->K = G_out->K;


  convert_slap2ij(RNDG_out->J_slap, RNDG_out->r_slap, RNDG_out->N, I, J);
  
  /* copy J at the end of I */
  memcpy(&(I[G_out->K]), J, G_out->K * sizeof(unsigned int));
  /* copy I at the end of J */
  memcpy(&(J[G_out->K]), I, G_out->K * sizeof(unsigned int));


  RNDG_u->N = convert_ij2slap(I, J, 2*G_out->K, & (RNDG_u->r_slap), &(RNDG_u->J_slap));
  
  RNDG_u->K = 2 * G_out->K;

  sort_neighbours(RNDG_u->J_slap, RNDG_u->r_slap, RNDG_u->N);
  sort_neighbours(RNDG_out->J_slap, RNDG_out->r_slap, RNDG_out->N);
  
  if (!is_loop_free(RNDG_u->J_slap, RNDG_u->r_slap, RNDG_u->K, RNDG_u->N)){
    fprintf(stderr, "Error!!!! The undirected version of the graph is not loop-free!!!!\n");
    exit(23);
  }
  
}


int main(int argc, char *argv[]){

  graph_t G_u, G_out, RNDG_u, RNDG_out;
  mstats_t st;
  FILE *filein;
  unsigned int nr;
  
  int i;
  
  if(argc < 2){
    usage(argv);
    exit(1);
  }
  filein = openfile_or_exit(argv[1], "r", 2);

  if (argc > 2){
    nr = atoi(argv[2]);
  }
  else{
    nr = 0;
  }
  
  init_stats(&st, nr);
  init_graph(&G_u);
  init_graph(&G_out);
  
  load_graph(filein, &G_u, &G_out);

  fclose(filein);
  
  find_subgraphs_3(&G_u, &G_out, st.f_count_real);

  srand(time(NULL));
  
  /* Now we create n_r random networks with the same degree
     distribution, and we perform motifs analysis on each of them */

  init_graph(&RNDG_out);
  init_graph(&RNDG_u);
  
  for(i=0; i<nr; i++){
    /* Create the random graph */
    randomise_graph(&G_out, &RNDG_out, &RNDG_u);
    /* call find_subgraphs_3 in it */
    find_subgraphs_3(&RNDG_u, &RNDG_out, st.f_count_rnd[i]);
    //show_progress(stderr, "Randomised networks: ", i+1, nr);
  }
  //fprintf(stderr,"\n");
  
  /* Now we should print the results on output  */
  
  dump_stats(&st);

  free(G_u.J_slap);
  free(G_u.r_slap);
  free(G_out.J_slap);
  free(G_out.r_slap);

  free(RNDG_u.J_slap);
  free(RNDG_u.r_slap);
  free(RNDG_out.J_slap);
  free(RNDG_out.r_slap);

  return 0;
}
