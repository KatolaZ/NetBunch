knn(1) -- Compute the average nearest neighbours degree function
======

## SYNOPSIS

`knn` <graph_in> [<NO|LIN|EXP> <bin_param>]

## DESCRIPTION

`knn` computes the average nearest neighbours degree function knn(k)
of the graph <graph_in> given as input. The program can (optionally)
average the results over bins of equal or exponentially increasing
width (the latter is also known as logarithmic binning).

## PARAMETERS

* <graph_in>:
    undirected input graph (edge list). If is equal to `-` (dash), read
    the edge list from STDIN.

* NO: 
    If the second (optional) parameter is equal to `NO`, or omitted,
    the program will print on output the values of knn(k) for all the
    degrees in <graph_in>.

* LIN:
    If the second (optional) parameter is equal to `LIN`, the program
    will average the values of knn(k) over <bin_param> bins of equal
    length. 

* EXP:
    If the second (optional) parameter is equal to `EXP`, the progam
    will average the values of knn(k) over bins of exponentially
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

        k1 knn(k1)
        k2 knn(k2)
        ....
        
If no binning is selected, `k1`, `k2`, etc. are the degrees observed
in <graph_in>. If linear or exponential binning is required, then
`k1`, `k2`, etc. are the right extremes of the corresponding bin.

## EXAMPLES

To compute the average neanest-neighbours degree function for a given
graph we just run:

          $ knn er_1000_5000.net 
          2 10.5
          3 11.333333
          4 10.785714
          5 11.255319
          6 11.336601
          7 11.176292
          8 11.067568
          9 11.093519
          10 10.898438
          11 10.906009
          12 11.031353
          13 10.73938
          14 10.961538
          15 10.730864
          16 10.669118
          17 10.702206
          18 10.527778
          19 11.302632
          20 11.8
          $

Since we have not requested a binning, the program will output the
value of knn(k) for each of the degrees actually observed in the graph
`er_1000_5000.net` (the mininum degree is 2 and the maximum degree is
20). Notice that in this case, as expected in a graph without
degree-degree correlations, the values of knn(k) are almost
independent of k. 

We can also ask `knn` to bin the results over 5 bins of equal width by
running:
    
        $ knn er_1000_5000.net LIN 5
        6 11.249206
        10 11.037634
        14 10.919366
        18 10.68685
        22 11.474138
        $

Let us consider the case of `movie_actors.net`, i.e. the actors
collaboration network. Here we ask `knn` to compute the average
nearest-neighbours degrees using exponential binning:

        $ knn movie_actors.net EXP 1.4
        2 142.56552
        5 129.09559
        9 158.44493
        15 198.77922
        23 205.96538
        34 210.07379
        50 227.57167
        72 235.89857
        102 254.47583
        144 276.572
        202 307.11004
        283 337.83733
        397 370.34222
        556 410.89117
        779 446.66331
        1091 498.73118
        1527 547.31923
        2137 577.87852
        2991 582.6855
        4187 557.44801
        $

Notice that, due to the presence of the second parameter `EXP`, the
program has printed on output knn(k) over bins of exponentially
increasing width, using an exponent `1.4`. This is useful for plotting
with log or semilog axes. In this case, the clear increasing trend of
knn(k) indicates the presence of assortative correlations.

## SEE ALSO

knn_w(1), deg_seq(1)

## REFERENCES

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Chapter 7, Cambridge University Press
  (2017)


## AUTHORS

(c) Vincenzo 'KatolaZ' Nicosia 2009-2017 `<v.nicosia@qmul.ac.uk>`.
