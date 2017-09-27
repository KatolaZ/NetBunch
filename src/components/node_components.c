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
 *  This program computes (the size of) all the components associated
 *  to a node, namely the OUT-, IN-, SCC (strongly connected
 *  component), and the WCC (weekly connected component).
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

#define COMP_IN 0x01
#define COMP_OUT 0x02
#define COMP_WCC 0x04
#define COMP_SCC 0x08
#define COMP_ALL (COMP_IN | COMP_OUT | COMP_WCC | COMP_SCC)


void usage(char *argv[]){
  printf("********************************************************************\n"
         "**                                                                **\n"
         "**                   -*-  node_components  -*-                    **\n"
         "**                                                                **\n"
         "**   Find all the components associated to a node of a directed   **\n"
         "**   graph, namely the IN-, OUT-, SCC, and WCC, and print on      **\n"
         "**   output their size and/or the list of nodes belonging to      **\n"
         "**   each component. The first parameter 'graph_in' is the name   **\n"
         "**   of the file containing the edge list of the graph.           **\n"
         "**                                                                **\n"
         "**   The second parameter 'node' is the label of the node whose   **\n"
         "**   components we are interested in. The (optional) third        **\n"
         "**   parameter <component> can be one of 'IN', 'OUT', 'INOUT',    **\n"
         "**   'SCC', 'WCC', respectively corresponding to the IN-, OUT-,   **\n"
         "**   IN- and OUT-, strongly connected and weakly connected        **\n"
         "**   component to which 'node'  belongs. If the <component> is    **\n"
         "**   'ALL', the program computes all the components to which      **\n"
         "**   'node' belongs. If no third parameter is provided, all       **\n"
         "**   the components are computed by default.                      **\n"
         "**                                                                **\n"
         "**   If SHOW is given as fourth argument, the list nodes in each  **\n"
         "**   component is dumped on output as well.                       **\n"
         "**                                                                **\n"
         "********************************************************************\n"
         " This is Free Software - You can use and distribute it under \n"
         " the terms of the GNU General Public License, version 3 or later\n\n"
         " Please visit http://www.complex-networks.net for more information\n\n"
         " (c) Vincenzo Nicosia 2010-2017 (v.nicosia@qmul.ac.uk)\n"
         "********************************************************************\n\n"
         );
  printf("Usage: %s <graph_in> <node> [<component> [SHOW]]\n\n" , argv[0]);
}

int dfs(unsigned int i, unsigned int *J_slap, unsigned int *r_slap, 
        unsigned int N, unsigned int nc, 
        unsigned int *ic, unsigned int *f, 
        char reset){
  
  static unsigned int time = 0;
  unsigned int j, s;
  
  if(reset){
    time = 0;
  }
  
  ic[i] = nc;
  s = 1;
  
  for(j=r_slap[i]; j<r_slap[i+1]; j++){
    if (ic[J_slap[j]] == 0){
      s += dfs(J_slap[j], J_slap, r_slap, N, nc, ic, f, 0);
    }
  }
  f[time] = i;
  time += 1;
  return s;
}



/**
 * 
 * Find all the components of the given graph
 *
 */

int components(unsigned int *J_slap, unsigned int *r_slap,
                unsigned int N, unsigned int **ic, 
                unsigned int **f, unsigned int **sizes){

  unsigned int nc, s;
  unsigned int i;

  *ic = malloc(N * sizeof(unsigned int));
  *f = malloc(N * sizeof(unsigned int));
  *sizes = malloc((N+1) * sizeof(unsigned int));
  
  for(i=0; i<N; i++){
    (*ic)[i] = 0;
    (*f)[i] = 0;
  }
  nc = 0;
  for(i=0; i<N; i++){
    while( i < N && (*ic) [i] != 0 )
      i += 1;
    if (i == N)
      break;
    nc += 1;
    if (nc ==1){
      s = dfs(i, J_slap, r_slap, N, nc, *ic, *f, 1);
    }
    else{
      s = dfs(i, J_slap, r_slap, N, nc, *ic, *f, 0);
    }

    (*sizes)[nc] = s;
  }
  return nc;
}



int components_rev(unsigned int *J_slap, unsigned int *r_slap,
                   unsigned int N, unsigned int **ic, 
                   unsigned int *f, unsigned int **f_T, unsigned int **sizes){
  
  unsigned int nc, s;
  unsigned int  idx;
  int i;
  
  *ic = malloc(N * sizeof(unsigned int));
  *f_T = malloc(N * sizeof(unsigned int));
  *sizes = malloc(N * sizeof(unsigned int));
  
  for(i=0; i<N; i++){
    (*ic)[i] = 0;
  }
  nc = 0;
  for(i=N-1; i>=0; i--){
    idx = f[i];
    while( (*ic) [idx] != 0 && i > 0 ){
      i -= 1;
      idx = f[i];
    }
    if (i == 0)
      break;
    nc += 1;
    if (nc == 1){
      s = dfs(idx, J_slap, r_slap, N, nc, *ic, *f_T, 1);
    }
    else{
      s = dfs(idx, J_slap, r_slap, N, nc, *ic, *f_T, 1);
    }
    (*sizes)[nc] = s;
  }
  return nc;
}


void compute_comp_out(FILE *filein, unsigned int node, char show){
  
  unsigned int N, K, size, i;
  unsigned int *J_slap, *r_slap, *ic, *f, *I, *J;

  I = J = NULL;
  K = read_ij(filein, &I, &J);
  rewind(filein);

  J_slap =  NULL;
  r_slap =  NULL;
  /* obtain the SLAP representation of the graph */
  N = convert_ij2slap(I, J, K, &r_slap, &J_slap);
  
  ic = malloc(N * sizeof(unsigned int));
  f = malloc(N * sizeof(unsigned int));
  for(i=0; i<N; i++){
    ic[i] = 0;
  }


  size = dfs(node, J_slap, r_slap, N, 1, ic, f, 1);
  
  printf("OUT: %d", size);
  if (show){
    for(i=0; i<N; i++){
      if (ic[i] == 1){
        printf(" %d", i);
      }
    }
  }
  printf("\n");
  free(ic);
  free(f);
  free(I);
  free(J);
  free(J_slap);
  free(r_slap);
}

void compute_comp_in(FILE *filein, unsigned int node, char show){

  unsigned int  N, K, size, i;
  unsigned int *J_slap, *r_slap, *ic, *f, *I, *J;

  I = J = NULL;
  
  K = read_ij(filein, &I, &J);
  rewind(filein);
  
  J_slap =  NULL;
  r_slap =  NULL;
  /* obtain the SLAP representation of the graph */
  N = convert_ij2slap(J, I, K, &r_slap, &J_slap);

  
  ic = malloc(N * sizeof(unsigned int));
  f = malloc(N * sizeof(unsigned int));
  for(i=0; i<N; i++){
    ic[i] = 0;
  }


  size = dfs(node, J_slap, r_slap, N, 1, ic, f, 1);
  
  printf("IN: %d", size);
  if (show){
    for(i=0; i<N; i++){
      if (ic[i] == 1){
        printf(" %d", i);
      }
    }
  }
  printf("\n");
  free(ic);
  free(f);
  free(I);
  free(J);
  free(J_slap);
  free(r_slap);
}




void compute_wcc(FILE *filein, unsigned int node, char show){
  
  unsigned int N, K;
  unsigned int *J_slap, *r_slap;
  unsigned int *ic, *f, *sizes;
  unsigned int j, node_comp;
  

  read_slap(filein, &K, &N, &J_slap, &r_slap);

  rewind(filein);
  
  components(J_slap, r_slap, N, &ic, &f, &sizes);
  
  node_comp = ic[node];
  
  printf("WCC: %d", sizes[node_comp]);
  if (show){
    //printf(": ");
    for(j=0; j<N; j++){
      if (ic[j] == node_comp){
        printf(" %d", j);
      }
    }
    printf("\n");
  }
  else{
    printf("\n");
  }
  free(sizes);
  free(J_slap);
  free(r_slap);
  free(ic);
  free(f);
}

void compute_scc(FILE *filein, unsigned int node, char show){

  unsigned int N, K, N1, N2;
  unsigned int *I, *J, *J_slap, *r_slap, *J_slap_T, *r_slap_T;
  unsigned int *ic, *f, *ic_T, *f_T, *sizes_T, *sizes;
  unsigned int j;
  unsigned int node_comp;

  

  I = J = NULL;
  K = read_ij(filein, &I, &J);

  rewind(filein);
    
  
  J_slap = J_slap_T = NULL;
  r_slap = r_slap_T = NULL;
  /* obtain the SLAP representation of the graph */
  N1 = convert_ij2slap(I, J, K, &r_slap, &J_slap);
  /* obtain the SLAP representation of the transposed graph */
  N2 = convert_ij2slap(J, I, K, &r_slap_T, &J_slap_T);
  N = N1 >= N2 ? N1 : N2;
  
  components(J_slap, r_slap, N, &ic, &f, &sizes);
  components_rev(J_slap_T, r_slap_T, N, &ic_T, f, &f_T, &sizes_T);
  
  node_comp = ic_T[node];
  
  
  printf("SCC: %d", sizes_T[node_comp]);
  if (show){
    for(j=0; j<N; j++){
      if (ic_T[j] == node_comp){
        printf(" %d", j);
      }
    }
    printf("\n");
  }
  else{
    printf("\n");
  }
  free(sizes);
  free(sizes_T);
  free(I);
  free(J);
  free(J_slap);
  free(r_slap);
  free(J_slap_T);
  free(r_slap_T);
  free(ic);
  free(ic_T);
  free(f);
  free(f_T);
}



int main(int argc, char *argv[]){

  unsigned int mode=0, node;
  FILE *filein;
  char show = 0;

  
  if (argc < 3){
    usage(argv);
    exit(1);
  }

  filein = openfile_or_exit(argv[1], "r", 2);

  node = atoi(argv[2]);
  
  
  if (argc < 4){
    mode = COMP_ALL;
  }
  else{
    if (!my_strcasecmp("IN", argv[3])){
      mode |= COMP_IN;
    }
    else  if (!my_strcasecmp("OUT", argv[3])){
      mode |= COMP_OUT;
    }
    else  if (!my_strcasecmp("INOUT", argv[3])){
      mode |= COMP_OUT | COMP_IN;
    }
    else  if (!my_strcasecmp("WCC", argv[3])){
      mode |= COMP_WCC;
    }
    else  if (!my_strcasecmp("SCC", argv[3])){
      mode |= COMP_SCC;
    }
    else  if (!my_strcasecmp("ALL", argv[3])){
      mode |= COMP_ALL;
    }
  }

  if (argc > 4 && !my_strcasecmp("SHOW", argv[4])){
    show = 1;
  }

  
  switch(mode){
  case COMP_IN:
    compute_comp_in(filein, node, show);
    break;
  case COMP_OUT:
    compute_comp_out(filein, node, show);
    break;
  case COMP_IN | COMP_OUT:
    compute_comp_in(filein, node, show);
    compute_comp_out(filein, node, show);
    break;
  case COMP_WCC:
    compute_wcc(filein, node, show);
    break;
  case COMP_SCC:
    compute_scc(filein, node, show);
    break;
  default:
    compute_comp_in(filein, node, show);
    compute_comp_out(filein, node, show);
    compute_wcc(filein, node, show);
    compute_scc(filein, node, show);
  };
  fclose(filein);
  
}
