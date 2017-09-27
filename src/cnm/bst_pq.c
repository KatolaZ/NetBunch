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

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

#include "utils.h"
#include "bst_pq.h"



/* BST-related functions */


void __recursive_preorder(node_t *cur, ilfunc_t *funs){
  
  if(cur->left){
    __recursive_preorder(cur->left, funs);
  }
  if(cur -> active)
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
    cur->left = NULL;
  }
  if(cur->right){
    __recursive_destroy(cur->right, funs);
    cur->right = NULL;
  }
}


int __recursive_insert(node_t *cur, node_t *elem, ilfunc_t *f){
  
  int res ;
  res = f->compare(cur->info, elem->info);
  /*  printf("res: %d\n", res); */
  assert(res != 0);
  if ( res > 0){
    if (cur->left){
      return __recursive_insert(cur->left, elem, f);
    }
    else{
      cur->left = elem;
      elem->parent = cur;
      return 0;
    }
  }
  else if (res < 0){
    if (cur->right){
      return __recursive_insert(cur->right, elem, f);
    }
    else{
      cur->right = elem;
      elem->parent = cur;
      return 0;
    }
  }
  if (cur -> active){
    printf("warning!!!!! duplicate entry!!!!!!\n\n");
    exit(1);
  }
  else 
    cur->active = 1;
  return -1;
}



void* __recursive_lookup(node_t *cur, int v, ilfunc_t *f){
  
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
  else{ /* res == 0, we found the element we were looking for */
    return cur;
  }
}



node_t* __recursive_getmin(node_t *cur){
  
  if(cur->left){
    return __recursive_getmin(cur->left);
  }
  else{
    return cur;
  }

}


node_t* __tree_getmin(node_t *n){
  
  if (!n){
    return NULL;
  }
  else{
    return __recursive_getmin(n);
  }

}


/* This is used by __tree__delete to put another tree v in place of
   the current node u */


void __tree_transplant(bst_pq_t t, node_t *u, node_t * v){
  
  if (u->parent == NULL){
    t->root = v;
  }
  else if(u == u->parent->left){
    u->parent->left = v;
  }
  else{
    u->parent->right = v;
  }
  if (v != NULL){
    v->parent = u->parent;
  }

}


void __tree_delete(bst_pq_t t, node_t *z){

  node_t *y;
  
  if (z == NULL)
    return;

  if (z->left == NULL){
    __tree_transplant(t, z, z->right);
  }
  else if(z->right == NULL){
    __tree_transplant(t, z, z->left);
  }
  else{
    y = __tree_getmin(z->right);
    if (y->parent != z){
      __tree_transplant(t, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    __tree_transplant(t, z, y);
    y->left = z->left;
    y->left->parent = y;
  }
}




void bst_pq_tree_destroy(bst_pq_t t){
  
  if(t->root)
    __recursive_destroy(t->root, & (t->bst_funs));
  free(t);
}




void __bst_pq_tree_view_pre(bst_pq_t t){
  
  if (t->root){
    printf("----\n");
    __recursive_preorder(t->root, & (t->bst_funs));
    printf("\n----\n");
  }
  else
    printf("----- Empty tree!!!! -----\n");
  
}



void* __bst_pq_tree_lookup(bst_pq_t t , int elem){

  if(t->root)
    return __recursive_lookup(t->root, elem, & (t->bst_funs) );
  else 
    return NULL;
}





/* void bst_pq_tree_map(bst_pq_t t, void (*func)(void*)){ */
  
/*   __recursive_map(t->root, func); */
  
/* } */


/* void bst_pq_tree_map_args(bst_pq_t t, void (*func)(void*, void*), void *args){ */
  
/*   __recursive_map_args(t->root, func, args); */
  
/* } */

void* bst_pq_tree_get_fileout(bst_pq_t t){

  return t->bst_funs.fileout;
}

void bst_pq_tree_set_fileout(bst_pq_t t, void *f){
  
  t->bst_funs.fileout = f;
}



node_t* __recursive_getmax(node_t *cur){
  
  if(cur->right){
    return __recursive_getmax(cur->right);
  }
  else{
    return cur;
  }

}


void* bst_pq_tree_getmax(bst_pq_t t){
  
  if (!t){
    return NULL;
  }
  else{
    return __recursive_getmax(t->root);
  }

}


/************************
 * PQ-related functions *
 ************************/

void __update_handle(bst_pq_t q, int idx){
  
  node_t *n;
  n = (node_t*) q->v[idx];
  n->index = idx;
  //q->handles[q->pq_funs.get_id(q->v[idx])] = idx;
}



void __bst_pq_queue_sift_up(bst_pq_t q, int i){

  int idx, parent;
  void *tmp;

  if ( q->last < 0)
    return; /* no sifting if the PQ is empty!!! */

  idx = i;
  parent = PARENT(idx);
  
  switch(q->qtype){
  case MAX_QUEUE:
    while ( idx >0 && q->pq_funs.compare(q->v[idx]->key,  q->v[parent]->key) > 0){
      tmp = q->v[idx];
      q->v[idx] = q->v[parent];
      q->v[parent] = tmp;
      __update_handle(q, idx);
      __update_handle(q, parent);
      idx = parent;
      parent = PARENT(idx);
    }
    break;
  case MIN_QUEUE:
    while ( idx >0 && q-> pq_funs.compare(q->v[idx]->key, q->v[parent]->key) < 0){
      tmp = q->v[idx];
      q->v[idx] = q->v[parent];
      q->v[parent] = tmp;
      __update_handle(q, idx);
      __update_handle(q, parent);
      idx = parent;
      parent = PARENT(idx);
    }
    break;
  }
}


void __bst_pq_queue_sift_down(bst_pq_t q, int i){

  int left, right, largest, smallest;
  void *tmp;

  if ( q->last < 0)
    return; /* no sifting if the PQ is empty!!! */
  if( i > q->last)
    return; /* no sifting if the index i is beyond the end of the PQ */

  switch(q->qtype){
  case MAX_QUEUE:
    largest = i;
    left = 2 * i + 1;
    right = 2 * i + 2;
  
    if (left <= q->last && q->pq_funs.compare(q->v[left]->key, q->v[largest]->key) > 0){
      largest = left;
    }
    if (right <= q->last && q->pq_funs.compare(q->v[right]->key, q->v[largest]->key) > 0){
      largest = right;
    }
    if (largest != i){
      tmp = q->v[i];
      q->v[i] = q->v[largest];
      q->v[largest] = tmp;
      __update_handle(q, i);
      __update_handle(q, largest);
      __bst_pq_queue_sift_down(q, largest);
    }
    else{
      __update_handle(q, i);
    }
    break;
    
  case MIN_QUEUE:
    smallest = i;
    left = 2 * i + 1;
    right = 2 * i + 2;
  
    if (left <= q->last && q->pq_funs.compare(q->v[left]->key, q->v[smallest]->key) < 0){
      smallest = left;
    }
    if (right <= q->last && q->pq_funs.compare(q->v[right]->key, q->v[smallest]->key) < 0){
      smallest = right;
    }
    if (smallest != i){
      tmp = q->v[i];
      q->v[i] = q->v[smallest];
      q->v[smallest] = tmp;
      __update_handle(q, i);
      __update_handle(q, smallest);
      __bst_pq_queue_sift_down(q, smallest);
    }
    else{
      __update_handle(q, i);
    }
    break;
  }
}


void __bst_pq_queue_insert(bst_pq_t q, void *elem){

  //void *tmp;

  if (q->last == q->N-1){
    /* reallocate the array to arrange a few new elements */
    q->N += 10;
    q->v = realloc(q->v, (q->N) * sizeof(void*));
    VALID_PTR_OR_EXIT(q->v, 17);
    //q->v = tmp;
  }
  
  q->last += 1;
  q->v[q->last] = elem;
  __update_handle(q, q->last);
  __bst_pq_queue_sift_up(q, q->last);

}



int __bst_pq_queue_delete_index(bst_pq_t q, int index){

  if (q->last >=0 && index <= q->last){
    //*val = q->v[0];
    q->v[index] = q->v[q->last];
    q->last -= 1;
    __bst_pq_queue_sift_down(q, index);
    return 0;
  }
  else{
    return 1;
  }
}



void* __bst_pq_queue_peek(bst_pq_t q){

  if (q->last >= 0)
    return q->v[0];
  else
    return NULL;
}



int __bst_pq_queue_force_key(bst_pq_t q, unsigned int idx, double key){

  switch(q->qtype){
  case MAX_QUEUE:
    if (q->pq_funs.compare_to_key(q->v[idx], key) > 0){
      //q->pq_funs.set_key(q->v[idx], key);
      q->v[idx]->key = key;
      __bst_pq_queue_sift_down(q, idx);
    }
    else{
      //q->pq_funs.set_key(q->v[idx], key);
      q->v[idx]->key = key;
      __bst_pq_queue_sift_up(q, idx);
    }
    break;
  case MIN_QUEUE:
    if (q->pq_funs.compare_to_key(q->v[idx], key) < 0){
      //q->pq_funs.set_key(q->v[idx], key);
      q->v[idx]->key = key;
      __bst_pq_queue_sift_down(q, idx);
    }
    else{
      //q->pq_funs.set_key(q->v[idx], key);
      q->v[idx]->key = key;
      __bst_pq_queue_sift_up(q, idx);
    }
    break;
  }
  return 0;
}



void __bst_pq_queue_dump(bst_pq_t q){

  int i; 
  
  unsigned int N;
  
  N = q->last+1;
  
  printf("N: %d last:%d root:", q->N, q->last);
  if (q->last >=0)
    q->pq_funs.print_elem(q->v[0]);
  else
    printf("NULL");
  printf("\n");
  
  for(i=0; i<N; i++){
    if (i < (N+1)/2){
      if (2*i+1 < N)
        if (2*i + 2 < N){
          printf("%d: ", i);
          q->pq_funs.print_elem(q->v[i]);
          printf(" (");
          q->pq_funs.print_elem(q->v[2*i+1]);
          printf(", ");
          q->pq_funs.print_elem(q->v[2*i+2]);
          printf(")\n");
        }
        else{
          printf("%d: ", i);
          q->pq_funs.print_elem(q->v[i]);
          printf(" (");
          q->pq_funs.print_elem(q->v[2*i+1]);
          printf(", NULL)\n");
        }
      else{
        printf("%d: ", i);
        q->pq_funs.print_elem(q->v[i]);
        printf(" (NULL, NULL)\n");
      }
    }
    else{
      printf("%d: ", i);
      q->pq_funs.print_elem(q->v[i]);
      printf(" (NULL, NULL)\n");
    }
  }
  printf("\n");
}




/* bst_pq interface */

/* init the BST_PQ -- TESTED --*/

bst_pq_t bst_pq_create(ilfunc_t *bst_funs, gen_pqueue_func_t *pq_funs, char qtype, 
                       unsigned int N, gen_stack_t *node_cache){
  
  bst_pq_t b;
  
  b = (bst_pq_t)malloc(sizeof(bst_pq_struct_t));
  b->root = NULL;
  b->bst_funs = *bst_funs;
  
  b->bst_funs.fileout = stdout;
  b->qtype = qtype;
  b->N = N;
  b->last = -1;
  b->pq_funs = *pq_funs;
  b->v = b->pq_funs.alloc_vector(N);
  if (node_cache == NULL){
    b->node_cache = malloc(sizeof(gen_stack_t));
    gen_stack_create(b->node_cache);
  }
  else{
    b->node_cache = node_cache;
  }
  return b;
}







/* insert a new element in the BST_PQ -- TESTED */
void bst_pq_insert(bst_pq_t b, unsigned int elem_info,  double elem_key){
  
  /* insert the element in the tree first */
  node_t *n;
  int err;

  n = __bst_pq_tree_lookup(b, elem_info);
  
  /* The following assert should fail ONLY if there is an auto-loop */
  assert(n == NULL);
  
  if (gen_stack_empty(b->node_cache)){
    n = (node_t*)malloc(sizeof(node_t));
    //n->info = b->bst_funs.alloc(); 
    //n->key = b->pq_funs.alloc_key();
  }
  else{
    err = gen_stack_pop(b->node_cache, (void**) &n);
    if (err){
      n = malloc(sizeof(node_t));
      //n->info = b->bst_funs.alloc(); 
      //n->key = b->pq_funs.alloc_key();
    }
  }
  //b->bst_funs.copy(elem_info, n->info);
  n->info = elem_info;
  n->left = n->right = NULL;
  if (b->root == NULL){
    b->root = n;
    n->parent = NULL;
  }
  else{
    err = __recursive_insert(b->root, n, & (b->bst_funs));
    if(err){
      fprintf(stderr, "Error during insert into BST!!! (%s: %d)\n", 
              __FILE__, __LINE__);
      exit(23);
    }
  }
  n->active = 1;
  n->index = -1;
  /* set the key as needed */
  //b->pq_funs.copy_key(elem_key, n->key);
  n->key = elem_key;
  
  /* then, insert the pointer to the element in the PQ */
  __bst_pq_queue_insert(b, n);
  
}

/* delete (or deactivate) the element associated to a given info  -- TESTED --*/
int  bst_pq_delete(bst_pq_t b, unsigned int info){
  
  node_t *n;

  n = __bst_pq_tree_lookup(b, info);
  
  if (n != NULL){
    __tree_delete(b, n);
    n->active = 0; /* deactivate the node */
    /* After the node has been deleted from the tree, we add it to
       the node cache */
    gen_stack_push(b->node_cache, n);
    __bst_pq_queue_delete_index(b, n->index); /* remove the reference form the PQ */
    return 0; /* No problem */
  }
  return 1; /* the element does not exist */
}

/* change the key of an element in the BST_PQ  -- TESTED --*/
int bst_pq_change_key(bst_pq_t b, unsigned int info, double key){

  node_t *n;
  int idx;
  
  n = __bst_pq_tree_lookup(b, info);
  if (n != NULL){
    /* the element exists. We should just force its key and sift as required */
    idx = n->index;
    __bst_pq_queue_force_key(b, idx, key);
    return 0; /* No problem */
  }
  else{
    return 1; /* The element does not exist */
  }
  
}

/* return the head of the PQ -- TESTED --*/
void* bst_pq_peek(bst_pq_t q){
  
  return __bst_pq_queue_peek(q);
}


/* Read the key of a given node of the BST */
double bst_pq_get_key(bst_pq_t b, unsigned int info){
  
  node_t *n;

  n = __bst_pq_tree_lookup(b, info);
  if (n != NULL){
    return n->key;
  }
  else{
    return -100000;
  }
}


/* Read the key of the element of index "index" in the PQ */
double bst_pq_get_key_from_index(bst_pq_t b, int index){
  
  node_t *n;
  
  n = (node_t *)b->v[index];
  
  return n->key;
}


/* Dump the BST  -- TESTED -- */
void bst_pq_dump_bst(bst_pq_t b){
  
  __bst_pq_tree_view_pre(b);

}

/* Show the PQ -- TESTED -- */
void bst_pq_dump_pq(bst_pq_t b){
  
  __bst_pq_queue_dump(b);

}

/* Perform a lookup of an element in the BST_PQ -- TESTED-- */
node_t* bst_pq_lookup(bst_pq_t b, unsigned int info){

  return __bst_pq_tree_lookup(b, info);
}


node_t* bst_pq_lookup_active(bst_pq_t b, unsigned int info){
  
  node_t *ptr;

  ptr = __bst_pq_tree_lookup(b, info);

  if(ptr)
    assert(ptr->active != 0);
  
  if (ptr != NULL && ptr->active){
    return ptr;
  }

  return NULL;
}


/* void bst_pq_insert_existing(bst_pq_t b, node_t *n){ */
  
/*   /\* insert the element in the tree first *\/ */
/*   if (n == NULL){ */
/*     fprintf(stderr, "Error!!!! Attempt to insert a NULL existing element (%s: %d)\n",  */
/*             __FILE__, __LINE__); */
/*     exit(37); */
/*   } */
/*   /\* n->info = b->bst_funs.alloc();  *\/ */
/*   /\* n->key = b->pq_funs.alloc_key(); *\/ */
/*   /\* b->bst_funs.copy(elem_info, n->info); *\/ */
/*   n->left = n->right = NULL; */
/*   if (b->root == NULL){ */
/*     b->root = n; */
/*     n->parent = NULL; */
/*   } */
/*   else{ */
/*     __recursive_insert(b->root, n, & (b->bst_funs)); */
/*   } */

/*   n->active = 1; */
/*   /\* set the key as needed *\/ */
/*   //b->pq_funs.copy_key(elem_key, n->key); */

/*   /\* then, insert the pointer to the element in the PQ *\/ */
/*   __bst_pq_queue_insert(b, n); */
  
/* } */






void bst_pq_destroy(bst_pq_t b, char destroy_cache){

  int i;
  node_t *n;
  
  if (b == NULL)
    return;

  if(destroy_cache && b->node_cache != NULL ){
    while(!gen_stack_pop(b->node_cache, (void**) &n)){
      //b->bst_funs.dealloc(n->info);
      //free(n->key);
      free(n);
    }
    free(b->node_cache->v);
    free(b->node_cache);
  }
  
  for(i=b->last; i>=0; i--){
    __tree_delete(b, b->v[i]);
    __bst_pq_queue_delete_index(b, i); /* remove the reference form the PQ */
    //bst_pq_delete(b, b->v[i]->info);
    //b->bst_funs.dealloc(b->v[i]->info);
    //free(b->v[i]->key);
    free(b->v[i]);
  }
  free(b->v);
  free(b);
}
