dijkstra(1) -- Compute the distance between one node and all the other nodes of a weighted graph
======

## SYNOPSIS

`dijkstra` <graph_in> <node> 

## DESCRIPTION

`dijkstra` computes the distance (and the shortest paths) between a
given node and all the other nodes of an undirected weighted graph
provided as input. The program implements the Dijkstra's algorithm.

## PARAMETERS

* <graph_in>:
    input graph (edge list) if equal to `-` (dash), read the edge list
    from STDIN.

* <node>:
    The label of the node from which distances are to be computed

## OUTPUT

`dijkstra` prints on the standard output the distances betwen <node>
and all the other nodes of the graph, in the format:

    d0 d1 d2 d3.....
    
where `d0` is the distance to node `0`, `d1` is the distance to node
`1`, and so forth. 

## EXAMPLES

The following command:

          $ dijkstra US_airports.net 0
          0 4784 5662 6603 11097 7470 4472 ....
          $

will show on output the distances between node 0 and all the other
nodes in the graph `US_airports.net` (the US air transportation
network).

## SEE ALSO

shortest(1)

## REFERENCES

* E\. W. Dijkstra. "A Note on Two Problems in Connexion with
  Graphs". Num. Math.  1 (1959), 269-271.

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Chapter 10, Cambridge University Press
  (2017)

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Appendix 6, Cambridge University Press
  (2017)


## AUTHORS

(c) Vincenzo 'KatolaZ' Nicosia 2009-2017 `<v.nicosia@qmul.ac.uk>`.
