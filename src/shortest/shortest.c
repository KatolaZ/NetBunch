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
 *  This program computes the distance from a given node to all the
 *  other nodes of an undirected graph, using the Breadth-First Search
 *  algorithm. 
 *
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"


void usage(char *argv[]){
  printf("********************************************************************\n"
         "**                                                                **\n"
         "**                        -*-  shortest  -*-                      **\n"
         "**                                                                **\n"
         "**  Compute the distance from the given 'node' to all the other   **\n"
         "**  nodes of an undirected graph. The first parameter 'graph_in'  **\n"
         "**  is the name of the file containing the edge list of the       **\n"
         "**  graph. The second parameter 'node' is the label of the node   **\n"
         "**  for which we want to compute the distances to all the other   **\n"
         "**  nodes.                                                        **\n"
         "**                                                                **\n"
         "**  If 'graph_in' is equal to '-' (dash), read the edge list      **\n"
         "**  from standard input (STDIN)                                   **\n"
         "**                                                                **\n"
         "**  The program prints on output a row of values:                 **\n"
         "**                                                                **\n"
         "**        d0 d1 d2 d3 d4......                                    **\n"
         "**                                                                **\n"
         "**  where d0 is the distance between 'node' and '0', 'd1' is the  **\n"
         "**  distance between 'node' and '1', and so on.                   **\n"
         "**                                                                **\n"         
         "**  If the third parameter is equal to 'SHOW', the program will   **\n"         
         "**  dump all the shortest paths from 'node' to all the other      **\n"
         "**  nodes on the standard error (STDERR).                         **\n"
         "**                                                                **\n"
         "********************************************************************\n"
         " This is Free Software - You can use and distribute it under \n"
         " the terms of the GNU General Public License, version 3 or later\n\n"
         " Please visit http://www.complex-networks.net for more information\n\n"
         " (c) Vincenzo Nicosia 2009-2017 (v.nicosia@qmul.ac.uk)\n"
         "********************************************************************\n\n"
         );
  printf("Usage: %s <graph_in> <node> [SHOW]\n\n" , argv[0]);
}


/*
 * Add 'k' to a list of predecessors
 */

void add_predecessor(unsigned int **pred, unsigned int k){
  
  (*pred)[0] += 1;
  *pred = realloc(*pred, ((*pred)[0] + 1)  * sizeof(unsigned int));
  (*pred)[ (*pred)[0] ] = k;
}



/*
 *  
 * This is the implementation of the Breadth-First Search algorithm
 * (BFS) to compute the shortest paths, and the distances, between a
 * given node 'i' and all the other nodes of a graph. 
 * 
 */
unsigned int** compute_shortest_paths(unsigned int N, unsigned int *J_slap, unsigned int *r_slap, 
                            unsigned int i, unsigned int **dist){
  
  unsigned int j, k, cur_node;
  unsigned int  *marked, **preds;
  unsigned int d;
  unsigned int n, nd, ndp;
  
  *dist = malloc(N * sizeof(unsigned int));
  marked = malloc(N * sizeof(unsigned int));
  preds = malloc(N * sizeof(unsigned int *));
  
  for(j=0; j<N; j ++){
    (*dist)[j] = N;
    preds[j] = malloc(sizeof(unsigned int));
    preds[j][0] = 0; /* The list of predecessors is empty! */
  }
  (*dist)[i] = 0;
  marked[0] = i;
  d = 0;
  n = 0;
  nd = 1;
  ndp = 0;
  while (d<N && nd > 0){
    for(k = n; k< n+nd; k ++){
      cur_node = marked[k];
      for (j=r_slap[cur_node]; j<r_slap[cur_node +1] ; j++){
        if ( (*dist)[ J_slap[j] ] == d+1){
          add_predecessor((unsigned int **)(preds + J_slap[j]), cur_node);
        }
        if ( (*dist)[ J_slap[j] ] == N){
          (*dist)[ J_slap[j] ] = d+1;
          marked[n + nd + ndp] = J_slap[j];
          add_predecessor(preds + J_slap[j], cur_node);
          ndp +=1;
        }
      }
    
    }
    n = n + nd;
    nd = ndp;
    ndp = 0;
    d += 1; 
  }
  free(marked);
  return preds;
}

/*
 * Dump on output the distances between 'node' and all the other nodes
 * of the graph
 *
 */

void dump_dists(unsigned int *dists, unsigned int N){

  unsigned int i;
  for (i=0; i<N; i++){
    printf("%d ", dists[i]);
  }
  printf("\n");
}


/*
 * recursively show the shortest paths from 'node' to all the other
 * nodes ot the graph
 *
 */
void recursive_show_paths(unsigned int **preds, unsigned int N, unsigned int k, 
                          char *buff, int pos, FILE *fileout){
  
  int i;
  char lbuff[10];

  
  if (preds[k][0] == 0){
    sprintf(buff + pos, "%5d\n", k);
    fprintf(fileout, "%s", buff );
    return;
  }
 
  sprintf(lbuff, "%5d  ", k);
  strncpy(buff + pos, lbuff, 7);
  for(i=1; i<= preds[k][0]; i ++){
    recursive_show_paths(preds, N, preds[k][i], buff, pos + 7, fileout);
  }
  return;
}

/*
 *
 * This function calls recursive_show_paths() for each of the nodes of
 * the graph, to dump the shortest paths between 'node' and all the
 * other nodes of the graph.
 *
 */

void show_paths(unsigned int **preds, unsigned int N, FILE *fileout){
  
  int j;
  char buff[256];

  for (j = 0; j<N; j++){
    if (preds[j][0] > 0)
      recursive_show_paths(preds, N, j, buff, 0, fileout);
  }
  
}

int main(int argc, char *argv[]){
  
  unsigned int *J_slap=NULL, *r_slap=NULL;
  unsigned int K, N, i;
  unsigned int **preds, *dists=NULL;
  FILE *filein;
  
  if (argc < 3){
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
  i = atoi(argv[2]);
  if (i>N){
    printf("Node id '%d' does not exist!!!! Exiting....\n", i);
    exit(3);
  }

  fclose(filein);
  
  preds = compute_shortest_paths(N, J_slap, r_slap, i, &dists);
  dump_dists(dists, N);
  /* check if we should dump the shortest paths on stderr */
  if (argc > 3 && !strcmp(argv[3], "SHOW")){
    show_paths(preds, N, stderr);
  }

  /* Cleanup */

  for (i=0; i<N; i++){
    free(preds[i]);
  }
  free(preds);
  free(dists);
  free(J_slap);
  free(r_slap);
}
