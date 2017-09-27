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
 *  This is an implementation of a simple insert-lookup Binary Search
 *  Tree.  It supports adding nodes, checking for the presence of
 *  keys, visiting the BST in pre-order, getting the maximum/minumum
 *  key, and applying a function to all the nodes. There is no support
 *  for deleting nodes.
 *
 */


/*
 *
 * A simple insert-lookup static binary tree datatype
 *
 */

#include <stdlib.h>
#include "iltree.h"
#include <stdio.h>


void __recursive_preorder(node_t *cur, ilfunc_t *funs){
  
  if(cur->left){
    __recursive_preorder(cur->left, funs);
  }
  funs->print(cur->info, funs->fileout);
  if(cur->right){
    __recursive_preorder(cur->right, funs);
  }
}

/*
 *
 * Recursive push of nodes in the nodecache :-)
 *
 */

void __recursive_destroy(node_t *cur, ilfunc_t *funs){
  if(cur->left){
    __recursive_destroy(cur->left, funs);
    free(cur->left);
    cur->left = NULL;
  }
  if(cur->right){
    __recursive_destroy(cur->right, funs);
    free(cur->right);
    cur->right = NULL;
  }
  funs->dealloc(cur->info);
}


int __recursive_insert(node_t *cur, node_t *elem, ilfunc_t *f){
  
  int res ;
  res = f->compare(cur->info, elem->info);
  /*  printf("res: %d\n", res); */
  if ( res > 0){
    if (cur->left){
      return __recursive_insert(cur->left, elem, f);
    }
    else{
      cur->left = elem;
      return 0;
    }
  }
  else if (res < 0){
    if (cur->right){
      return __recursive_insert(cur->right, elem, f);
    }
    else{
      cur->right = elem;
      return 0;
    }
  }
  printf("warning!!!!! duplicate entry!!!!!!\n\n");
  return -1;
}



void* __recursive_lookup(node_t *cur, void *v, ilfunc_t *f){
  
  int res;

  res = f->compare(cur->info, v);

  if (res > 0){
    if(cur->left)
      return __recursive_lookup(cur->left, v, f);
    else
      return NULL;
    
  }
  else if (res < 0){
    if(cur->right)
      return __recursive_lookup(cur->right, v, f);
    else
      return NULL;
  }
  else
    return cur->info;
}

void __recursive_map(node_t *cur, void (*func)(void*)){
  
  if (cur->left)
    __recursive_map(cur->left, func);
  func(cur->info);
  if (cur->right)
    __recursive_map(cur->right, func);
}

void __recursive_map_args(node_t *cur, void (*func)(void*, void*), void *args){
  
  if (cur->left)
    __recursive_map_args(cur->left, func, args);
  func(cur->info, args);
  if (cur->right)
    __recursive_map_args(cur->right, func, args);
}



iltree_t iltree_create(iltree_t t){
  if (!t) {
    t = (iltree_t)malloc(sizeof(iltree_struct_t));
  }
  t->root = NULL;
  return t;
}


void iltree_set_funs(iltree_t t, ilfunc_t *funs){
  
  t->funs = *funs;
}


void iltree_insert(iltree_t t, void *elem){
  
  node_t *n;
  
  n = (node_t*)malloc(sizeof(node_t));
  n->info = t->funs.alloc(); 
  t->funs.copy(elem, n->info);
  n->left = n->right = NULL;
  if (t->root == NULL){
    t->root = n;
  }
  else{
    __recursive_insert(t->root, n, & (t->funs));
  }
}


void iltree_destroy(iltree_t t){
  
  if(t->root)
    __recursive_destroy(t->root, & (t->funs));
  free(t->root);
  free(t);
}




void iltree_view_pre(iltree_t t){
  
  if (t->root){
    /*printf("----\n");*/
    __recursive_preorder(t->root, & (t->funs));
    /*printf("----\n");*/
  }
  else
    printf("----- Empty tree!!!! -----\n");
  
}



void* iltree_lookup(iltree_t t , void *elem){

  if(t->root)
    return __recursive_lookup(t->root, elem, & (t->funs) );
  else 
    return NULL;
}


void iltree_map(iltree_t t, void (*func)(void*)){
  
  __recursive_map(t->root, func);
  
}


void iltree_map_args(iltree_t t, void (*func)(void*, void*), void *args){
  
  __recursive_map_args(t->root, func, args);
  
}

void* iltree_get_fileout(iltree_t t){

  return t->funs.fileout;
}

void iltree_set_fileout(iltree_t t, void *f){
  
  t->funs.fileout = f;
}

void* __recursive_getmin(node_t *cur){
  
  if(cur->left){
    return __recursive_getmin(cur->left);
  }
  else{
    return cur->info;
  }

}


void* iltree_getmin(iltree_t t){
  
  if (!t){
    return NULL;
  }
  else{
    return __recursive_getmin(t->root);
  }

}


void* __recursive_getmax(node_t *cur){
  
  if(cur->right){
    return __recursive_getmax(cur->right);
  }
  else{
    return cur->info;
  }

}


void* iltree_getmax(iltree_t t){
  
  if (!t){
    return NULL;
  }
  else{
    return __recursive_getmax(t->root);
  }

}

