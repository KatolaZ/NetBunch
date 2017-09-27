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
 *  Functions needed to manage the BST.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "knntree.h"


void* __alloc_knnsum(){
  knnsum_t *t;
  
  t = malloc(sizeof(knnsum_t));
  return  t;
}

void __dealloc_knnsum(void *elem){

  free(elem);
  
}

void __copy_knnsum(void *elem1, void *elem2){
  *((knnsum_t*)elem2) = *((knnsum_t*)elem1); 
}

int __compare_knnsum(void *elem1, void *elem2){

  knnsum_t *t1, *t2;

  t1 = (knnsum_t*)elem1;
  t2 = (knnsum_t*)elem2;

  return (t1->k < t2->k ? -1 : (t1->k > t2->k ? 1 : 0));
}

void __print_knnsum(void *elem, void *fileout){

  knnsum_t *t;
  
  t = (knnsum_t*)elem;
  
  fprintf((FILE*)fileout, "%d %2.8g\n", t->k, t->knnsum / (t->k * t->Nk));
}


knntree_t knntree_init(knntree_t t, void *fileout){
  
  ilfunc_t funs= {
    .alloc = __alloc_knnsum,
    .dealloc = __dealloc_knnsum, 
    .copy = __copy_knnsum,
    .compare = __compare_knnsum, 
    .print = __print_knnsum,
    .fileout = fileout
  };
  
  t = iltree_create((iltree_t)t);
  iltree_set_funs((iltree_t)t, &funs);
  return t;
}


