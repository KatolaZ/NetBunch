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
 *  This files implements a disjoint-set data structure, where nodes
 *  labels are integers.
 *
 */


#include <stdio.h>
#include <stdlib.h>

#include "dset.h"

void dset_makeset(dset_t *ds){
  
  if (*ds == NULL){
    *ds= malloc(sizeof(dset_elem_t));
  }
  (*ds)->parent = *ds;
  (*ds) -> rank = 0;
}

void dset_destroy(dset_t ds){
  free(ds);
}


void dset_makeset_id(dset_t *ds, int id){
  
  dset_makeset(ds);
  (*ds)->id = id;
}


dset_t dset_find(dset_t ds){
  if (ds -> parent == ds){
    return ds;
  }
  else return dset_find(ds->parent);
}

void dset_union(dset_t s1, dset_t s2){
  dset_t r1, r2;

  r1= dset_find(s1);
  r2= dset_find(s2);
  r2->parent = r1;
}


void dset_union_opt(dset_t s1, dset_t s2){
  dset_t r1, r2;

  r1= dset_find(s1);
  r2= dset_find(s2);
  if (r1 == r2){
    return;
  }
  
  if (r1->rank < r2->rank){
    r1->parent = r2;
  }
  else if (r1->rank > r2->rank){
    r2->parent = r1;
  }
  else{
    r2->parent = r1;
    r1->rank += 1;
  }
}


dset_t dset_find_opt(dset_t ds){
  if (ds->parent != ds){
    ds->parent = dset_find_opt(ds->parent);
  }
  return ds->parent;
}


int dset_find_id(dset_t ds){
  
  dset_t res;

  res = dset_find(ds);
  
  return res->id;
}


int dset_find_id_opt(dset_t ds){
  
  dset_t res;

  res = dset_find_opt(ds);
  
  return res->id;
}

