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
 *  This program samples a simple graph (i.e., without self-loops
 *  and/or multiple edges) from the configuration model, using the
 *  degree sequence provided as input.
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"
#include "iltree_double.h"


#define MAXREJECTS 1000000


/* Usage */
void usage(char *argv[]){
  printf("********************************************************************\n"
         "**                                                                **\n"
         "**                   -*-  conf_model_deg  -*-                     **\n"
         "**                                                                **\n"
         "**  Sample a simple graph from the configuration model associated **\n"
         "**  to the degree sequence provided as input.                     **\n"
         "**                                                                **\n"
         "**  The degree sequence is a file with format:                    **\n"
         "**                                                                **\n"
         "**                           k_1                                  **\n"
         "**                           k_2                                  **\n"
         "**                           k_3                                  **\n"
         "**                           ....                                 **\n"
         "**                           k_N                                  **\n"
         "**                                                                **\n"
         "**   where 'k_i' is the degree of the i-th node. The program      **\n"
         "**   prints on output the edge-list of the resulting graph.       **\n"
         "**                                                                **\n"
         "**   The second (optional) parameter 'threshold' indicates the    **\n"
         "**   number of edge stubs that are allowed to remain unmatched    **\n"
         "**   in the final graph. This parameter might be used to          **\n"
         "**   obtain a graph from a too-constrained degree sequence, e.g., **\n"
         "**   a power-law sequence whose maximum degree is larger than     **\n"
         "**   the structural cut-off.                                      **\n"
         "**                                                                **\n"
         "********************************************************************\n"
         " This is Free Software - You can use and distribute it under \n"
         " the terms of the GNU General Public License, version 3 or later\n\n"
         " Please visit http://www.complex-networks.net for more information\n\n"
         " (c) Vincenzo Nicosia 2010-2017 (v.nicosia@qmul.ac.uk)\n"
         "********************************************************************\n\n"
         );
  printf("Usage: %s <degs> [<threshold>]\n\n" , argv[0]);
}


unsigned int count_stubs(unsigned int *nodes, int N){
  
  int i;
  unsigned int cnt;

  cnt = 0;
  for (i=0; i<N; i ++){
    cnt += nodes[i];
  }
  return cnt;
}


void sample_graph_conf(unsigned int *nodes, int N, iltree_t *t, 
                           unsigned int thresh){
  
  unsigned int S;
  unsigned int i, j;
  unsigned int n_stubs;
  unsigned int *stubs;
  unsigned int countreject;
  unsigned int L1, L2, I1, I2, tmp;
  long double k;
  FILE *tmp_file;
  
  n_stubs = count_stubs(nodes, N);
  
  if (n_stubs %2){
    fprintf(stderr, "The sequence is not even!\n");
    exit(3);
  }
  /* ind = malloc(n_stubs * sizeof(unsigned int)); */
  /* stub2node = malloc(n_stubs * sizeof(unsigned int)); */
  
  stubs = malloc(n_stubs * sizeof(unsigned int));
  
  
  do{
    countreject = 0;
    S = 0;
    /*
     * We first initialize the stubs
     *
     * This is needed only before the first iteration, indeed.... 
     *
     */
    for(i=0; i<N; i ++){
      for(j=0; j<nodes[i]; j ++){
        stubs[S] = i;
        S += 1;
      }
    }
    while (S > 0 && countreject < MAXREJECTS){
      /* sample the first stub */
      L1 = rand() % S;
      I1 = stubs[L1];
      S -= 1;

      /* sample the second stub */
      L2 = rand() % S;
      I2 = stubs[L2];
      S -= 1;

      if (L1 == L2){/* If L1 and L2 are the same element, discard the
                       sample */
        /* reset S to the value it had before */
        S += 2;
        continue;
      }
      
      if (I1 == I2){
        /* reject the pair: it's the same node! */
        countreject += 1;
        /* reset S to the value it had before */
        S += 2;
        continue;
      }
      
    
      if (I1 > I2){ /* we require that I1 < I2 */
        tmp = I2;
        I2 = I1;
        I1 = tmp;
      }
      k = 1.0 * (I2)* (I2-1) / 2 + I1 + 1; /* This number represents the
                                        couple I1, I2 (I1 < I2)*/
      if (!iltree_lookup(*t, &k)){ /* The element "k" is not yet present in the BST*/
        /* We can accept the new pair... */
        /* ...so we insert it into the tree */
        iltree_insert(*t, &k);
        /* remove the two stubs */
        tmp = stubs[L1];
        stubs[L1] = stubs[S+1];
        stubs[S+1] = tmp;

        tmp = stubs[L2];
        stubs[L2] = stubs[S];
        stubs[S] = tmp;
      }
      else{/* reject the pair */
        /* reset S to the value it had before */
        countreject += 1;
        S += 2;
      }
    }
    
    /* Let's check if we got a graph....*/
    if (countreject >= MAXREJECTS){
      if (S <= thresh){
        fprintf(stderr, "Found a graph (unmatched stubs: %d <= threshold: %d)\n", 
                S, thresh);
        break;
      }
      else{
        fprintf(stderr, "Graph not found (unmatched stubs: %d > threshold: %d)\n", 
                S, thresh);
        S = 0;
        countreject = 0;
        tmp_file = iltree_get_fileout(*t);
        iltree_destroy(*t);
        *t = NULL;
        *t = iltree_double_init(*t, tmp_file);
      }
    }
    else{
      fprintf(stderr, "Found a graph!\n");
      break;
    }
  } while(1);
  free(stubs);
}



int main(int argc, char *argv[]){
  
  unsigned int  *nodes= NULL;
  int N;
  iltree_t t = NULL;
  FILE *filein;
  unsigned int thresh;

  if (argc < 2){
    usage(argv);
    exit(1);
  }
  
  srand(time(NULL));

  if (!strcmp(argv[1], "-")){
    /* take the input from STDIN */
    filein = stdin;
  }
  else {
    filein = openfile_or_exit(argv[1], "r", 2);
  }
  
  if (argc > 2) /* Set the threshold */
    thresh = atoi(argv[2]);
  else
    thresh = 0;
  
  
  t = iltree_double_init(t, stdout);
  
  N = read_deg_seq(filein, &nodes);
  sample_graph_conf(nodes, N, &t, thresh);
  iltree_double_dump_edges(t);
  fclose(filein);
  free(nodes);
  iltree_destroy(t);
}
