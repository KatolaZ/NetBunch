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
 *  This program prints the degree sequence of a graph given as input
 *
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"


#define DISTR_ONLY 0x01
#define SEQ_ONLY   0x02 
#define BOTH       0x04


void usage(char *argv[]){
  printf("********************************************************************\n"
         "**                                                                **\n"
         "**                      -*-    deg_seq    -*-                     **\n"
         "**                                                                **\n"
         "**   Compute the degree sequence of a graph given as input.       **\n"
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
         "**    The program prints on output the degree of each node, one   **\n"
         "**    on each line.                                               **\n"
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


void print_deg_seq(FILE *fileout, unsigned int *r, unsigned int N){
  
  int i;

  for(i=1; i<N +1; i ++)
    fprintf(fileout, "%d\n", r[i] - r[i-1] );
}


int main(int argc, char *argv[]){

  unsigned int *J_slap=NULL, *r_slap=NULL;
  unsigned int K, N;
  FILE *filein;
  
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

  read_slap(filein, &K, &N, &J_slap, &r_slap);

  fclose(filein);
  
  print_deg_seq(stdout, r_slap, N);
  free(J_slap);
  free(r_slap);
}
