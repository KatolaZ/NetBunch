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
 *  Implementation of a data structure to maintain a cumulative
 *  distribution and sample values from it. Used in all the models of
 *  growing graphs based on some kind of preferential attachment.
 * 
 */



#include <stdlib.h>
#include <stdio.h>
#include "cum_distr.h"



cum_distr_t* cum_distr_init(unsigned int N){

  cum_distr_t* d = NULL;

  d = malloc(sizeof(cum_distr_t));
  
  d->N = N;
  d->num = 0;
  d->sum = 0;

  d->v = malloc((d->N) * sizeof(idval_t));
  
  return d;
}


int cum_distr_add(cum_distr_t *d, unsigned int id, long double val){
  
  idval_t *tmp;

  if (d->num == d->N){
    //fprintf(stderr, "+++++++++ REALLOC ++++++++++\n");
    d->N += 10;
    tmp = realloc(d->v, d->N * sizeof(idval_t));
    if (!tmp){
      d->N -= 10;
      fprintf(stderr, "#### Error!!! Unable to add more values to the cumulative distribution!!!\n");
      return -1;
    }
    else{
      d->v = tmp;
    }
  }
  
  d->sum += val;
  d->v[d->num].id = id;
  d->v[d->num].value = d->sum;
  d->num += 1;
  //fprintf(stderr, ">>>>>> update >>>>>> d->num: %d\n", d->num);
  return 0;

}



/* sample an id from the cumulative distribution, by means of binary
   search */
unsigned int cum_distr_sample(cum_distr_t *d){
  
  long double val;
  unsigned int high, low, cur;
  
  val = d->sum * rand() / RAND_MAX;
  
  cur = d->num / 2;
  
  low = 0;
  high = d->num - 1;

  while (low < high){
    if (d->v[cur].value < val){
      low = cur+1;
    }
    else if(d->v[cur].value >= val){
      high = cur;
    }
    cur = (high + low) / 2;
  }
  return d->v[cur].id;
}



void cum_distr_dump(cum_distr_t *d){
  
  unsigned int i;
  
  
  for(i=0; i< d->num; i++){
    fprintf(stderr, "(%d, %g)\n", d->v[i].id, (double)(d->v[i].value));
  }
}


void cum_distr_destroy(cum_distr_t *d){
  
  free(d->v);
  free(d);
}
