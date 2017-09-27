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
 *  Compute and print the basic properties of the undirected graph on
 *  input, i.e., the number of nodes, the number of edges, and the
 *  first two uncentered moments of the degree sequence (<k> and
 *  <k^2>).
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"


void usage(char *argv[]){

    printf("********************************************************************\n"
         "**                                                                **\n"
         "**                        -*- graph_info -*-                      **\n"
         "**                                                                **\n"
         "**  Compute basic information about the 'graph_in', namely the    **\n"
         "**  number of nodes, number of edges, average degree, and the     **\n"
         "**  average squared degree.                                        **\n"
         "**                                                                **\n"
         "**   The input file 'graph_in' is an edge-list:                   **\n"
         "**                                                                **\n"
         "**                            I_1 J_1                             **\n"
         "**                            I_2 J_2                             **\n"
         "**                            I_3 J_3                             **\n"
         "**                            ... ...                             **\n"
         "**                            I_K J_K                             **\n"
         "**                                                                **\n"
         "**    If 'graph_in' is equal to '-' (dash), read the file from    **\n" 
         "**    the standard input (STDIN).                                 **\n"
         "**                                                                **\n"
         "**    The output is a single line containing in the format:       **\n"
         "**                                                                **\n"
         "**       N K avg_k avg_k2                                         **\n"
         "**                                                                **\n"
         "**    where 'N' is the number of nodes, 'K' is the number of      **\n"
         "**    edges, 'avg_k' is the average degree, and 'avg_k2' is the   **\n"
         "**    average of the squared degree.                              **\n"
         "**                                                                **\n"
         "********************************************************************\n"
         " This is Free Software - You can use and distribute it under \n"
         " the terms of the GNU General Public License, version 3 or later\n\n"
         " Please visit http://www.complex-networks.net for more information\n\n"
         " (c) Vincenzo Nicosia 2009-2017 (v.nicosia@qmul.ac.uk)\n"
         "********************************************************************\n\n"
         );
  printf("Usage: %s <graph_in>\n", argv[0]);
}



double ksquare_mean(unsigned int N, unsigned int *r_slap){
  
  double sum = 0;
  unsigned int i;
  unsigned int k;
  for(i=0; i<N; i++){
    k = r_slap[i+1] - r_slap[i];
    sum += k*k;
  }
  sum /= N;
  return sum;
}


int main(int argc, char *argv[]){
  
  unsigned int N, K;
  unsigned int *J_slap, *r_slap;
  FILE *filein;
  double k2mean;

  if(argc < 2){
    usage(argv);
    exit(1);
  }
  
  if (!strcmp(argv[1], "-")){
    /* take the input from STDIN */
    filein = stdin;
  }
  else {
    filein = openfile_or_exit(argv[1], "r", 2);
  }
  
  read_slap(filein, &K, &N, &J_slap, &r_slap);

  fclose(filein);
  
  k2mean = ksquare_mean(N, r_slap);
  printf("%d %d %2.6f %2.6f\n", N, K/2, 1.0*K / N, k2mean);
  free(J_slap);
  free(r_slap);
  
}
