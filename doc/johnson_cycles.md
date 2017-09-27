johnson_cycles(1) -- Enumerate the simple cycles of a graph
======

## SYNOPSIS

`johnson_cycles` <graph_in> [<max_length> [SHOW]]

## DESCRIPTION

`johnson_cycles` enumerates all the simple cycles of the graph given
on input, and prints the total number of cycles of each length. If
<max_length> is provided, `johnson_cycles` ignores any cycle whose
length is larger than <max_length>. If `SHOW` is given as third
argument, all the found cycles are printed on STDERR as soon as they
are found.

## PARAMETERS

* <graph_in>:
    input graph (edge list) if equal to `-` (dash), read the edge list
    from STDIN.

* <max_length>: The maximum length of cycles to count/print. If not
    provided, `johnson_cycles` tries to find all the cycles of the
    graph (see the **WARNING** in the section [OUTPUT][] below).

* SHOW: 
    If the third (optional) parameter is equal to `SHOW`, the program
    will dump on the standard error  all the cycles found.

## OUTPUT

`johnson_cycles` prints on the standard output the number of cycles of
each length, in the format:

        2 N_2
        3 N_3
        4 N_4
        5 N_5
        ...
  
where 2, 3, 4, 5... is the cycle lengths and N_2, N_3, N_4, N_5... is
(twice) the number of cycles of that length.  If `SHOW` is given, each
cycle is also printed on STDERR as soon as it is found, in the format:

    node_l node_(l-1) node_(l-2) ... node_0
    
where `node_l`, `node_(l-1)`, etc. are the labels of the nodes
belonging to the cycle which starts at node  `node_0`.

**WARNING**: If the second parameter <max_length> is not provided,
  `johnson_cycles` will try to enumerate **all** the cycles of the
  graph. In general, this might take a time **exponential** in the
  number of nodes and edges of the graph. As a consequence, specifying
  a maximum length is **highly recommended** if you are not interested
  in finding the number of cycles of any length.


## EXAMPLES

We can count the cycle of any length in the graph of Florentine
families using the command:

          $ johnson_cycles florentine.net
          2 20
          3 6
          4 4
          5 6
          6 10
          7 20
          8 22
          9 8
          10 2
          11 0
          12 0
          13 0
          14 0
          15 0
          16 0
          $

The output means that the graph has 20 cycles of length 2 (edges),
6/2=3 cycles of length 3, 4/2=2 cycles of length 4, and so on. We
could otherwise focus on the cycles of length up to 5 and have each
cycle printed on output:

          $ johnson_cycles florentine.net 5 SHOW
          8 0 
          5 1 
          8 15 6 1 
          8 12 15 6 1 
          6 1 
          6 15 12 8 1 
          6 15 8 1 
          8 1 
          8 12 14 4 2 
          ....
          15 12 
          2 20
          3 6
          4 4
          5 6
          $ 
          
Apart from the degenerate cycles like "8 0", "5 1", etc.,
corresponding to the cycles obtained by traversing the same undirected
edge in the two possible directions, we see in that list some of the
cycles of length 4 (such as "8 15 6 1") and of length 5 (such as "8 12
15 6 1").

The enumeration of all the cycles is normally impractical on larger
graphs, so it is highly recommended to limit the search to short
sizes. For instance, the command:

        $ johnson_cycles er_1000_5000.net 6
        2 5000
        3 340
        4 2406
        5 19416
        6 160554
        $ 

will require less than one second on a modern desktop computer, but
the command:

        $ johnson_cycles er_1000_5000.net 7
        2 5000
        3 340
        4 2406
        5 19416
        6 160554
        7 1360104
        $

will probably take about 15 seconds, while:

        $ johnson_cycles er_1000_5000.net 8
        2 5000
        3 340
        4 2406
        5 19416
        6 160554
        7 1360104
        8 11743500
        $

will run for more than 2 minutes, and larger cycle lengths will
require exponentially more time. 


## SEE ALSO

f3m(1), shortest(1)

## REFERENCES

* D\. B. Johnson. "Finding All the Elementary Circuits of a Directed
  Graph". SIAM J. Comput. 4 (1975), 77-84.

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Chapter 8, Cambridge University Press
  (2017)

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Appendix 15, Cambridge University Press
  (2017)


## AUTHORS

(c) Vincenzo 'KatolaZ' Nicosia 2009-2017 `<v.nicosia@qmul.ac.uk>`.
