knn_w(1) -- Compute the weighted average nearest neighbours degree function
======

## SYNOPSIS

`knn_w` <graph_in> [<NO|LIN|EXP> <bin_param>]

## DESCRIPTION

`knn_w` computes the weighted average nearest neighbours degree
function knn_w(k) of the weighted graph <graph_in> given as input. The
program can (optionally) average the results over bins of equal or
exponentially increasing width (the latter is also known as
logarithmic binning).

## PARAMETERS

* <graph_in>:
    undirected and weighted input graph (edge list). If is equal to
    `-` (dash), read the edge list from STDIN.

* NO: 
    If the second (optional) parameter is equal to `NO`, or omitted,
    the program will print on output the values of knn_w(k) for all the
    degrees in <graph_in>.

* LIN:
    If the second (optional) parameter is equal to `LIN`, the program
    will average the values of knn_w(k) over <bin_param> bins of equal
    length. 

* EXP:
    If the second (optional) parameter is equal to `EXP`, the progam
    will average the values of knn_w(k) over bins of exponentially
    increasing width (also known as 'logarithmic binning', which is
    odd, since the width of subsequent bins increases exponentially,
    not logarithmically, but there you go...). In this case,
    <bin_param> is the exponent of the increase.

* <bin_param>:
    If the second parameter is equal to `LIN`, <bin_param> is the
    number of bins used in the linear binning. If the second parameter
    is `EXP`, <bin_param> is the exponent used to determine the width
    of each bin. 

## OUTPUT

The output is in the form:

        k1 knn_w(k1)
        k2 knn_w(k2)
        ....
        
If no binning is selected, `k1`, `k2`, etc. are the degrees observed
in <graph_in>. If linear or exponential binning is required, then
`k1`, `k2`, etc. are the right extremes of the corresponding bin.

## EXAMPLES

To compute the average neanest-neighbours degree function of the US
air transportation network we can run:

          $ knn_w US_airports.net
          1 81.8
          2 30.350938
          3 15.198846
          4 15.046341
          5 13.967998
          6 16.293341
          7 11.746223
          8 11.53912
          9 7.9134643
          10 8.317504
          ....
          132 0.46248989
          136 0.47312661
          145 0.37386548
          $

Since we have not requested a binning, the program will output the
value of knn_w(k) for each of the degrees actually observed in the
input graph (the mininum degree is 1 and the maximum degree is
145). We can also ask `knn_w` to bin the results over 10 bins of equal
width by running:
    
        $ knn_w US_airports.net 10
        16 68.359133
        31 89.519255
        46 78.911709
        61 78.802765
        76 76.352358
        91 71.589354
        106 60.433329
        121 62.600988
        136 64.81641
        151 54.210494
        $

or to use instead an exponential binning: 

        $ knn_w US_airports.net EXP 1.3
        3 63.062388
        6 70.319368
        10 81.856768
        15 79.766008
        21 96.172011
        29 84.771533
        39 79.591139
        52 80.222237
        69 79.776163
        91 72.217712
        119 61.878435
        155 62.695227
        $

## SEE ALSO

knn(1), deg_seq(1)

## REFERENCES

* A\. Barrat et al. "The architecture of complex weighted
  networks". P. Natl. Acad. Sci USA 101 (2004), 3747-3752.

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Chapter 10, Cambridge University Press
  (2017)

## AUTHORS

(c) Vincenzo 'KatolaZ' Nicosia 2009-2017 `<v.nicosia@qmul.ac.uk>`.
