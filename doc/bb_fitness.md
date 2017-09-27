bb_fitness(1) -- Grow a random graph with the fitness model
======

## SYNOPSIS

`bb_fitness` <N> <m> <n0> [SHOW]

## DESCRIPTION

`bb_fitness` grows an undirected random scale-free graph with <N>
nodes using the fitness model proposed by Bianconi and Barabasi. The
initial network is a clique of <n0> nodes, and each new node creates
<m> new edges. The probability that a new node create an edge to node
`j` is proportional to

        a_j * k_j

where `a_j` is the attractiveness (fitness) of node `j`. The values of
node attractiveness are sampled uniformly in the interval [0,1].

## PARAMETERS

* <N>:
    Number of nodes of the final graph.

* <m>:
    Number of edges created by each new node.
    
* <n0>:
    Number of nodes in the initial (seed) graph. 

* SHOW:
    If the fourth parameter is equal to `SHOW`, the values of node
    attractiveness are printed on STDERR.

## OUTPUT

`bb_fitness` prints on STDOUT the edge list of the final graph. 

## EXAMPLES

The following command:

        $ bb_fitness 10000 3 4 > bb_fitness_10000_3_4.txt

uses the fitness model to create a random graph with <N=10000> nodes,
where each new node creates <m=3> new edges and the initial seed
network is a ring of <n0=5> nodes. The edge list of the resulting
graph is saved in the file `bb_fitness_10000_3_4.txt` (notice the
redirection operator `>`). The command:

        $ bb_fitness 10000 3 4 SHOW > bb_fitness_10000_3_4.txt 2> bb_fitness_10000_3_4.txt_fitness

will do the same as above, but it will additionally save the values of
node fitness in the file `bb_fitness_10000_3_4.txt_fitness` (notice
the redirection operator `2>`, that redirects the STDERR to the
specified file).

## SEE ALSO

ba(1), dms(1)

## REFERENCES

* G\. Bianconi, A.-L. Barabasi, " Competition and multiscaling in
  evolving networks". EPL-Europhys. Lett. 54 (2001), 436.

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Chapter 6, Cambridge University Press
  (2017)

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Appendix 13, Cambridge University Press
  (2017)


## AUTHORS

(c) Vincenzo 'KatolaZ' Nicosia 2009-2017 `<v.nicosia@qmul.ac.uk>`.

