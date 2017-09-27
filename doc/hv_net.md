hv_net(1) -- Sample a random graph with an assigned joint degree distribution
======

## SYNOPSIS

`hv_net` <graph_in> [SHOW]

## DESCRIPTION

`hv_net` samples a random graph whose joint degree distribution is
equal to that of another graph provided as input, using the
hidden-variable model proposed by Boguna ans Pastor-Satorras.

## PARAMETERS

* <graph_in>:
    File containing the edge list of the existing graph. If equal to
    '-' (dash), read the edge list from STDIN.

* SHOW:
    If the second parameter is equal to `SHOW`, the program prints on
    STDERR the hidden variable and actual degree of each node.

## EXAMPLES

Let us assume that we want to create a graph whose joint degree
distribution is equal to that of the graph contained in
`AS-20010316.net` (i.e., the graph of the Internet at the AS level in
March 2001). We can use the command:

        $ hv_net AS-20010316.net > AS-20010316.net_rand

which will sample a random graph with the same joint-degree
distribution and will save its edge list in the file
`AS-20010316.net_rand` (notice the STDOUT redirection operator
`>`). Additionally, we can also save the values of the hidden
variables and actual degrees of the nodes by specifying `SHOW` as a
second parameter:

        $ hv_net AS-20010316.net SHOW > AS-20010316.net_rand 2>AS-20010316.net_rand_hv 

In this case, the file `AS-20010316.net_rand_hv` will contain the
values of the hidden variable of each node and of the actual degree of
the node in the sampled graph, in the format:
     
         h1 k1
         h2 k2
         ....


## SEE ALSO

conf_model_deg(1), conf_model_deg_nocheck(1)

## REFERENCES

* M\. Boguna and R. Pastor-Satorras. "Class of correlated random
  networks with hidden variables". Phys. Rev. E 68 (2003), 036112.

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Chapter 7, Cambridge University Press
  (2017)

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Appendix 14, Cambridge University Press
  (2017)


## AUTHORS

(c) Vincenzo 'KatolaZ' Nicosia 2009-2017 `<v.nicosia@qmul.ac.uk>`.

