largest_component(1) -- Find largest connected component of a graph
======

## SYNOPSIS

`largest_component` <graph_in>

## DESCRIPTION

`largest_component` computes the largest connected component of
<graph_in>, and prints on STDOUT the corresponding edge list.

## PARAMETERS

* <graph_in>:
    undirected input graph (edge list). If it is equal to  `-` (dash),
    read the edge list from STDIN.
    

## OUTPUT

The program prints on STDOUT the edge list of the largest connected
component of  <graph_in>, in the format:

        i_1 j_1 
        i_2 j_2 
        ....


## SEE ALSO

components(1), node_components(1), strong_conn(1)

## REFERENCES

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Chapter 6, Cambridge University Press
  (2017)
  
* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Appendix 8, Cambridge University Press
  (2017)


## AUTHORS

(c) Vincenzo 'KatolaZ' Nicosia 2009-2017 `<v.nicosia@qmul.ac.uk>`.
