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
 *  This program samples a graph from the Erdos-Renyi random graph
 *  model A, i.e., the ensemble of graphs with N nodes and K edges.
 *
 *
 *  References: 
 *
 *  [1] Erdős, P. & Rényi, A. On Random Graphs I Publ. Math. Debrecen,
 *      6, 290 (1959)
 *
 *  [2] Erdos, P. & Renyi, A. On the evolution of random graphs
 *      Publ. Math. Inst. Hungary. Acad. Sci., 5, 17-61 (1960)
 *
 */



#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "iltree_double.h"

  

void usage(char *argv[]){
  printf("********************************************************************\n"
         "**                                                                **\n"
         "**                        -*-     er     -*-                      **\n"
         "**                                                                **\n"
         "**       sample an Erdos-Renyi graph of N nodes and K links       **\n"
         "**                                                                **\n"
         "**                  the output is an I,J matrix:                  **\n"
         "**                            I_1 J_1                             **\n"
         "**                            I_2 J_2                             **\n"
         "**                            I_3 J_3                             **\n"
         "**                            ... ...                             **\n"
         "**                            I_K J_K                             **\n"
         "**                                                                **\n"
         "**               (output is to stdout or to a file)               **\n"
         "**                                                                **\n"
         "**      If a filename is specified as fourth parameter then       **\n"
         "**         the degree sequence is dumped out to that file         **\n"
         "**                                                                **\n"
         "**                                                                **\n"
         "********************************************************************\n"
         " This is Free Software - You can use and distribute it under \n"
         " the terms of the GNU General Public License, version 3 or later\n\n"
         " (c) Vincenzo Nicosia 2010-2017 (v.nicosia@qmul.ac.uk)\n\n"
         "********************************************************************\n\n"
         );
  printf("Usage: %s <N> <K> [<filout>]\n\n" , argv[0]);
}


void sample_edges(unsigned int N, double K, iltree_t t){
  
  long double k;
  double i, k1, k2, tmp;
  
  for(i=0; i<K; ){
    k1 = rand() % N +1; /* sample the first node uniformly in [0,N-1] */
    k2 = rand() % N +1; /* sample the first node uniformly in [0,N-1] */
    while (k1 == k2){ /* Avoid selfloops */
      k1 = rand() % N +1; /* sample the first node uniformly in [0,N-1] */
      k2 = rand() % N +1; /* sample the first node uniformly in [0,N-1] */
    }
    if (k1 > k2){
      tmp = k2;
      k2 = k1;
      k1 = tmp;
    }
    k = 1.0 * (k2-1)* (k2-2) / 2 + k1; /* This number represents the couple k1, k2 (k1 < k2)*/
    if (!iltree_lookup(t, &k)){ /* The element "k" is not yet present in the BST*/
      /* So we insert into in the tree */
      iltree_insert(t, &k);
      /* and then we increment the counter of inserted edges */
      i += 1;
    }
  }
}



int main(int argc, char *argv[]){

  iltree_t t = NULL; /* This is the insert-lookup BST */
  FILE *fileout;
  char error_str [256];
  unsigned int N;
  double K;
  
  if(argc < 3){
    usage(argv);
    exit(1);
  }

  N = atoi(argv[1]);
  K = atof(argv[2]);
  
  
  if (argc >3){
    fileout = fopen(argv[3], "w+");
    if (!fileout){
      sprintf(error_str, "Error opening file %s", argv[3]);
      perror(error_str);
      exit(2);
    }
  }
  else{
    fileout = stdout;
  }
  
  t = iltree_double_init(t, fileout);

  srand(time(NULL));
  /* Now we sample K integers without repetition (using the BST)*/
  sample_edges(N, K, t);
  /* And then we dump out the edges in I,J format */
  iltree_view_pre(t);
  iltree_destroy(t);
}
