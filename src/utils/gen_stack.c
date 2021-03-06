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
 *  Implementation of a stack data structure, which stores pointers to
 *  generic objects
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "gen_stack.h"

void gen_stack_create(gen_stack_t *s){
  
  s->size = 10;
  s->head = -1;
  s->v = malloc(s->size * sizeof(void*)); 
  
}

void gen_stack_push(gen_stack_t *s, void *elem){
  
  //void ** tmp;
  
  if (s->head == s->size-1){
    s->size += 10;
    s->v = realloc(s->v, s->size * sizeof(void*));
    if (!s->v){
      fprintf(stderr, "Unable to allocate more memory in stack.c:stack_push... Exiting!\n");
      exit(17);
    }
  }
  s->head++;
  s->v[s->head] = elem;
}

int gen_stack_pop(gen_stack_t *s, void  **res){

  if (!gen_stack_empty(s)){
    *res = s->v[s->head];
    s->head--;
    return 0;
  }
  else{
    return -1;
  }
  
}

int gen_stack_empty(gen_stack_t *s){

  return (s->head < 0 ? 1 : 0);
}


int gen_stack_size(gen_stack_t *s){
  return s->head + 1;
}
