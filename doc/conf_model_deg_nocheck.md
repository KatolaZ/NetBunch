conf_model_deg_nocheck(1) -- Sample a multigraph from the configuration model
======

## SYNOPSIS

`conf_model_deg_nocheck` <degs> 

## DESCRIPTION

`conf_model_deg_nocheck` samples an undirected multigraph (i.e., a
graph that might contain self-loops multiple edges) from the
configuration model associated to the degree sequence provided in the
input file <degs>.

## PARAMETERS

* <degs>: 
    The name of the file containing the degree sequence. If <degs> is
    equal to '-' (dash) the degree sequence is read from STDIN
    (standard input).

## OUTPUT

`conf_model_deg_nocheck` prints on output the edge list of the sampled
graph.


## EXAMPLES

Let us assume that the file `AS-20010316.net_degs` contains the degree
sequence of the data set `AS-20010316.net` (the graph of the Internet
at AS level in March 2001). In this case, the degree sequence is too
constrained, since the maximum degree (2277) is much larger than the
structural cut-off (see [REFERENCES][] below). However, since
`conf_model_deg_nocheck` allows for self-loops and multiple edges, it
always finds a multigraph: 

        $ conf_model_deg_nocheck AS-20010316.net_degs > AS_20010316_rand.net
        $

## SEE ALSO

conf_model_deg(1), power_law(1), hv_net(1)

## REFERENCES

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Chapter 5, Cambridge University Press
  (2017)

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Appendix 12, Cambridge University Press
  (2017)


## AUTHORS

(c) Vincenzo 'KatolaZ' Nicosia 2009-2017 `<v.nicosia@qmul.ac.uk>`.
