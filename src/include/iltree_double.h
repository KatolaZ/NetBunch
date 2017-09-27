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

#ifndef __ILTREE_DOUBLE_H__
#define __ILTREE_DOUBLE_H__



#include "iltree.h"

iltree_t iltree_double_init(iltree_t t, void *fileout);


void* alloc_double();

void dealloc_double(void *elem);

void copy_double(void *elem1, void *elem2);

int compare_long_double(void *elem1, void *elem2);

void print_long_double(void *elem, void *fileout);



void iltree_double_dump_edges(iltree_t t);


#endif // __ILTREE_DOUBLE_H__
