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


#ifndef __GEN_HEAP__
#define __GEN_HEAP__


#define MAX_HEAP  0x01
#define MIN_HEAP  0x02

#define SORT_ASC  0x04
#define SORT_DESC 0x08

#define ERR_DELETE -0x04

typedef struct{
  int (*compare)(const void *e1, const void *e2);
  void* (*alloc_vector)(unsigned int N);
  void (*dealloc_vector)(void *v);
  void (*dealloc_elem)(void *e);
  void (*print_elem)(void *e);
} gen_heap_func_t;


typedef struct{
  int N;
  int last;
  void **v;
  gen_heap_func_t funs;
  char htype;
} gen_heap_t;


#define PARENT(i) (int)(floor((i-1)/2))

gen_heap_t * gen_heap_init(unsigned int N, char htype, gen_heap_func_t *funs);

void gen_heap_insert(gen_heap_t *h, void *elem);

int gen_heap_delete(gen_heap_t *h, void **val);

void* gen_heap_peek(gen_heap_t *h);

gen_heap_t* gen_heap_from_array(void **v, unsigned int N, unsigned int last, char htype, 
                                gen_heap_func_t *funs);

void gen_heap_dump(gen_heap_t *h);

void gen_heap_destroy(gen_heap_t *h);

int gen_heap_sort(void *v, unsigned int N, size_t size, 
                  char dir, int (*compar)(const void*, const void*));

#endif // __GEN_HEAP__
