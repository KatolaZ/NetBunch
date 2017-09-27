deg_seq_w(1) -- Compute the degree and strength sequence of a graph
======

## SYNOPSIS

`deg_seq_w` <graph_in> 

## DESCRIPTION

`deg_seq_w` computes the degree and the strength sequence of <graph_in>. 


## PARAMETERS

* <graph_in>:
    undirected input graph (edge list). If is equal to `-` (dash), read
    the edge list from STDIN.

## OUTPUT

The program prints on STDOUT the degree and strength sequence of
<graph_in> in the format:

        k_1 s_1
        k_2 s_2
        k_3 s_3
        ....

where `k_i` is the degree of the i-th node and `s_i` is its strength.

## SEE ALSO

deg_seq(1), conf_model_deg(1), fitmle(1), power_law(1)


## REFERENCES

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Chapter 3, Cambridge University Press
  (2017)


## AUTHORS

(c) Vincenzo 'KatolaZ' Nicosia 2009-2017 `<v.nicosia@qmul.ac.uk>`.
