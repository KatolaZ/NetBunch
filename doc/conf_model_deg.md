conf_model_deg(1) -- Sample a simple graph from the configuration model
======

## SYNOPSIS

`conf_model_deg` <degs> [<threshold>]

## DESCRIPTION

`conf_model_deg` samples a simple random undirected graph (i.e., a
graph without self-loops and multiple edges) from the configuration
model associated to the degree sequence provided in the input file
<degs>.

## PARAMETERS

* <degs>: 
    The name of the file containing the degree sequence. If <degs> is
    equal to '-' (dash) the degree sequence is read from STDIN
    (standard input).

* <threshold>:

    Optional number of edge stubs that are allowed to remain
    unmatched. This is useful to create graphs from too-constrained
    degree sequences (see [EXAMPLES][] below).
    

## OUTPUT

`conf_model_deg` prints on output the edge list of the sampled graph,
and reports on STDERR whether it has been able to find a graph. 


## EXAMPLES

Let us assume that the file `pl_-3.0_3_500_10000` contains a power-law
degree sequence ('gamma=-3.0', 'k_min=3', 'k_max=500', 'N=10000'), for
instance created using [power_law(1)][power_law(1)]. We can sample a
simple configuration model graph having that degree sequence using:

        $ conf_model_deg pl_-3.0_3_500_10000 > conf_model_-3.0_3_500_10000.net

where we have redirected the output (the resulting edge list) to the
file `conf_model_-3.0_3_500_10000.net`. In this case it is not
necessary to specify a threshold, since the maximum degree in the
sequence is 126, which is smaller than the structural cut-off. 

Let us assume now that the file `AS-20010316.net_degs` contains the
degree sequence of the data set `AS-20010316.net` (the graph of the
Internet at AS level in March 2001). In this case, the degree sequence
is too constrained, since the maximum degree (2277) is much larger
than the structural cut-off (see [REFERENCES][] below). So if we don't
specify a threshold, we get:

        $ conf_model_deg AS-20010316.net_degs > AS_20010316_rand.net
        Graph not found (unmatched stubs: 986 > threshold: 0)
        Graph not found (unmatched stubs: 888 > threshold: 0)
        Graph not found (unmatched stubs: 974 > threshold: 0)
        .....
        
and so on, since `conf_model_deg` is trying to match all the edge
stubs, without success. From the messages printed by `conf_model_deg`
on STDERR we notice that the number of unmatched stubs is 986 in the
first attempt, 888 in the second one, and so on, while the acceptable
threshold is set to 0. If we instead specify a reasonable threshold
(in this example set to 950):

        $ conf_model_deg AS-20010316.net_degs 950 > AS_20010316_rand.net
        Graph not found (unmatched stubs: 978 > threshold: 950)
        Found a graph (unmatched stubs: 876 <= threshold: 950)
        $

`conf_model_deg` is able to create a graph after a few
attempts. Notice that in the resulting graph 876 of the original edge
stubs will remain unmatched, meaning that the resulting graph has
876/2=438 edges less than those contained in the original graph.

## SEE ALSO

deg_seq(1), conf_model_deg_nocheck(1), power_law(1), hv_net(1)


## REFERENCES

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Chapter 5, Cambridge University Press
  (2017)

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Appendix 12, Cambridge University Press
  (2017)


## AUTHORS

(c) Vincenzo 'KatolaZ' Nicosia 2009-2017 `<v.nicosia@qmul.ac.uk>`.
