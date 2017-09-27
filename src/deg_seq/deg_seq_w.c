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
 *  This program prints the degree and the strength sequence of a
 *  graph given as input.
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"



void usage(char *argv[]){
  printf("********************************************************************\n"
         "**                                                                **\n"
         "**                      -*-  deg_seq_w    -*-                     **\n"
         "**                                                                **\n"
         "**   Compute the degree and the strength sequence of a graph.     **\n"
         "**                                                                **\n"
         "**   The input file 'graph_in' is an edge-list:                   **\n"
         "**                                                                **\n"
         "**                        I_1 J_1 W_1                             **\n"
         "**                        I_2 J_2 W_2                             **\n"
         "**                        I_3 J_3 W_3                             **\n"
         "**                        ... ...                                 **\n"
         "**                        I_K J_K W_K                             **\n"
         "**                                                                **\n"
         "**    If 'graph_in' is equal to '-' (dash), read the file from    **\n" 
         "**    the standard input (STDIN).                                 **\n"
         "**                                                                **\n"
         "**    The program prints on output the degree and strength of     **\n"
         "**    all the nodes, in the format:                               **\n"
         "**                                                                **\n"
         "**        k_1 s_1                                                 **\n"
         "**        k_2 s_2                                                 **\n"
         "**        .....                                                   **\n"
         "**                                                                **\n"
         "**    where 'k_i' is the degree of the i-th node, and 's_i' is    **\n"
         "**    its strength (sum of the weights of incident edges).        **\n"
         "**                                                                **\n"
         "********************************************************************\n"
         " This is Free Software - You can use and distribute it under \n"
         " the terms of the GNU General Public License, version 3 or later\n\n"
         " Please visit http://www.complex-networks.net for more information\n\n"
         " (c) Vincenzo Nicosia 2009-2017 (v.nicosia@qmul.ac.uk)\n"
         "********************************************************************\n\n"
         );
  printf("Usage: %s <graph_in>\n" , argv[0]);
}


int main(int argc, char *argv[]){

  FILE *filein;
  unsigned int K, N, i, j, k;
  double str;
  
  unsigned int *J_slap = NULL, *r_slap=NULL;
  double *W_slap = NULL;
  
  if (argc < 2){
    usage(argv);
    exit(1);
  }
  
  
  if(!strcmp(argv[1], "-")){
    filein = stdin;
  }
  else{
    filein = openfile_or_exit(argv[1], "r", 2);
  }
  
  read_slap_w(filein, &K, &N, &J_slap, &r_slap, &W_slap);

  fclose(filein);
  
  for(i=0; i<N; i++){
    k = r_slap[i+1] - r_slap[i];
    str = 0;
    for(j=r_slap[i]; j< r_slap[i+1]; j++){
      str += W_slap[j];
    }
    fprintf(stdout, "%d %0.18g\n", k, str);
  }

  free(J_slap);
  free(r_slap);
  free(W_slap);
  
}
