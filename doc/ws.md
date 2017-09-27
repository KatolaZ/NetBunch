ws(1) -- Create a small-world graph using the Watts-Strogatz model
======

## SYNOPSIS

`ws` <N> <m> _p_ [SHOW]

## DESCRIPTION

`ws` creates a small-world undirected graph with 'N' nodes using the
Watts-Strogatz small-world network model.  The nodes are initially
placed around a circle and each node is connected to its 'm' closest
neighbours on either side. Then, each edge is rewired (independently)
with probability 'p'. The program prints on output the edge-list of
the resulting graph.

## PARAMETERS

* <N>:
    Number of nodes in the final graph.

* <m>:
    Number of neighbours on the circle graph to which a node is
    connected on either side (i.e., each node will initially have
    '2m' edges).

* _p_:
    Edge rewiring probability.

* SHOW:
    If the fourth (optional) parameter is equal to `SHOW`, the program
    will print on STDERR the number of edges rewired while constructing
    the graph.

## EXAMPLES

The command:

          $ ws 100 3 0.1
          0 95
          0 2
          0 3
          1 2
          1 24
          1 4
          2 3
          ....
          99 0
          99 56
          99 2
          $
          
creates a Watts-Strogatz small-world network with 'N=100' nodes and
'K=m*N=300' edges, where each edge is independently rewired with
probability 'p=0.1'. The output of `ws` is the edge-list of the
resulting graph. Notice that in this case the first edge od node `0`
has been rewired and connected to node `95`.  In order to be useful,
such edge-list should be saved into a file. The following command:

          $ ws 100 300 0.1 SHOW > ws_100_3_0.1.net
          Rewired edges: 32
          $ 

will save the resulting graph in the file `ws_100_3_0.1.net`. Notice
the usage of the symbol "\>" to redirect the output of the program to
a file. The line printed on output reports the number of rewired
edges, since `SHOW` was specified as a fourth parameter.

## SEE ALSO

er\_A(1), er\_B(1)

## REFERENCES

*  D\. J. Watts and S. H. Strogatz. "Collective dynamics of
   'small-world' networks".  Nature 393 (1998), 440–442.

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Chapter 4, Cambridge University Press
  (2017)


## AUTHORS

(c) Vincenzo 'KatolaZ' Nicosia 2009-2017 `<v.nicosia@qmul.ac.uk>`.
