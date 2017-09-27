bet_dependency(1) -- Compute the betweenness dependency of nodes
======

## SYNOPSIS

`bet_dependency` <graph_in> [ <node_start> [<node_end>]]


## DESCRIPTION

`bet_dependency` computes the betweenness dependency of all the nodes
of an undirected graph provided as input. The program implements the
algorithm by U. Brandes, and computes the betweenness dependency using
all the shortest paths originating from the subset of the nodes of the
graph whose labels are in the interval [<node_start>, <node_end>]. If
<node_end> is not given, the last label of the graph is assumed. If
<node_start> is not given, use the shortest paths originating from all
the nodes of the graph.

## PARAMETERS

* <graph_in>:
    input graph (edge list) if equal to `-` (dash), read the edge list
    from STDIN.

* <node_start>:
    The label of the first node in a sequence of nodes.

* <node_end>:
    The label of the last node in a sequence of nodes.


## OUTPUT

`bet_dependency` prints on the standard output (STDOUT) the
betweenness dependency score of all the nodes, starting from the node
with label 0, one per line.


## EXAMPLES

The following command:

          $ bet_dependency er_1000_5000.txt 

computes the betweenness dependency of all the nodes of the graph
`er_1000_5000.txt`, using all the shortest paths, and prints the
result on the standard output (STOUT).

The command:
    
        $ bet_dependency er_1000_5000.txt 100 200 >node_bet_dep

will compute the betweenness dependency of the nodes in the graph
`er_1000_5000.txt`, based only on the shortest paths originating from
the nodes whose labels are in the range [100,200]. The results will be
saved in the file `node_bet_dep`.

## SEE ALSO

betweenness(1), shortest(1)

## REFERENCES
* U\. Brandes. "A Faster Algorithm for Betweenness Centrality". 
  J\. Math. Sociol. 25 (2001), 163-177.

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Chapter 6, Cambridge University Press
  (2017)

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Appendix 7, Cambridge University Press
  (2017)


## AUTHORS

(c) Vincenzo 'KatolaZ' Nicosia 2009-2017 `<v.nicosia@qmul.ac.uk>`.
