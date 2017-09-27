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

#ifndef __CUM_DISTR_H__
#define __CUM_DISTR_H__

typedef struct{
  unsigned int id;
  long double value;
} idval_t;

typedef struct{
  unsigned int N;
  unsigned int num;
  idval_t *v;
  long double sum;
} cum_distr_t;

cum_distr_t* cum_distr_init(unsigned int N);

int cum_distr_add(cum_distr_t *d, unsigned int id, long double val);

unsigned int cum_distr_sample(cum_distr_t *d);

void cum_distr_dump(cum_distr_t *d);


void cum_distr_destroy(cum_distr_t *d);


#endif //__CUM_DISTR_H__
