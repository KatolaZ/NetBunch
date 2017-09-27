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


#ifndef __GEN_PQUEUE_H__
#define __GEN_PQUEUE_H__

#include "gen_heap.h"

#define KEY_ERROR -1
#define ID_ERROR  -2

/*
 * 
 * This implementation of the priority queue is based on gen_heap, but
 * requires some additional code to work, since we need to maintain an
 * array of "handles", i.e. a mapping between elements and their
 * current positions in the heap
 *
 */

typedef struct{
  int (*compare)(const void *e1, const void *e2); /* compare two elements (standard comparator) */
  void* (*alloc_vector)(unsigned int N); /* */
  void (*dealloc_vector)(void *v); /* */
  void (*dealloc_elem)(void *e); /* deallocate an element */
  void (*print_elem)(void *e); /* print an element */
  int  (*get_id)(void *e); /* get the id associated to an element
                              (used for handles management) */
  void* (*get_key)(void *e);/* get the current key of element e */
  void (*set_key)(void *e, void *k); /* set a new key to element e */
  int (*compare_to_key)(void *e, void *key); /* compare a key with the one of element e */
} gen_pqueue_func_t;



typedef struct{
  int N;
  int last;
  void **v;
  gen_pqueue_func_t funs;
  char qtype;
  int *handles;
} gen_pqueue_t;



#define MIN_QUEUE 0x01
#define MAX_QUEUE 0x02



gen_pqueue_t * gen_pqueue_init(unsigned int N, char htype, gen_pqueue_func_t *funs);

void gen_pqueue_insert(gen_pqueue_t *h, void *elem);

int gen_pqueue_delete(gen_pqueue_t *h, void **val);

void* gen_pqueue_peek(gen_pqueue_t *h);

gen_pqueue_t* gen_pqueue_from_array(void **v, unsigned int N, unsigned int last, char htype, 
                                gen_pqueue_func_t *funs);

int gen_pqueue_change_key(gen_pqueue_t *q, unsigned int i, void *key);

int gen_pqueue_force_key(gen_pqueue_t *q, unsigned int idx, void *key);

void gen_pqueue_dump(gen_pqueue_t *h);

void gen_pqueue_destroy(gen_pqueue_t *h);

int gen_pqueue_get_handle(gen_pqueue_t *q, int id);

void* gen_pqueue_get_key(gen_pqueue_t *q, int idx);




#endif // __GEN_PQUEUE_H__
