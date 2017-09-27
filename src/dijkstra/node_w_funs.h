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
 *  Declaration of the functions to be used with the Priority Queue
 *
 */
#ifndef __NODE_W_FUNS_H__
#define __NODE_W_FUNS_H__



typedef struct{
  int i;
  double w;
} node_w_t;


int compare_node_w(const void *e1, const void *e2);

void* alloc_vector_node_w(unsigned int N);

void dealloc_vector_node_w(void *v);

void dealloc_elem_node_w(void *e);

void print_elem_node_w(void *e);

int get_id_node_w(void *e);

void set_key_node_w(void *e, void *k);

void* get_key_node_w(void *e);

int compare_to_key_node_w(void *e, void *k);



#endif //__NODE_W_FUNS_H__
