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
 *  Implementation of the iltree data structure with keys of type
 *  "long double"
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "iltree_double.h"


void* alloc_double(){
  return malloc(sizeof(long double));
}

void dealloc_double(void *elem){
  free(elem);
}

void copy_double(void *elem1, void *elem2){
  *((long double*)elem2) = *((long double*)elem1);
}


int compare_long_double(void *elem1, void *elem2){
  
  long double *l1, *l2;

  l1 = (long double*)elem1;
  l2 = (long double*)elem2;

  return (*l1 < *l2 ? -1 : (*l1 > *l2 ? 1 : 0));
  //return *((long double*)elem1) - *((long double*)elem2);
}

void print_long_double(void *elem, void *fileout){
  
  long double k, i, j;
  long double x;

  k = *((long double*)elem);

  x = (1 + sqrtl(1 + 8 * (k-1))) / 2;
  i = floorl(x) + 1;
  j = k - ( (i-1)*1.0 * (i-2) ) /2;
  //printf("x: %Lf\n i: %0.0Lf j: %0.0Lf\n", x, i, j);
  fprintf((FILE*)fileout, "%d %d\n", (unsigned int)(i-1), (unsigned int)(j-1));
}

iltree_t iltree_double_init(iltree_t t, void *fileout){
  
  ilfunc_t funs= {
    .alloc = alloc_double,
    .dealloc = dealloc_double, 
    .copy = copy_double,
    .compare = compare_long_double, 
    .print = print_long_double,
    .fileout = fileout
  };
  
  t = iltree_create(t);
  iltree_set_funs(t, &funs);
  return t;
}


void iltree_double_dump_edges(iltree_t t){

  iltree_view_pre(t);
}
