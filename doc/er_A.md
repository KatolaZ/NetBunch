er_A(1) -- Sample a random graph from the Erdos-Renyi model A
======

## SYNOPSIS

`er_A` <N> <K> [<fileout>]

## DESCRIPTION

`er_A` samples a random graph with <N> nodes and <K> edges from the
Erdos-Renyi model A, i.e. the ensemble of random graphs where K links
are placed uniformly at random among N nodes. The program dumps the
edge list of the resulting graph on output. If the optional <fileout>
is provided, the output is written on a file with that name.

## PARAMETERS

* <N>:
    Number of nodes in the final graph.

* <K>:
    Number of edges in the final graph.

* <fileout>:
    The (optional) name of the filename where the edge list of the
    graph will be saved.

## EXAMPLES

The following command:

         $ er_A 1000 3000

samples an undirected random network with <N=1000> nodes and <K=3000>
edges using the Erdos-Renyi model A. The output of the command `er_A`
will be the edge-list of the resulting graph, where each (undirected)
edge is reported only once. In order to be useful, such edge-list
should be saved into a file. The following command:

          $ er_A 1000 3000 > er_A_1000_3000.net

will save the resulting graph in the file er\_A\_1000\_3000.net. Notice
the usage of the symbol "\>" to redirect the output of the program to a
file.

## SEE ALSO

er_B(1), ws(1)

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
