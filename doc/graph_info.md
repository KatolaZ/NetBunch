graph_info(1) -- Print basic information about a graph
======

## SYNOPSIS

`graph_info` <graph_in> 

## DESCRIPTION

`graph_info` prints the number of nodes, number of edges, average
degree, and average squared degree of  <graph_in>. 


## PARAMETERS

* <graph_in>:
    undirected input graph (edge list). If is equal to `-` (dash), read
    the edge list from STDIN.

## OUTPUT

The program prints on STDOUT a single line in the format:

        N K avg_k avg_k2

where `N` is the number of nodes, `K` is the number of edges, `avg_k`
is the average degree, and `avg_k2` is the average squared degree of <graph_in>.

## SEE ALSO

clust(1)

## REFERENCES

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Cambridge University Press (2017)


## AUTHORS

(c) Vincenzo 'KatolaZ' Nicosia 2009-2017 `<v.nicosia@qmul.ac.uk>`.
