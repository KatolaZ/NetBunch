betweenness(1) -- Compute the betweenness centrality of nodes and edges
======

## SYNOPSIS

`betweenness` <graph_in> [ SEQ <node_start> [<node_end>]]

`betweenness` <graph_in> [ RND <num>]

## DESCRIPTION

`betweenness` computes the betweenness centrality of all the nodes and
edges of an undirected graph provided as input. The program implements
the algorithm by U. Brandes, and computes the betweenness using all
the shortest paths originating from a subset of the nodes of the
graph, either in a sequence (if _SEQ_ is the second parameter) or
sampled unirofmly at random (if _RND_ is the second parameter). If
<graph_in> is the only parameter, `betweenness` takes into account all
the shortest paths.

## PARAMETERS

* <graph_in>:
    input graph (edge list) if equal to `-` (dash), read the edge list
    from STDIN.

* <node_start>:
    The label of the first node in a sequence of nodes (when _SEQ_ is
    the second parameter).

* <node_end>:
    The label of the last node in a sequence of nodes (when _SEQ_ is
    the second parameter).

* <num>: 
    The number of nodes to be considered (when _RND_ is the second 
    parameter)

## OUTPUT

`betweenness` prints on the standard output (STDOUT) the betweenness
centrality of all the nodes, starting from the node with label 0, and
on the standard error (STDERR) the betweenness centrality of all the
edges, one edge per line, in the format:

    node1 node2 bet_score


## EXAMPLES

The following command:

          $ betweenness er_1000_5000.txt >node_bet 2>edge_bet

computes the node and edge betweenness centrality of the graph
`er_1000_5000.txt`, using all the shortest paths. Since we used the
redirections `>node_bet` and `2>edge_bet`, the values of node and edge
betweenness will be saved in the files `node_bet` and `edge_bet`,
respectively.

The command:
    
        $ betweenness er_1000_5000.txt SEQ 100 200  2>edge_bet_100_200

will compute the betweenness of nodes and edges in the graph
`er_1000_5000.txt`, based only on the shortest paths originating from
the nodes whose labels are in the range [100,200]. The node
betweenness will be printed on STDOUT, while the edge betweenness will
be saved in the file `edge_bet_100_200`.

Finally, the command:

        $ betweenness er_1000_5000.txt RND 250 >node_bet_RND 

will compute the betweenness centrality based on the shortest paths
originating from 250 nodes in the graph, sampled uniformly at
random. The node betweenness is saved in the file `node_bet_RND`,
while the values of edge betweenness are printed on STDOUT.

## SEE ALSO

bet_dependency(1), shortest(1)

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
