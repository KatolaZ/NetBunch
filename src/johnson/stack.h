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
 *  Implementation of a stack data structure (for unsigned integers)
 *
 */

#ifndef __MY_STACK_H__
#define __MY_STACK_H__

typedef struct{
  unsigned int size;
  int head;
  unsigned int *v;
} my_stack_t;


void stack_create(my_stack_t *s);

void stack_push(my_stack_t *s, unsigned int n);

int stack_pop(my_stack_t *s, unsigned int *res);

int stack_empty(my_stack_t *s);

void stack_dump(my_stack_t *s, FILE *f);

int stack_size(my_stack_t *s);


#endif //__MY_STACK_H__
