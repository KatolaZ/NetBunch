shortest(1) -- Compute the distance between one node and all the other nodes of a graph
======

## SYNOPSIS

`shortest` <graph_in> <node> [SHOW]

## DESCRIPTION

`shortest` computes the distance (and the shortest paths) between a
given node and all the other nodes of an undirected graph provided as
input. The program implements the Breadth-First Search algorithm.

## PARAMETERS

* <graph_in>:
    input graph (edge list) if equal to `-` (dash), read the edge list
    from STDIN.

* <node>:
    The label of the node from which distances are to be computed

* SHOW: 
    If the third (optional) parameter is equal to `SHOW`, the program
    will dump on the standard error also all the shortest paths
    between <node> and all the other nodes of the graph

## OUTPUT

`shortest` prints on the standard output the distances betwen <node>
and all the other nodes of the graph, in the format:

    d0 d1 d2 d3.....
    
where `d0` is the distance to node `0`, `d1` is the distance to node
`1`, and so forth. If `SHOW` is given, the list of all the shortest
paths between <node> and the other nodes is printed on the standard
error, one path per line, in the format:
    
    label0 label1 label2 ... node
    
where `label1`, `label2`, etc. are the labels of a shortest path
between `label0` and <node>

## EXAMPLES

The following command:

          $ shortest er_1000_5000.net 25 
          3 4 4 4 2.......
          $

will show on output the distances between node 25 and all the other
nodes in the graph `er_1000_5000.net`. If we invoke the program with:

          $ shortest er_1000_5000.net 25 SHOW 2>er_1000_5000.net_25_paths
          3 4 4 4 2.......
          $

the program will dump on STDERR the list of all the shortest paths
between 'node' and all the other nodes of the graph. Since we used the
redirection `2>er_1000_5000.net_25_paths` (which can be read "redirect
STDERR to 'er\_1000\_5000.net\_25\_paths' "), the list of shortest
paths will be written to the file `er_1000_5000.net_25_paths`.

## SEE ALSO

dijkstra(1), bet_dependency(1), betweenness(1), shortest_avg_max_hist(1)

## REFERENCES

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Chapter 3, Cambridge University Press
  (2017)

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Appendix 6, Cambridge University Press
  (2017)


## AUTHORS

(c) Vincenzo 'KatolaZ' Nicosia 2009-2017 `<v.nicosia@qmul.ac.uk>`.
