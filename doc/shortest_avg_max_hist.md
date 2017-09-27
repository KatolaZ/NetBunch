shortest\_avg\_max\_hist(1) -- Compute the distance between one node and all the other nodes of a graph
======

## SYNOPSIS

`shortest_avg_max_hist` <graph_in> <node>

## DESCRIPTION

`shortest_avg_max_hist` computes the distance (and the shortest paths)
between a given node and all the other nodes of an undirected graph
provided as input. The program implements the Breadth-First Search
algorithm, and works almost exactly as [shortest(1)][], except for the
output.

## PARAMETERS

* <graph_in>:
    input graph (edge list) if equal to `-` (dash), read the edge list
    from STDIN.

* <node>:
    The label of the node from which distances are to be computed

## OUTPUT

The output 

## EXAMPLES

The following command:

          $ shortest_avg_max_hist er_1000_5000.net 25 
          3.2002 4 11 111 544 333
          $

is showing the average distance between node `25` and all the other
nodes in the graph (`3.2002`), the eccentricity of node `25` (equal to
`4`), and the number of nodes at distance `1` (11), `2` (111), `3`
(544) and `4` (333) from node `25`.


## SEE ALSO

shortest(1), betweenness(1), bet_dependency(1)


## REFERENCES

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Chapter 3, Cambridge University Press
  (2017)

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Appendix 6, Cambridge University Press
  (2017)


## AUTHORS

(c) Vincenzo 'KatolaZ' Nicosia 2009-2017 `<v.nicosia@qmul.ac.uk>`.
