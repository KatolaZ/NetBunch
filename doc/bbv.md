bbv(1) -- Grow a weighted scale-free random graph
======

## SYNOPSIS

`bbv` <N> <m> <n0> <w0> <delta>

## DESCRIPTION

`bbv` grows an undirected weighted random scale-free graph with <N>
nodes using the model proposed by Barrat, Barthelemy, and
Vespignani. The initial network is a clique of <n0> nodes, and each
new node creates <m> new edges, each with weight <w0>.  The parameter
<delta> sets the amount of weight to be redistributed in the
neighbourhood of newly-connected nodes.

## PARAMETERS

* <N>:
    Number of nodes of the final graph.

* <m>:
    Number of edges created by each new node.
    
* <n0>:
    Number of nodes in the initial (seed) graph. 

* <w0>:
    Weight of each new edge (must be >=0)

* <delta>: 
    The amount of weight to be redistributed among the neighbours of
    newly-connected nodes.

## OUTPUT

`bbv` prints on STDOUT the edge list of the final graph, which
consists of three columns:

        node1 node2 weight

where `weight` is the weight of the corresponding edge. Please note
that each edge is printed only once. 

## EXAMPLES

The following command:

        $ bbv 10000 3 5 1.0 0.5 > bbv_10000_3_5_1.0_0.5.txt

creates a weighted scale-free graph with <N=10000> nodes, where each
new node creates <m=3> new edges and the initial seed network is a
ring of <n0=5> nodes. Each new edge has an initial weight equal to
<w0>. The weights of existing edges are rearranged after the addition
of a new edge, by rearranging an amount of weight equal to <delta>. 
The final graph is saved in the file `bbv_10000_3_5_1.0_0.5.txt`
(notice the STDOUT redirection operator `>`).


## SEE ALSO

ba(1), bb_fitness(1), dms(1)

## REFERENCES

* A\. Barrat, M. Barthelemy, and A. Vespignani. "Weighted Evolving
  Networks: Coupling Topology and Weight Dynamics".
  Phys. Rev. Lett. 92 (2004), 228701.

* A\. Barrat, M. Barthelemy, and A. Vespignani. "Modeling the
  evolution of weighted networks". Phys. Rev. E 70 (2004), 066149.

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Chapter 6, Cambridge University Press
  (2017)

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Appendix 13, Cambridge University Press
  (2017)


## AUTHORS

(c) Vincenzo 'KatolaZ' Nicosia 2009-2017 `<v.nicosia@qmul.ac.uk>`.

