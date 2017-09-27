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
 *  This program computes the weighted clustering coefficient as
 *  defined in Barrat, Barthelemy, Pastor-Satorras, Vespignani, PNAS
 *  101, 3747 (2004).
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

void usage(char *argv[]){

    printf("********************************************************************\n"
         "**                                                                **\n"
         "**                        -*-   clust_w   -*-                     **\n"
         "**                                                                **\n"
         "**   Compute the weighted clustering coefficient of 'graph_in'    **\n"
         "**   using the definition proposed by Barrayt, Barthelemy,        **\n"
         "**   Pastor-Satorras and Vespignani.                              **\n"
         "**                                                                **\n"
         "**   The input file 'graph_in' is an edge-list, in the format:    **\n"
         "**                                                                **\n"
         "**           I_1 J_1 W_1                                          **\n"
         "**           I_2 J_2 W_2                                          **\n"
         "**                                                                **\n"
         "**    where W_i is the weight if the edge (I_i, J_i).             **\n"
         "**                                                                **\n"
         "**    If 'graph_in' is equal to '-' (dash), read the file from    **\n" 
         "**    the standard input (STDIN).                                 **\n"
         "**                                                                **\n"
         "**    The output is a single line containing the value of the     **\n"
         "**    graph clustering coefficient of 'graph_in'.                 **\n"
         "**                                                                **\n"
         "**    If 'SHOW' is provided as second argument, the program       **\n"
         "**    on the standard error (STDERR) the label, degree, and       **\n"
         "**    node clustering coefficient of all the nodes of 'graph_in'  **\n"
         "**    in the format:                                              **\n"
         "**                                                                **\n"
         "**      node_1 k_1 c_1                                            **\n"
         "**      node_2 k_2 c_2                                            **\n"
         "**      node_3 k_3 c_3                                            **\n"
         "**      node_4 k_4 c_4                                            **\n"
         "**      .....                                                     **\n"
         "**                                                                **\n"
         "**    where 'k_1' is the degree of 'node_1' and 'c_1' is its      **\n"
         "**    node clustering coefficient. If a node has degree smaller   **\n"
         "**    than 2, its clustering coefficient is equal to zero.        **\n"
         "**                                                                **\n"
         "********************************************************************\n"
         " This is Free Software - You can use and distribute it under \n"
         " the terms of the GNU General Public License, version 3 or later\n\n"
         " Please visit http://www.complex-networks.net for more information\n\n"
         " (c) Vincenzo Nicosia 2009-2017 (v.nicosia@qmul.ac.uk)\n"
         "********************************************************************\n\n"
         );
  
  printf("Usage: %s <graph_in> [SHOW]\n", argv[0]);
}




double avg_clust_w(unsigned int *J_slap, unsigned int *r_slap, double *W_slap, 
                 unsigned int N, int show){

  double c = 0, ci=0, w=0.0;
  unsigned int i, j, l, ki;
  
  for(i=0; i<N; i ++){/* for each node i..*/
    ci = 0;
    ki = r_slap[i+1] - r_slap[i];
    for(j=r_slap[i]; j<r_slap[i+1]; j++){/*... pick a neighbour...*/
      for (l=r_slap[J_slap[j]]; l<r_slap[J_slap[j]+1]; l++){
          /*...and check how many times it has a link to a neighbour of i */
        w = get_neigh_weight(J_slap, r_slap, W_slap, N, i, J_slap[l]);
        if (J_slap[l] != i && w > 0.0){
          ci += (W_slap[j] + w) / 2.0;
        }
      }
    }
    if(ki > 1)
      ci /= strength(r_slap, W_slap, i) * (ki-1);
    else
      ci = 0;
    if (show) 
      fprintf(stderr, "%d %d %0.14g\n", i, ki, ci);
    c += ci;
  }
  c /= N;
  return c;
}




int main(int argc, char *argv[]){

  unsigned int *J_slap=NULL, *r_slap=NULL;
  double *W_slap = NULL;
  unsigned int K, N;
  FILE *filein;
  double c;
  int show = 0;

  if (argc < 2){
    printf("Usage: %s <filein> [SHOW]\n", argv[0]);
    exit(1);
  }
  if (argc  >2 ){
    show = 1;
  }
  
  if (!strcmp(argv[1], "-")){
    /* take the input from STDIN */
    filein = stdin;
  }
  else {
    filein = openfile_or_exit(argv[1], "r", 2);
  }

  
  read_slap_w(filein, &K, &N, &J_slap, &r_slap, &W_slap);
  fclose(filein);
  c = avg_clust_w(J_slap, r_slap, W_slap, N, show);
  printf("%2.8lf\n", c);
  free(J_slap);
  free(r_slap);
  free(W_slap);
}
