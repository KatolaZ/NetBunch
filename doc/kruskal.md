kruskal(1) -- Find the minimum/maximum spanning tree of a graph
======

## SYNOPSIS

`kruskal` <graph_in> [MAX]

## DESCRIPTION

`kruskal` computes the minimum (or maximum) spanning tree of
<graph_in>, using the Kruskal's algorithm. If <grahp_in> is
unweighted, `kruskal` computes one of the spanning trees of the
graph. The program prints on output the (weighted) edge list of the
spanning tree.

## PARAMETERS

* <graph_in>:
    undirected input graph (edge list). It must be an existing file.
    
* `MAX`:
    If the second (optional) parameter is equal to `MAX`, compute the
    maximum spanning tree. Otherwise, compute the minimum spanning tree.

## OUTPUT

The program prints on STDOUT the edge list of the minimum (maximum)
spannig tree of <graph_in>, in the format:

        i_1 j_1 w_ij_1
        i_2 j_2 w_ij_2
        ....


## EXAMPLES

To find the minimum spanning tree of the graph `stocks_62_weight.net`
(the network of stocks in the New York Exchange market) we use the
command:

        $ kruskal stocks_62_weight.net
        52 53 0.72577357
        43 53 0.72838212
        2 53 0.72907212
        ...
        36 53 0.7973488
        53 58 0.79931683
        26 27 0.8029602
        $
        
which prints on output the edge list of the minimum spanning tree.
However, since the weight of each edge in that graph indicates the
similarity in the behaviour of two stocks, the maximum spanning tree
contains information about the backbone of similarities among
stocks. To obtain the maximum spannin tree, we just specify `MAX` as
second parameter:

        $ kruskal stocks_62_weight.net MAX
        56 58 1.523483
        2 52 1.3826744
        32 51 1.3812241
        ...
        33 55 0.86880272
        7 28 0.8631584
        1 53 0.81876166
        $

## SEE ALSO

clust_w(1), dijkstra(1), largest_component(1)

## REFERENCES


* J\. B. Kruskal. "On the shortest spanning subtree of a graph and the
  traveling sales-man problem". P. Am. Math. Soc. 7 (1956),
  48-48.

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Appendix 20, Cambridge University Press
  (2017)

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Chapter 10, Cambridge University Press
  (2017)

## AUTHORS

(c) Vincenzo 'KatolaZ' Nicosia 2009-2017 `<v.nicosia@qmul.ac.uk>`.
