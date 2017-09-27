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
 *  This program samples a multigraph (i.e., a graph that might
 *  contain self-loops and/or multiple edges) from the configuration
 *  model, using the degree sequence provided as input.
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"
#include "iltree.h"



/* Usage*/
void usage(char *argv[]){
  printf("********************************************************************\n"
         "**                                                                **\n"
         "**                -*-  conf_model_deg_nocheck  -*-                **\n"
         "**                                                                **\n"
         "**  Sample a multigraph from the configuration model associated   **\n"
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
         "**   prints on output the edge-list of the resulting graph. The   **\n"
         "**   final graph might contain self-loops and multiple edges.     **\n"
         "**                                                                **\n"
         "********************************************************************\n"
         " This is Free Software - You can use and distribute it under \n"
         " the terms of the GNU General Public License, version 3 or later\n\n"
         " Please visit http://www.complex-networks.net for more information\n\n"
         " (c) Vincenzo Nicosia 2010-2017 (v.nicosia@qmul.ac.uk)\n"
         "********************************************************************\n\n"
         );
  printf("Usage: %s <degs>\n\n" , argv[0]);
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


/* sample a multigraph from the configuration model */

void sample_graph_conf_nocheck(unsigned int *nodes, int N){
  
  unsigned int S;
  unsigned int i, j;
  unsigned int n_stubs;
  unsigned int *stubs;
  unsigned int L1, L2, I1, I2, tmp;
  
  n_stubs = count_stubs(nodes, N);
  
  if (n_stubs %2){
    fprintf(stderr, "The sequence is not even!\n");
    exit(3);
  }

  stubs = malloc(n_stubs * sizeof(unsigned int));
  
  /*
   * We first initialize the stubs
   */
  
  S = 0;
  for(i=0; i<N; i ++){
    for(j=0; j<nodes[i]; j ++){
      stubs[S] = i;
      S += 1;
    }
  }
  while (S > 0){
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

    if (I1 > I2){
      tmp = I2;
      I2 = I1;
      I1 = tmp;
    }
    
    printf("%d %d\n", I1, I2);
    /* And then we remove the two stubs */
    stubs[L1] = stubs[S+1];
    stubs[L2] = stubs[S];
  }
  free(stubs);
}



int main(int argc, char *argv[]){
  
  unsigned int  *nodes= NULL;
  int N;
  FILE *filein;

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
  
  N = read_deg_seq(filein, &nodes);
  fclose(filein);
  sample_graph_conf_nocheck(nodes, N);
  free(nodes);
}
