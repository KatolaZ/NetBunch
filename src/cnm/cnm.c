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
 *  This program finds the communities in a graph using the greedy
 *  modularity optimisation algorithm proposed by Clauset, Newman, and
 *  Moore.
 *
 *  References:
 * 
 *  [1] A. Clauset, M. E. J. Newman, and C. Moore. "Finding community
 *      structure in very large networks". Phys. Rev. E 70 (2004),
 *      066111.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "utils.h"
#include "cnm_bst_pq.h"
#include "dset.h"


void usage(char *argv[]){

  printf("********************************************************************\n"
         "**                                                                **\n"
         "**                         -*-  cnm  -*-                          **\n"
         "**                                                                **\n"
         "**   Find the communities of the input graph 'graph_in' using     **\n"
         "**   the greedy modularity optimisation algorithm proposed by     **\n"
         "**   Clauset, Newman, and Moore.                                  **\n"
         "**                                                                **\n"
         "**   The input file 'graph_in' is an edge-list.                   **\n"
         "**   If 'graph_in' is equal to '-' (dash), read the file from     **\n" 
         "**   the standard input (STDIN).                                  **\n"
         "**                                                                **\n"
         "**   The program prints on STDOUT the partition corresponding     **\n"
         "**   to the largest value of modularity, in the format:           **\n"
         "**                                                                **\n"
         "**        node_1 comm_1                                           **\n"
         "**        node_2 comm_2                                           **\n"
         "**        node_3 comm_3                                           **\n"
         "**       .....                                                    **\n"
         "**                                                                **\n"
         "**   where 'comm_1' is the community to which 'node_1' belongs.   **\n"
         "**                                                                **\n"
         "**   The program prints on STDERR the number of communities and   **\n"
         "**   the value of modularity obtained at each step, in the        **\n"
         "**   format:                                                      **\n"
         "**                                                                **\n"
         "**        ## nc: NUM_COMM Q_max: Q_MAX                            **\n"
         "**        nc_1 Q_1                                                **\n"
         "**        nc_2 Q_2                                                **\n"
         "**        nc_3 Q_3                                                **\n"
         "**        ...                                                     **\n"
         "**                                                                **\n"
         "**   where 'nc_1', 'nc_2', 'nc_3', etc. is the number of          **\n"
         "**   communities at the 1st, 2nd, 3rd step etc., and 'Q_1',       **\n"
         "**   'Q_2', 'Q_3', etc. are the value of the modularity           **\n"
         "**   function of the corresponding node partition. The first      **\n"
         "**   output line reports the number of communities NUM_COMM       **\n"
         "**   and corresponding value of modularity Q_MAX of the best      **\n"
         "**   partition found.                                             **\n"
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


/* shuffle a vector in-place */
void shuffle_vector_ptr(unsigned int **v, unsigned int N){
  
  int i, pos;
  void *tmp;

  for(i=N-1; i>=0; i--){
    pos = rand() % N;
    if (pos != i){
      tmp = v[i];
      v[i] = v[pos];
      v[pos] = tmp;
    }
  }
}




/* initialise BST-related functions */
void set_bst_funs(ilfunc_t *f){

  f->alloc = cnm_bst_alloc;
  f->dealloc = cnm_bst_dealloc;
  f->compare = cnm_bst_compare;
  f->print = cnm_bst_print;

}

/* Initialise priority-queue-related functions */
void set_pq_funs(gen_pqueue_func_t *f){
  
  f->compare = cnm_pq_compare;
  f->alloc_vector = cnm_pq_alloc_vector;
  f->dealloc_vector = cnm_pq_dealloc_vector;
  f->alloc_key = cnm_pq_alloc_key;
  f->copy_key = cnm_pq_copy_key;
  f->print_elem = cnm_pq_print_elem;
  f->set_key = cnm_pq_set_key;
  f->compare_to_key = cnm_pq_compare_to_key;
}



void init_bsts(unsigned int *J_slap, unsigned int *r_slap, unsigned int N, 
               bst_pq_t *b, bst_pq_t *H, gen_stack_t *node_cache){
  
  unsigned int i, j, n, m, deg_i, deg_j;
  //struct_key_t dQ;
  double dQ;
  //struct_neigh_t ith, jth;
  unsigned int K;
  node_t *node_ptr;

  ilfunc_t bst_funs;
  gen_pqueue_func_t pq_funs;
  unsigned int *nodes;

  set_bst_funs(&bst_funs);
  set_pq_funs(&pq_funs);

  K = r_slap[N]/2;
  *H = bst_pq_create(&bst_funs, &pq_funs, MAX_QUEUE, N, node_cache);
  nodes = malloc(N * sizeof(unsigned int));
  for (i=0; i<N; i++){
    nodes[i] = i;
  }
  shuffle_vector(nodes, N);

  for(n=0; n<N; n++){
    i = nodes[n];
    //ith.neigh = i;
    deg_i = r_slap[i+1] -  r_slap[i];
    if (deg_i == 0){
      b[i] = NULL;
      continue;
    }
    /* create the BST_PQ for the current node i */
    b[i] = bst_pq_create(&bst_funs, &pq_funs, MAX_QUEUE, deg_i, node_cache);
    /* Let's shuffle the ids of the neighbours of i, to have a somehow
     balanced BST on average  */
    shuffle_vector(J_slap + r_slap[i], deg_i);
    /* Now we insert all the neighbours of i into b[i] */
    for(m = r_slap[i]; m<r_slap[i+1]; m++){
      j = J_slap[m];
      if(i == j)
        continue;
      deg_j = r_slap[j + 1] - r_slap[j];
      dQ = 2.0 * (1.0 / (2.0*K) - 1.0 * (deg_i * deg_j)/ (4.0 * K * K));
      bst_pq_insert(b[i], j, dQ);
    }
    /* then we get the maximum value of modularity in the neighbourhood of i */
    node_ptr = bst_pq_peek(b[i]);
    if (node_ptr){
      dQ = cnm_get_Q(node_ptr);
      /* and insert it into H */
      bst_pq_insert(*H, i, dQ);
    }
  }
  free(nodes);
}


dset_t*  cnm(unsigned int *J_slap, unsigned int *r_slap, unsigned int N,
         bst_pq_t *b, bst_pq_t H){
  
  double *a;
  double Q;
  double dQ_ik, dQ_jk, corr;
  double dQ;
  //struct_neigh_t ith, jth, kth;
  int i, j, k;
  node_t *best, *merge, *tmp;
  unsigned int K;
  node_t **neighs; /* pointers to the neighbours of a node */
  int num_neighs;
  double Q_max;
  int l, m, N_max;
  double Q_step;
  char found_max;

  dset_t *comms;
  
  Q = 0;
  K = r_slap[N]/2;
  
  
  a = malloc(N * sizeof(double));
  neighs = malloc(N * sizeof(node_t*));
  
  comms = malloc(N * sizeof(dset_t));
  
  for (l=0; l<N; l++){
    a[l] = 1.0 * (r_slap[l+1] - r_slap[l]) / (2.0 * K);
    Q -= a[l] * a[l];
    comms[l] = NULL;
    dset_makeset_id(comms + l, l);
  }

  Q_max = Q;
  N_max = N;
  found_max = 0;

  for(l=0; l<N; l++){
    fprintf(stderr, "%d %g\n", N-l, Q);
    /* Get the maximum from H and remove the element */
    best = bst_pq_peek(H);
    if (!best)
      break;
    j = cnm_get_id(best);
    
    //Q_step = cnm_get_Q(best);
    merge = bst_pq_peek(b[j]);
    if (!merge){
      /* the node j does not have any neighbour, indeed... discard it
         and continue */
      bst_pq_delete(H, j);
       continue;
    }

    i = cnm_get_id(merge);
    /* So, we will merge community i into community j */
    if (i == j){
      /* This should never happen */
      fprintf(stderr, "Error!!!! i and j are the same node!!!!\n");
      exit(1);
    }

    Q_step = cnm_get_Q(best);
    if (Q_step < 0){
      found_max = 1;
    }
    Q += Q_step;


    /* Let's go to the neighbours of i */
    memcpy(neighs, b[i]->v, (b[i]->last + 1) * sizeof(node_t *));
    num_neighs = b[i]->last + 1;
    shuffle_vector_ptr((void*)neighs, num_neighs);

    for(m=0; m<num_neighs; m++){
      /* let's call this neighbour k */
      k = cnm_get_id(neighs[m]);

      if (k == j)
        continue;
      /* let's check whether k is also a neighbour of j already */
      tmp = bst_pq_lookup_active(b[j], k);
      if (tmp){ /* this is a closed triangle k-i-j */
        dQ_ik = cnm_get_Q(neighs[m]); /* this is i-k*/
        dQ_jk = cnm_get_Q(tmp); /* this is j-k */

        dQ = dQ_ik + dQ_jk;
        
        /* now we update Q_jk and Q_kj */
        if (bst_pq_change_key(b[j], k, dQ)){
          fprintf(stderr, "error changing key %d into %d-th tree!!! (%s: %d)\n", 
                  k, j, __FILE__, __LINE__);
          exit(1);
        }
        if (bst_pq_change_key(b[k], j, dQ)){
          fprintf(stderr, "error changing key %d into %d-th tree!!! (%s: %d)\n", 
                  j, k, __FILE__, __LINE__);
          exit(1);
        }
        bst_pq_delete(b[k], i);
      }
      else{ /* this is a chain k-i-j */
        dQ_ik = cnm_get_Q(neighs[m]);
        corr = -2.0 * a[j] * a[k];
        dQ = dQ_ik + corr;

        /* we add Q_{jk} which did not exist before */
        bst_pq_insert(b[j], k, dQ);
        /* we add Q_{kj} which did not exist before */
        bst_pq_insert(b[k], j, dQ);
        bst_pq_delete(b[k], i);
      }
      
      /* now we update the maximum value associated to k in H */
      tmp = bst_pq_peek(b[k]);
      if (tmp){
        dQ = cnm_get_Q(tmp);
        bst_pq_change_key(H, k, dQ);
      }
    }

    /* We delete the value associated to i in j */
    bst_pq_delete(b[j], i);
    
    
    /* Let's start with the neighbours of j */
    memcpy(neighs, b[j]->v, (b[j]->last + 1) * sizeof(node_t *));
    num_neighs = b[j]->last + 1;
    shuffle_vector_ptr((void*)neighs, num_neighs);
    
    for(m=0; m<num_neighs; m++){
      k = cnm_get_id(neighs[m]);
      if (k == i)
        continue;
      /* let's check whether k is also a neighbour of i */
      tmp = bst_pq_lookup_active(b[i], k);
      if(! tmp){ /* this is a chain i-j-k */
        dQ_jk = cnm_get_Q(neighs[m]);
        corr = -2.0 * a[i] * a[k];
        dQ = dQ_jk + corr;
       
        /* update Q_jk and Q_kj */
        if (bst_pq_change_key(b[j], k, dQ)){
          fprintf(stderr, "error changing key %d into %d-th tree!!! (%s: %d)\n", 
                  k, j, __FILE__, __LINE__);
          exit(1);

        }
        if (bst_pq_change_key(b[k], j, dQ)){
          fprintf(stderr, "error changing key %d into %d-th tree!!! (%s: %d)\n", 
                  j, k, __FILE__, __LINE__);
          exit(1);
        }
      }

      /* now we update the value associated to k in H */
      tmp = bst_pq_peek(b[k]);
      if (tmp){
        dQ = cnm_get_Q(tmp);
        bst_pq_change_key(H, k, dQ);
      }
    }
    
    /*  We can now free the BST_PQ associated to node i, since it will
       not be used ever again...*/
    bst_pq_destroy(b[i], 0);
    b[i] = NULL;

    /* We can now remove the value associated to i in H */
    bst_pq_delete(H, i);
    
    /* OK, now we should update the value associated to j in H */
    tmp = bst_pq_peek(b[j]);
    if (tmp){
      dQ = cnm_get_Q(tmp);
      bst_pq_change_key(H, j, dQ);
    }
    a[j]+= a[i];
    a[i] = 0;
    if (! found_max)
      dset_union_opt(comms[j], comms[i]);
    
    if (Q>Q_max){
      Q_max = Q;
      N_max = N-l-1;
    }
  }
  fprintf(stdout, "### nc: %d Q_max: %g\n", N_max, Q_max);
  free(a);
  free(neighs);

  return comms;
}


void dump_communities(dset_t *comms, unsigned int N){
  
  int i;

  for(i=0; i<N; i++){
    fprintf(stdout, "%d %d\n", i, dset_find_id_opt(comms[i]));
  }
}


int main(int argc, char *argv[]){

  unsigned int *J_slap = NULL, *r_slap = NULL;
  unsigned int N, K;
  bst_pq_t *b = NULL;
  bst_pq_t H = NULL;
  dset_t *comms;
  int i;
  
  gen_stack_t *node_cache = NULL;
  
  FILE *filein;
  
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

  b = malloc(N * sizeof(bst_pq_t));
  
  node_cache = malloc(sizeof(gen_stack_t));
  gen_stack_create(node_cache);

  
  init_bsts(J_slap, r_slap, N, b, &H, node_cache);


  comms = cnm(J_slap, r_slap, N, b, H);
  dump_communities(comms, N);

  free(J_slap);
  free(r_slap);
  for(i=0; i<N; i++){
    if(b[i]){
      bst_pq_destroy(b[i], 0);
    }
    if (comms[i])
      free(comms[i]);
  }
  free(b);
  bst_pq_destroy(H, 1);
  free(comms);
  fclose(filein);
  return 0;
}



