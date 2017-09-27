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
 *  model B, i.e., the ensemble of graphs with N nodes where each of
 *  the possible N(N-1)/2 edges is created with probability equal to
 *  p, and independently from all the other edges.
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

void usage(char *argv[]){
  printf("********************************************************************\n"
         "**                                                                **\n"
         "**                        -*-    er_B    -*-                      **\n"
         "**                                                                **\n"
         "**  sample an Erdos-Renyi graph of N nodes and edge probability p **\n"
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
         "********************************************************************\n"
         " This is Free Software - You can use and distribute it under \n"
         " the terms of the GNU General Public License, version 3 or later\n\n"
         " (c) Vincenzo Nicosia 2010-2017 (v.nicosia@qmul.ac.uk)\n\n"
         "********************************************************************\n\n"
         );
  printf("Usage: %s <N> <p> [<filout>]\n\n" , argv[0]);
}



int main(int argc, char *argv[]){

  unsigned int N, i, j;;
  FILE *fileout;
  double p, val;
  char error_str [256];

  if(argc < 3){
    usage(argv);
    exit(1);
  }
  
  N = atoi(argv[1]);
  p = atof(argv[2]);
  
  
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
  
  srand(time(NULL));

  /* Loop over all nodes...*/
  for(i=0; i<N; i++){
    /* Loop over the nodes with label larger than i...*/
    for(j=i+1; j<N; j++){
      val = rand() * 1.0 / RAND_MAX;
      /* if the sample is smaller than p, create the edge */
      if (val < p){
        fprintf(fileout, "%d %d\n", i, j);
      }
    }
  }
  fclose(fileout);
}
