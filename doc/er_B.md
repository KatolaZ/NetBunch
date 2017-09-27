er_B(1) -- Sample a random graph from the Erdos-Renyi model B
======

## SYNOPSIS

`er_B` <N> _p_ [<fileout>]

## DESCRIPTION

`er_B` samples a random graph from the Erdos-Renyi model B, i.e. a
graph with <N> nodes where each of the <N(N-1)/2> edges is created
independently with probability _p_. The program dumps the edge list of
the resulting graph on output. If the optional <fileout> is provided,
the output is written on a file with that name.

## PARAMETERS

* <N>:
    Number of nodes in the final graph.

* _p_:
    Edge probability.

* <fileout>:
    The (optional) name of the filename where the edge list of the
    graph will be saved.

## EXAMPLES

The following command:

          $ er_B 1000 0.006

samples an undirected random network with <N=1000> nodes where each
possible edge is created with probability _p=0.006_.  The output of
the command `er_B` will be the edge-list of the resulting graph, where
each (undirected) edge is reported only once. In order to be useful,
such edge-list should be saved into a file. The following command:

          $ er_B 1000 0.006 > er_B_1000_0.006.net

will save the resulting graph in the file
er\_B\_1000\_0.006.net. Notice the usage of the symbol "\>" to
redirect the output of the program to a file.

## SEE ALSO

er_A(1), ws(1)


## REFERENCES

* P\. Erdos, & A. Rényi, "On Random Graphs I", Publ. Math. Debrecen, 6,
  290 (1959)

* P\.Erdos, & A. Renyi, "On the evolution of random graphs"
  Publ. Math. Inst. Hungary. Acad. Sci., 5, 17-61 (1960)

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Chapter 3, Cambridge University Press
  (2017)

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Appendix 10, Cambridge University Press
  (2017)


## AUTHORS

(c) Vincenzo 'KatolaZ' Nicosia 2009-2017 `<v.nicosia@qmul.ac.uk>`.
