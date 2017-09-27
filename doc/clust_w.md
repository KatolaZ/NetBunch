clust_w(1) -- Compute the graph and node clustering of weighted graphs
======

## SYNOPSIS

`clust_w` <graph_in> [SHOW]

## DESCRIPTION

`clust_w` computes the clustering coefficient of the undirected and
weighted graph given as input in the file <graph_in>. The program uses
the definition of weighted clustering proposed by Barrat, Barthelemy,
Pastor-Satorras ans Vespignani.  If `SHOW` is provided as a second
parameter, the program prints on STDERR the label, degree, and
clustering coefficient of all the nodes in <graph_in>.

## PARAMETERS

* <graph_in>:
    undirected and weighted input graph (edge list). If it is equal to 
    `-` (dash), read the edge list from STDIN.

* SHOW: 
    If the second (optional) parameter is equal to `SHOW`, the program
    will dump on the standard error the label, degree, and clustering 
    coefficient of each node in <graph_in>.

## OUTPUT

If only <graph_in> is specified, then the output is a single line,
containing the clustering coefficient of the undirected and weighted
graph provided as input. If `SHOW` is specified, the program will
print on the standard output one line for each node, in the format:

    node_1 k_1 c_1
    node_2 k_2 c_2
    node_3 k_3 c_3
     ....
    
where `node_1` is the label of the node, `k_1` is its degree, and
`c_1` is its node clustering coefficient. 

## EXAMPLES

The most simple way of using `clust` is to compute only the clustering
coefficient of a graph. For instance, the command:

          $ clust_w US_airports.net
          0.01034196
          $

computes the weighted clustering coefficient of the graph
`US_airports.net` (the US air transportation network). In order to
obtain the weighted clustering coefficient of all the nodes, we should
use:

          $ clust_w US_airports.net SHOW 
          0 145 0.30493676524961
          1 136 0.364315549333
          2 132 0.3443612411812
          3 76 0.52204673222936
          ....
          497 2 1
          498 2 1
          499 1 0
          0.65111741
          $

The last line printed on output is still the value of the weighted
clustering coefficient of the graph, while the previous 500 lines
(which are printed on STDERR) contain the label, degree, and weighted
clustering coefficient of each node. For instance, the first line
indicates that node `0` has degree equal to `145` and clustering
coefficient equal to `0.30493676524961`. It is more convenient to save
the values of node clustering coefficients in a file, e.g.:

          $ clust_w US_airports.net SHOW 2> node_clust_w
          0.65111741
          $

In this case, the program prints on output only the weighted graph
clustering coefficient `0.65111741`, while the node clustering
coefficients are saved on the file `node_clust_w` (notice the syntax
`2> node_clust_w`, which redirects the STDERR to the file
`node_clust_w`).

## SEE ALSO

clust(1)

## REFERENCES

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Chapter 10, Cambridge University Press
  (2017)


## AUTHORS

(c) Vincenzo 'KatolaZ' Nicosia 2009-2017 `<v.nicosia@qmul.ac.uk>`.
