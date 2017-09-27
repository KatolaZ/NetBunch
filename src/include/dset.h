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

#ifndef __DSET_H__
#define __DSET_H__


typedef struct dset{
  int rank;
  struct dset *parent;
  int id;
} dset_elem_t;

typedef dset_elem_t* dset_t;

void dset_makeset(dset_t *ds);

void dset_destroy(dset_t ds);

dset_t dset_find(dset_t ds);

void dset_union(dset_t s1, dset_t s2);

void dset_union_opt(dset_t d1, dset_t s2);

dset_t dset_find_opt(dset_t ds);

void dset_makeset_id(dset_t *ds, int id);

int dset_find_id(dset_t ds);

int dset_find_id_opt(dset_t ds);



#endif /* __DSET_H__ */
