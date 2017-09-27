ba(1) -- Grow a Barabasi-Albert scale-free random graph
======

## SYNOPSIS

`ba` <N> <m> <n0>

## DESCRIPTION

`ba` grows an undirected random scale-free graph with <N> nodes using
the linear preferential attachment model proposed by Barabasi and
Albert. The initial network is a ring of <n0> nodes, and each new node
creates <m> new edges. The resulting graph will have a scale-free
degree distribution, whose exponent converges to `gamma=3.0` for large
<N>.

## PARAMETERS

* <N>:
    Number of nodes of the final graph.

* <m>:
    Number of edges created by each new node.
    
* <n0>:
    Number of nodes in the initial (seed) graph. 

## OUTPUT

`ba` prints on STDOUT the edge list of the final graph.

## EXAMPLES

The following command:

        $ ba 10000 3 5 > ba_10000_3_5.txt

creates a Barabasi-Albert scale-free graph with <N=10000> nodes, where
each new node creates <m=3> new edges and the initial seed network is
a ring of <n0=5> nodes. The edge list of the graph is saved in the
file `ba_10000_3_5.txt` (thanks to the redirection operator `>`).

## SEE ALSO

bb_fitness(1), dms(1), bbv(1)

## REFERENCES

* A\.-L. Barabasi, R. Albert, "Emergence of scaling in random
  networks", Science 286, 509-512 (1999).

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Chapter 6, Cambridge University Press
  (2017)

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Appendix 13, Cambridge University Press
  (2017)


## AUTHORS

(c) Vincenzo 'KatolaZ' Nicosia 2009-2017 `<v.nicosia@qmul.ac.uk>`.

