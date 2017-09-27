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
 *  This is an implementation of a Binary Search Tree augmented by a
 *  Priority Queue.  This data structure is central to the
 *  implementation of the Clauset-Newman-Moore greedy modularity
 *  optimisation algorithm
 *
 */

#ifndef __BST_PQ_H__
#define __BST_PQ_H__

#include "gen_stack.h"


/**
 *
 * This is an implementation of a Binary Search Tree augmented by a
 * Priority Queue. 
 *
 */

#define PARENT(i) (int)(floor((i-1)/2))

#define MIN_QUEUE 0x01
#define MAX_QUEUE 0x02

typedef struct node{
  unsigned int info; /* This is the ID of the neighbour used in  the BST */
  double key; /* This is the \DeltaQ values used as a key in the
                    PQ */
  struct node* left; /* left child of the current element in the BST */
  struct node* right; /* right child of the current element in the BST */
  int index; /* Index of the current node in the PQ */
  struct node *parent; /* Parent of the current node in the BST */
  char active; /* indicates whether the node is active or not */
} node_t;


typedef struct{
  void* (*alloc)();
  void (*dealloc)(void*);
  //void (*copy)(void *src, void *dst);
  int (*compare)(unsigned int e1, unsigned int e2);
  void (*print)(unsigned int, void*);
  void *fileout;
} ilfunc_t;


typedef struct{
  int (*compare)(double e1, double e2); /* compare two elements (standard comparator) */
  void* (*alloc_vector)(unsigned int N); /* */
  void (*dealloc_vector)(void *v); /* */
  void* (*alloc_key)(void); /* Allocate an element for the key */
  void (*copy_key)(void *src, void *dst); /* copy into key */
  void (*print_elem)(void *e); /* print an element */
  void (*set_key)(void *e, double k); /* set a new key to element e */
  int (*compare_to_key)(void *e, double key); /* compare a key with the one of element e */
} gen_pqueue_func_t;


typedef struct {
  node_t* root; /* root of the BST */
  int N; /* Maximum size of the PQ */
  int last; /* last element in the PQ */
  node_t **v; /* vector of pointers to the nodes of the BST */
  char qtype; /* type of PQ, either MIN_QUEUE or MAX_QUEUE */
  ilfunc_t bst_funs;
  gen_pqueue_func_t pq_funs;
  gen_stack_t *node_cache; /* this is a pointer to a cache of
                              allocated nodes, implemented as a stack */
} bst_pq_struct_t;


typedef bst_pq_struct_t* bst_pq_t;





/* bst_pq interface */

/* init the BST_PQ */

bst_pq_t bst_pq_create(ilfunc_t* bst_funs, gen_pqueue_func_t* pq_funs, char qtype, 
                       unsigned int N, gen_stack_t *node_cache);

/* Destroy an existing BST_PQ */
void bst_pq_destroy(bst_pq_t b, char clear_cache);

/* lookup an element in the BST */
node_t* bst_pq_lookup(bst_pq_t b, unsigned int info);

/* insert a new element in the BST_PQ */
void bst_pq_insert(bst_pq_t b, unsigned int info, double key);

/* delete (or deactivate) an element in the BST_PQ */
int bst_pq_delete(bst_pq_t b, unsigned int info);

/* change the key of an element in the BST_PQ */
int bst_pq_change_key(bst_pq_t b, unsigned int info, double key);

/* return the head of the PQ */
void* bst_pq_peek(bst_pq_t);

/* Read the key of a given node of the BST */
double bst_pq_get_key(bst_pq_t b, unsigned int info);

/* Read the key of the element of index "index" in the PQ */
double bst_pq_get_key_from_index(bst_pq_t b, int index);

/* Dump the BST  */
void bst_pq_dump_bst(bst_pq_t b);

/* Show the PQ */
void bst_pq_dump_pq(bst_pq_t b);

/* See if a node is in the BST, either active or inactive */
node_t* bst_pq_lookup(bst_pq_t b, unsigned int info);

/* See if a node is in the BST and is ACTIVE */
node_t* bst_pq_lookup_active(bst_pq_t b, unsigned int info);


/* Insert a pre-allocated node_t in the BST_PQ */
//void bst_pq_insert_existing(bst_pq_t b, node_t *n);



#endif /*__BST_PQ_H__*/
