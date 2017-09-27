dms(1) -- Grow a scale-free random graph with tunable exponent
======

## SYNOPSIS

`dms` <N> <m> <n0> _a_

## DESCRIPTION

`dms` grows an undirected random scale-free graph with <N> nodes using
the modified linear preferential attachment model proposed by
Dorogovtsev, Mendes and Samukhin. The initial network is a clique of
<n0> nodes, and each new node creates <m> new edges. The resulting
graph will have a scale-free degree distribution, whose exponent
converges to `gamma=3.0 + a/m` for large <N>.

## PARAMETERS

* <N>:
    Number of nodes of the final graph.

* <m>:
    Number of edges created by each new node.
    
* <n0>:
    Number of nodes in the initial (seed) graph. 

* _a_:
    This parameter sets the exponent of the degree distribution
    (`gamma = 3.0 + a/m`). _a_ must be larger than <-m>.

## OUTPUT

`dms` prints on STDOUT the edge list of the final graph.

## EXAMPLES

Let us assume that we want to create a scale-free network with
<N=10000> nodes, with average degree equal to 8, whose degree
distribution has exponent

        gamma = 2.5

Since `dms` produces graphs with scale-free degree sequences with an
exponent `gamma = 3.0 + a/m`, the command:

        $ dms 10000 4 4 -2.0 > dms_10000_4_4_-2.0.txt

will produce the desired network. In fact, the average degree of the
graph will be:

        <k> = 2m = 8

and the exponent of the power-law degree distribution will be:

        gamma = 3.0 + a/m = 3.0 -0.5 = 2.5

The following command:

        $ dms 10000 3 5 0 > dms_10000_3_5_0.txt

creates a scale-free graph with <N=10000> nodes, where each new node
creates <m=3> new edges and the initial seed network is a ring of
<n0=5> nodes. The degree distribution of the final graph will have
exponent equal to `gamma = 3.0 + a/m = 3.0`. In this case, `dms`
produces a Barabasi-Albert graph (see ba(1) for details). The edge
list of the graph is saved in the file `dms_10000_3_5_0.txt` (thanks
to the redirection operator `>`).

## SEE ALSO

ba(1), bb_fitness(1)

## REFERENCES

* S\. N. Dorogovtsev, J. F. F. Mendes, A. N. Samukhin. "Structure of
    Growing Networks with Preferential Linking".  Phys. Rev. Lett. 85
    (2000), 4633-4636.

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Chapter 6, Cambridge University Press
  (2017)

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Appendix 13, Cambridge University Press
  (2017)


## AUTHORS

(c) Vincenzo 'KatolaZ' Nicosia 2009-2017 `<v.nicosia@qmul.ac.uk>`.

