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
 */

#ifndef __UTILS_H__
#define __UTILS_H__

#define VALID_PTR_OR_EXIT(ptr, status) \
  if (!ptr){\
    fprintf(stderr, "Error in %s at line %d -- Exiting!!!\n", \
            __FILE__, __LINE__);\
    exit(status);\
  }


int read_deg_distr(FILE *filein, unsigned int **degs, unsigned int **Nk, double **p);

int read_deg_seq(FILE *filein, unsigned int **nodes);

int read_stubs(FILE *filein, unsigned int **S);

int read_ij(FILE *filein, unsigned int **i, unsigned int **j);

int read_ij_w(FILE *filein, unsigned int **i, unsigned int **j, double **w);

void read_slap(FILE *filein, unsigned int *K, unsigned int *N, 
               unsigned int **J_slap, unsigned int **r_slap);

void read_slap_w(FILE *filein, unsigned int *K, unsigned int *N, 
                 unsigned int **J_slap, unsigned int **r_slap, double **W_slap);

void read_slap_dir(FILE *filein, unsigned int *K, unsigned int *N, 
               unsigned int **J_slap, unsigned int **r_slap);

void read_slap_dir_incoming(FILE *filein, unsigned int *K, unsigned int *N, 
               unsigned int **J_slap, unsigned int **r_slap);


int convert_ij2slap(unsigned int *I, unsigned int *J, unsigned int K, 
                    unsigned int ** r_slap, unsigned int **J_slap);

int convert_ij2slap_N(unsigned int *I, unsigned int *J, unsigned int K,   
                      unsigned int N, unsigned int ** r_slap, 
                      unsigned int **J_slap);

int convert_ij2slap_w(unsigned int *I, unsigned int *J, double *W,  unsigned int K, 
                      unsigned int ** r_slap, unsigned int **J_slap, 
                      double **W_slap);
  

void write_edges(FILE *fileout, unsigned int *J_slap, 
                 unsigned int *r_slap, unsigned int N);
  
void write_edges_dir(FILE *fileout, unsigned int *J_slap, 
                 unsigned int *r_slap, unsigned int N);
  

void dump_deg_distr(unsigned int *degs, double *p, int n);

void dump_deg_seq(unsigned int *nodes, int N);


FILE* openfile_or_exit(char *filename, char *mode, int exitcode);

int compare_int(const void *x1, const void *x2);

int compare_double(const void *x1, const void *x2);

void print_int(void *elem);

void print_double(void *elem);

unsigned int find_max(unsigned int *, unsigned int);

int is_neigh(unsigned int *J_slap, unsigned int *r_slap, unsigned int N, 
             unsigned int i, unsigned int j);

int is_neigh_bs(unsigned int *J_slap, unsigned int *r_slap, unsigned int N, 
             unsigned int i, unsigned int j);

double get_neigh_weight(unsigned int *J_slap, unsigned int *r_slap, double *W_slap, 
                        unsigned int N,  unsigned int i, unsigned int j);

/* sort the adjacency list of neighbours of each node in ascending order */
void sort_neighbours(unsigned int *J_slap, unsigned int *r_slap, unsigned int N);

double strength(unsigned int *r_slap,  double *W_slap, int i);

void convert_slap2ij(unsigned int *J_slap, unsigned int *r_slap, int N, unsigned int *I, unsigned int *J);

/* Show a simple progress message */
void show_progress(FILE *fout, char *s, unsigned int progress, unsigned int total);


/* find the position of a neghbour of i (node j) in J_slap */
int find_neigh_in_Jslap(unsigned int *J_slap, unsigned int *r_slap, unsigned int N, 
                        unsigned int i, unsigned int j, unsigned int *ret);


/* shuffle a vector of integers in-place */
void shuffle_vector(unsigned int *v, unsigned int N);


unsigned int read_partition(FILE *fin, unsigned int N, unsigned int *part);

int degree (unsigned int *r_slap, unsigned int i);

int my_strcasecmp(const char *s1, const char *s2);

#endif /*__UTILS_H__*/
