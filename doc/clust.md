clust(1) -- Compute the graph and node clustering coefficients
======

## SYNOPSIS

`clust` <graph_in> [SHOW]

## DESCRIPTION

`clust` computes the clustering coefficient of the undirected graph
given as input in the file <graph_in>. If `SHOW` is provided as a
second parameter, the program prints on STDERR the label, degree, and
clustering coefficient of all the nodes in <graph_in>.

## PARAMETERS

* <graph_in>:
    undirected input graph (edge list). If is equal to `-` (dash), read
    the edge list from STDIN.

* SHOW: 
    If the second (optional) parameter is equal to `SHOW`, the program
    will dump on the standard error the label, degree, and clustering 
    coefficient of each node in <graph_in>.

## OUTPUT

If only <graph_in> is specified, then the output is a single line,
containing the clustering coefficient of the undirected graph provided
as input. If `SHOW` is specified, the program will print on the
standard output one line for each node, in the format:

    node_1 k_1 c_1
    node_2 k_2 c_2
    node_3 k_3 c_3
     ....
    
where `node_1` is the label of the node, `k_1` is its degree, and
`c_1` is its node clustering coefficient. 

## EXAMPLES

The most simple way of using `clust` is to compute only the clustering
coefficient of a graph. For instance:

          $ clust er_1000_5000.txt 
          0.01034196
          $

will show on output the clustering coefficient of the graph
`er_1000_5000.txt`. In order to obtain the clustering coefficient of
all the nodes, we should use:

          $ clust er_1000_5000.txt SHOW 
          0 10 0.0222222
		  1 3 0
          2 7 0
          3 5 0
          4 10 0
          5 17 0
          6 14 0
          7 8 0
          8 6 0
          9 11 0
          10 9 0
          11 10 0
          12 13 0.0128205
	       ....
          998 10 0.0222222
          999 9 0
          0.01034196
          $

The last line printed on output is still the value of the clustering
coefficient of the graph, while the previous 1000 lines (which are
printed on STDERR) contain the label, degree, and clustering
coefficient of all the nodes. For instance, the first line indicates
that node `0` has degree equal to `10` and clustering coefficient
equal to `0.0222222`. It is more convenient to save the values of node
clustering coefficients in a file, e.g.:

          $ clust er_1000_5000.txt SHOW  2> er_1000_5000.txt_node_clust
          0.01034196
          $

In this case, the program prints on output only the graph clustering
coefficient `0.01034196`, while the node clustering coefficients are
saved on the file `er_1000_5000.txt_node_clust` (notice the syntax `2>
er_1000_5000.txt_node_clust`, which redirects the STDERR to the file
`er_1000_5000.txt_node_clust`). 

## SEE ALSO

clust_w(1)

## REFERENCES

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Chapter 4, Cambridge University Press
  (2017)


## AUTHORS

(c) Vincenzo 'KatolaZ' Nicosia 2009-2017 `<v.nicosia@qmul.ac.uk>`.
