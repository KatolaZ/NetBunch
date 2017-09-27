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
 *  This is an implementation of a (MIN/MAX)-priority-queue based on
 *  gen_heap.  Most of the functions, with the only exception of
 *  gen_pqueue_change_key which is the core of the priority queue, are
 *  just wrappers around the corresponding functions in gen_heap
 *
 */




#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gen_pqueue.h"

void __update_handle(gen_pqueue_t *q, int idx){
  
  q->handles[q->funs.get_id(q->v[idx])] = idx;
}


gen_pqueue_t * gen_pqueue_init(unsigned int N, char qtype, gen_pqueue_func_t *funs){

  gen_pqueue_t* q;
  int i;

  q = malloc(sizeof(gen_pqueue_t));
  q->qtype = qtype;
  q->N = N;
  q->last = -1;
  q->funs = *funs;
  q->v = q->funs.alloc_vector(N);
  q->handles = malloc(N * sizeof(int));
  for (i=0; i<N; i++){
    q->handles[i] = -1;
  }
  return q;  

}

void __gen_pqueue_sift_up(gen_pqueue_t *q, int i){

  int idx, parent;
  void *tmp;

  idx = i;
  parent = PARENT(idx);
  
  switch(q->qtype){
  case MAX_QUEUE:
    while ( idx >0 && q->funs.compare(q->v[idx],  q->v[parent]) > 0){
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
    while ( idx >0 && q-> funs.compare(q->v[idx], q->v[parent]) < 0){
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


void __gen_pqueue_sift_down(gen_pqueue_t *q, int i){

  int left, right, largest, smallest;
  void *tmp;

  switch(q->qtype){
  case MAX_QUEUE:
    largest = i;
    left = 2 * i + 1;
    right = 2 * i + 2;
  
    if (left <= q->last && q->funs.compare(q->v[left], q->v[largest]) > 0){
      largest = left;
    }
    if (right <= q->last && q->funs.compare(q->v[right], q->v[largest]) > 0){
      largest = right;
    }
    if (largest != i){
      tmp = q->v[i];
      q->v[i] = q->v[largest];
      q->v[largest] = tmp;
      __update_handle(q, i);
      __update_handle(q, largest);
      __gen_pqueue_sift_down(q, largest);
    }
    else{
      __update_handle(q, i);
    }
    break;
    
  case MIN_QUEUE:
    smallest = i;
    left = 2 * i + 1;
    right = 2 * i + 2;
  
    if (left <= q->last && q->funs.compare(q->v[left], q->v[smallest]) < 0){
      smallest = left;
    }
    if (right <= q->last && q->funs.compare(q->v[right], q->v[smallest]) < 0){
      smallest = right;
    }
    if (smallest != i){
      tmp = q->v[i];
      q->v[i] = q->v[smallest];
      q->v[smallest] = tmp;
      __update_handle(q, i);
      __update_handle(q, smallest);
      __gen_pqueue_sift_down(q, smallest);
    }
    else{
      __update_handle(q, i);
    }
    break;
  }
}


void gen_pqueue_insert(gen_pqueue_t *q, void *elem){

  if (q->last < q->N-1){
    q->last += 1;
    q->v[q->last] = elem;
    __update_handle(q, q->last);
  }
  else{
    fprintf(stderr, "Error! Trying to insert more than %d elements in the heap (%s:%d)\n", 
            q->N, __FILE__, __LINE__);
    return;
  }
  __gen_pqueue_sift_up(q, q->last);
}



int gen_pqueue_delete(gen_pqueue_t *q, void **val){

  if (q->last >=0){
    *val = q->v[0];
    q->v[0] = q->v[q->last];
    q->last -= 1;
    __gen_pqueue_sift_down(q, 0);
    return 0;
  }
  else{
    return 1;
  }
}



void* gen_pqueue_peek(gen_pqueue_t *q){

  return q->v[0];
}

gen_pqueue_t* gen_pqueue_from_array(void **v, unsigned int N, unsigned int last, char qtype, 
                                    gen_pqueue_func_t *funs){

  gen_pqueue_t *q;
  int i;

  q = gen_pqueue_init(N, qtype, funs);
  /* FIXME!!!! WARNING!!!! we should associate the array v to the array of the pqueue!!!! */
  for (i=last >> 1 ; i>=0; i--){
    __gen_pqueue_sift_down(q, i);
  }
  return q;
}




int gen_pqueue_force_key(gen_pqueue_t *q, unsigned int idx, void *key){

  
  switch(q->qtype){
  case MAX_QUEUE:
    if (q->funs.compare_to_key(q->v[idx], key) > 0){
      q->funs.set_key(q->v[idx], key);
      __gen_pqueue_sift_down(q, idx);
    }
    else{
      q->funs.set_key(q->v[idx], key);
      __gen_pqueue_sift_up(q, idx);
    }
    break;
  case MIN_QUEUE:
    if (q->funs.compare_to_key(q->v[idx], key) < 0){
      q->funs.set_key(q->v[idx], key);
      __gen_pqueue_sift_down(q, idx);
    }
    else{
      q->funs.set_key(q->v[idx], key);
      __gen_pqueue_sift_up(q, idx);
    }
    break;
  }
  return 0;
}


int gen_pqueue_change_key(gen_pqueue_t *q, unsigned int idx, void *key){

  
  switch(q->qtype){
  case MAX_QUEUE:
    if (q->funs.compare_to_key(q->v[idx], key) > 0){
      return KEY_ERROR; /* we cannot assign a smaller key on a MAX_QUEUE*/
    }
    /* If everything is OK, we then set the new key here */
    q->funs.set_key(q->v[idx], key);
    if (idx == 0)
      return 0;
    __gen_pqueue_sift_up(q, idx);
    break;
  case MIN_QUEUE:
    if (q->funs.compare_to_key(q->v[idx], key) < 0){
      return KEY_ERROR; /* we cannot assign a higher key on a MIN_QUEUE*/
    }
    /* If everything is OK, we then set the new key here */
    q->funs.set_key(q->v[idx], key);
    /* parent = (int)(floor((idx-1)/2)); */
    if (idx == 0)
      return 0;
    __gen_pqueue_sift_up(q, idx);
    break;
  }
  return -1;
}



void gen_pqueue_dump(gen_pqueue_t *q){
  
  int i; 
  
  unsigned int N;
  
  N = q->last+1;
  
  printf("N: %d last:%d root:", q->N, q->last);
  if (q->last >=0)
    q->funs.print_elem(q->v[0]);
  else
    printf("NULL");
  printf("\n");
  
  for(i=0; i<N; i++){
    if (i < (N+1)/2){
      if (2*i+1 < N)
        if (2*i + 2 < N){
          printf("%d: ", i);
          q->funs.print_elem(q->v[i]);
          printf(" (");
          q->funs.print_elem(q->v[2*i+1]);
          printf(", ");
          q->funs.print_elem(q->v[2*i+2]);
          printf(")\n");
        }
        else{
          printf("%d: ", i);
          q->funs.print_elem(q->v[i]);
          printf(" (");
          q->funs.print_elem(q->v[2*i+1]);
          printf(", NULL)\n");
        }
      else{
        printf("%d: ", i);
        q->funs.print_elem(q->v[i]);
        printf(" (NULL, NULL)\n");
      }
    }
    else{
      printf("%d: ", i);
      q->funs.print_elem(q->v[i]);
      printf(" (NULL, NULL)\n");
    }
  }
  printf("\n");
}


void gen_pqueue_destroy(gen_pqueue_t *q){

  int i;
  
  /* First deallocate all the remaining elems (those that have not
     been deleted) */
  for(i=0; i<=q->last; i++){
    q->funs.dealloc_elem(q->v[i]);
  }
  
  /* now we deallocate the array of elems */
  q->funs.dealloc_vector(q->v);
  free(q->handles);
  free(q);
}


int gen_pqueue_get_handle(gen_pqueue_t *q, int id){

  if (id >= q->N){
    return ID_ERROR;
  }
  return q->handles[id];
}

void* gen_pqueue_get_key(gen_pqueue_t *q, int idx){
  
  return q->funs.get_key(q->v[idx]);
  
}
