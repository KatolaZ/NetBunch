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
 *  Definition of the functions to be used with the Priority Queue
 *
 */
#include <stdio.h>
#include <stdlib.h>

#include "gen_pqueue.h"
#include "node_w_funs.h"


int compare_node_w(const void *e1, const void *e2){

  node_w_t *n1, *n2;
  
  n1 = (node_w_t*) e1;
  n2 = (node_w_t*) e2;

  return (n1->w == n2->w ? 0 : (n1->w < n2->w ?  -1  : 1));
}


void* alloc_vector_node_w(unsigned int N){

  return malloc(N * sizeof(node_w_t));
}

void dealloc_vector_node_w(void *v){
  free(v);
}


void dealloc_elem_node_w(void *e){

  free(e);
}

void print_elem_node_w(void *e){

  node_w_t *elem;
  
  elem = (node_w_t*)e;
  
  printf("[%d, %2.8g]", elem->i, elem->w);
}

int get_id_node_w(void *e){
  
  node_w_t *elem;
  
  elem = (node_w_t*)e;
  return elem->i;
}

void set_key_node_w(void *e, void *k){

  node_w_t *elem;
  

  elem = (node_w_t*)e;
  elem-> w = *((double*)(k));
}

int compare_to_key_node_w(void *e, void *k){

  node_w_t *elem;
  double d;

  elem = (node_w_t*)e;
  d = *((double*)k);
  
  return (elem->w == d ? 0 : (elem->w < d ? -1 : 1));
}

void* get_key_node_w(void *e){
  
  node_w_t *n;

  n = (node_w_t*)e;
  return &(n->w);
}
