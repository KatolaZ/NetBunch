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

#ifndef __CNM_BST_PQ_H__
#define __CNM_BST_PQ_H__

#include "bst_pq.h"

/**
 *
 * Specific functions needed for the management of the BST_PQ used in
 * the implementation of CNM
 *
 * (c) Vincenzo Nicosia (KatolaZ) -- 2016
 */

/* data structure for the id, to be used by the BST */
typedef struct{
  int neigh; /* This is the label of a neighbour */
} struct_neigh_t;


/* data structure for the key, to be used by the PQ */
typedef struct{
  double deltaQ; /* This is the variation in modularity associated to
                    the join with neighbour contained in info */
} struct_key_t;


/* Prototypes for BST-related functions */

void* cnm_bst_alloc();

void cnm_bst_dealloc(void*);

//void cnm_bst_copy(unsigned int src, unsigned int dst);

int cnm_bst_compare(unsigned int, unsigned int );

void cnm_bst_print(unsigned int, void*);


/* Prototypes for PQ-related functions */

int cnm_pq_compare(double e1, double e2); 

void* cnm_pq_alloc_vector(unsigned int N); 

void cnm_pq_dealloc_vector(void *v); 

void* cnm_pq_alloc_key(void); 

void cnm_pq_copy_key(void *src, void *dst); 

void cnm_pq_print_elem(void *e); 

double cnm_pq_get_key(void *e);

void cnm_pq_set_key(void *e, double k); 

int cnm_pq_compare_to_key(void *e, double key); 

unsigned int cnm_get_id(node_t *);

double cnm_get_Q(node_t *);




#endif /*__CNM_BST_PQ_H__*/

