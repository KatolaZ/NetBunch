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
 *  This is an implementation of binary heaps (both Min-Heap and
 *  Max-Heap).
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "gen_heap.h"



gen_heap_t * gen_heap_init(unsigned int N, char htype, gen_heap_func_t *funs){
  
  gen_heap_t* h;

  h = malloc(sizeof(gen_heap_t));
  h->htype = htype;
  h->N = N;
  h->last = -1;
  h->funs = *funs;
  h->v = h->funs.alloc_vector(N);
  return h;  
}


void __gen_heap_sift_up(gen_heap_t *h, int i){

  int idx, parent;
  void *tmp;
  
  idx = i;
  parent = PARENT(idx);
  switch(h->htype){
  case MAX_HEAP:
    while ( idx >0 && h->funs.compare(h->v[idx],  h->v[parent]) > 0){
      tmp = h->v[idx];
      h->v[idx] = h->v[parent];
      h->v[parent] = tmp;
      idx = parent;
      parent = PARENT(idx);
    }
    break;
  case MIN_HEAP:
    while ( idx >0 && h-> funs.compare(h->v[idx], h->v[parent]) < 0){
      tmp = h->v[idx];
      h->v[idx] = h->v[parent];
      h->v[parent] = tmp;
      idx = parent;
      parent = PARENT(idx);
    }
    break;
  }
}


void __gen_heap_sift_down(gen_heap_t *h, int i){

  int left, right, largest, smallest;
  void *tmp;


  switch(h->htype){
    
  case MAX_HEAP:
    largest = i;
    left = 2 * i + 1;
    right = 2 * i + 2;
    
    if (left <= h->last && h->funs.compare(h->v[left], h->v[largest]) > 0){
      largest = left;
    }
    if (right <= h->last && h->funs.compare(h->v[right], h->v[largest]) > 0){
      largest = right;
    }
    if (largest != i){
      tmp = h->v[i];
      h->v[i] = h->v[largest];
      h->v[largest] = tmp;
      __gen_heap_sift_down(h, largest);
    }
    break;
    
  case MIN_HEAP:
    smallest = i;
    left = 2 * i + 1;
    right = 2 * i + 2;
  
    if (left <= h->last && h->funs.compare(h->v[left], h->v[smallest]) < 0){
      smallest = left;
    }
    if (right <= h->last && h->funs.compare(h->v[right], h->v[smallest]) < 0){
      smallest = right;
    }
    if (smallest != i){
      tmp = h->v[i];
      h->v[i] = h->v[smallest];
      h->v[smallest] = tmp;
      __gen_heap_sift_down(h, smallest);
    }
    break;
  }
  
}


void gen_heap_insert(gen_heap_t *h, void *elem){
  
  if (h->last < h->N-1){
    h->last += 1;
    h->v[h->last] = elem;
  }
  else{
    fprintf(stderr, "Error! Trying to insert more than %d elements in the heap (%s:%d)\n", 
            h->N, __FILE__, __LINE__);
    return;
  }
  __gen_heap_sift_up(h, h->last);
}



int gen_heap_delete(gen_heap_t *h, void **val){
  
  if (h->last >=0){
    *val = h->v[0];
    h->v[0] = h->v[h->last];
    h->last -= 1;
    __gen_heap_sift_down(h,0);
    return 0;
  }
  else{
    return 1;
  }
}



void* gen_heap_peek(gen_heap_t *h){
  return h->v[0];
}


gen_heap_t* gen_heap_from_array(void **v, unsigned int N, unsigned int last, 
                                char htype, gen_heap_func_t *funs){
  
  gen_heap_t *h;
  int i;
  
  h = malloc(sizeof(gen_heap_t));
  h->N = N;
  h->last = last;
  h->htype = htype;
  h->funs = *funs;
  h->v = v;
  
  for (i=last >> 1 ; i>=0; i--){
    __gen_heap_sift_down(h, i);
  }
  return h;
}



void gen_heap_dump(gen_heap_t* h){

  int i; 
  
  unsigned int N;
  
  N = h->last+1;
  
  printf("N: %d last:%d root:", h->N, h->last);
  if (h->last >=0)
    h->funs.print_elem(h->v[0]);
  else
    printf("NULL");
  printf("\n");
  
  for(i=0; i<N; i++){
    if (i < (N+1)/2){
      if (2*i+1 < N)
        if (2*i + 2 < N){
          printf("%d: ", i);
          h->funs.print_elem(h->v[i]);
          printf(" (");
          h->funs.print_elem(h->v[2*i+1]);
          printf(", ");
          h->funs.print_elem(h->v[2*i+2]);
          printf(")\n");
        }
        else{
          printf("%d: ", i);
          h->funs.print_elem(h->v[i]);
          printf(" (");
          h->funs.print_elem(h->v[2*i+1]);
          printf(", NULL)\n");
        }
      else{
        printf("%d: ", i);
        h->funs.print_elem(h->v[i]);
        printf(" (NULL, NULL)\n");
      }
    }
    else{
      printf("%d: ", i);
      h->funs.print_elem(h->v[i]);
      printf(" (NULL, NULL)\n");
    }
  }
  printf("\n");
}

void gen_heap_destroy(gen_heap_t *h){

  int i;
  
  /* First deallocate all the elems */
  for(i=0; i<=h->last; i++){
    h->funs.dealloc_elem(h->v[i]);
  }
  
  /* now we deallocate the array */
  h->funs.dealloc_vector(h->v);
  h->v = NULL;
  free(h);
}


int gen_heap_sort(void *v, unsigned int N, size_t size, char dir, 
                   int (*compar)(const void *, const void *)){
  
  gen_heap_func_t funs;
  gen_heap_t *h;
  void *val, **new_v=NULL, *tmp_v=NULL;
  char htype;
  int i;

  funs.compare = compar;

  htype = MAX_HEAP;
  
  if (dir == SORT_DESC)
    htype = MIN_HEAP;

  new_v = malloc(N * sizeof(void*));
  tmp_v = malloc(N * size);

  for (i=0; i<N; i++){
    new_v[i] = (char*)v+ i* size;
  }

  h = gen_heap_from_array(new_v, N, N-1, htype, &funs);
  for(i = 0; i<N; i++){
    if (gen_heap_delete(h, (void **)&val)){
      free(new_v);
      free(tmp_v);
      return ERR_DELETE;
    }
    //fprintf(stderr, "(%p <- %p) ", (char*) v + (N-i-1) * size, val);
    memcpy((char*)tmp_v + (N-i-1) * size, val, size);
  }
  memcpy(v, tmp_v, N*size);
  free(new_v);
  free(tmp_v);
  return 0;
}

