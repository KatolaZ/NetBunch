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
 */


#include "edge_w_funs.h"
#include <stdio.h>
#include <stdlib.h>

int compare_edge_w(const void *e1, const void *e2){
  
  edge_w_t *t1, *t2;
  
  t1 = (edge_w_t*) e1;
  t2 = (edge_w_t*) e2;
  
  return (t1->w == t2->w? 0 : (t1->w < t2->w? -1 :1));
}


void* alloc_vector_edge_w(unsigned int N){
  
  return malloc(N * sizeof(edge_w_t*));
}

void dealloc_vector_edge_w(void *v){
  
  free(v);
}

void dealloc_elem_edge_w(void *e){
  
  free(e);
}

void print_elem_edge_w(void *e){
  
  edge_w_t *elem;

  elem = (edge_w_t*)e;
  
  printf("%d %d %2.8g\n", elem->i, elem->j, elem->w);
}


