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
 *  Functions used to manage the special BST-augmented priority queue
 *  used in the Clauset-Newman-Moore greedy modularity optimisation
 *  algorithm.
 *
 */


#include <stdio.h>
#include <stdlib.h>

#include "cnm_bst_pq.h"


/* Prototypes for BST-related functions */

void* cnm_bst_alloc(){
  
  /* nothing to be done, actually*/
  return NULL; // malloc(sizeof(struct_neigh_t));
}

void cnm_bst_dealloc(void *info){
  
  /* Nothing to be done, actually */
  return;
}


int cnm_bst_compare(unsigned int e1, unsigned int e2){
  
  //struct_neigh_t *i1, *i2;

  /* i1 = (struct_neigh_t*)e1; */
  /* i2 = (struct_neigh_t*)e2; */

  //return (((struct_neigh_t*)e1)->neigh - ((struct_neigh_t*)e2)->neigh);
  return e1 - e2;
}

void cnm_bst_print(unsigned int elem, void *fout){
  fprintf(fout, " %d ", elem);
}


/* Prototypes for PQ-related functions */

int cnm_pq_compare(double e1, double e2){
  
  return (e1 - e2) > 0 ? 1 : (e1 - e2) < 0 ? -1 : 0;
}

void* cnm_pq_alloc_vector(unsigned int N){
  
  return malloc(N * sizeof(node_t*));
}

void cnm_pq_dealloc_vector(void *v){

  free (v);
}

void* cnm_pq_alloc_key(void){

  
  //return malloc(sizeof(struct_key_t));
  return NULL;
}

void cnm_pq_copy_key(void *src, void *dst){
  
  struct_key_t *k1 = (struct_key_t *)src;
  struct_key_t *k2 = (struct_key_t *)dst;

  k2->deltaQ = k1->deltaQ;
  
}

void cnm_pq_print_elem(void *e){
  
  node_t *n = (node_t*)e;
  printf("%g{%d [%d]}", n->key, n->info, n->index);
  
}


double cnm_pq_get_key(void *e){
  
  node_t *n;

  n = (node_t *)e;
  return n->key;
}

void cnm_pq_set_key(void *e, double k){
  
  node_t *n;

  n = (node_t *)e;
  n->key = k;
}

int cnm_pq_compare_to_key(void *e, double key){
  
  double d1;
  d1 = ((node_t*)e)->key;

  return (d1 - key) > 0 ? 1 : (d1 - key) < 0 ? -1 : 0;
} 



unsigned int cnm_get_id(node_t *n){

  return n->info;
}


double cnm_get_Q(node_t *n){

  if (n)
    return n->key;
  else
    return 0;
}
