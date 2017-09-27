strong_conn(1) -- Find the strongly connected components of a directed graph
======

## SYNOPSIS

`strong_conn` <graph_in> [SHOW]

## DESCRIPTION

`strong_conn` finds the strongly connected components of the directed
graph given as input using the Kosaraju-Sharir algorithm, and prints
the size of each of them. If the optional second parameter `SHOW` is
provided, the program dumps on output also the list of nodes belonging
to each component.

## PARAMETERS

* <graph_in>:
    input graph (edge list) if equal to `-` (dash), read the edge list
    from STDIN.


* SHOW: 
    If the (optional) second parameter is equal to `SHOW`, the program
    will dump on output the list of all the nodes belonging to each
    strongly connected component.

## OUTPUT

`strong_conn` prints on the standard output the size of all the
strongly connected components of the directed graph given as input,
one per line:

        size_1
        size_2
        size_3
        .....

where `size_1` is the size of the first component, `size_2` is the
size of the second component, and so on. Notice that the sizes are not
sorted.  If `SHOW` is given, the program shows the list of nodes
belonging to each strongly connected component, in the format:

    size_1: node_1 node_2 node_3 ...
    size_2: node_1 node_2 node_3 ...
    
## EXAMPLES

The following command:

          $ strong_conn web-NotreDame.net 
          53968
          1
          1
          1
          1
          1
          1
          ....
          $

shows on output the size of the strongly connected component of the
graph `web-NotreDame.net` (the NotreDame WWW data set), in no particular
order. In this case the graph has 203609 strongly connected
components, most of them containing only 1 isolated node.  If we want
to know who are the nodes belonging to each connected component, we
run:

          $ strong_conn web-NotreDame.net SHOW
          53968: 0 1 3 4 5 6 7 8.....
          ..... 325727 325728
          1: 351
          1: 350
          1: 2209
          1: 2208
          1: 2206
          1: 10609
          ....
          $

It is better to save the output of `strong_conn` into a file, e.g. by
using:

          $ strong_conn web-NotreDame.net SHOW > web-NotreDame.net_scc

## SEE ALSO

components(1), node\_components(1), largest_component(1)


## REFERENCES

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Chapter 6, Cambridge University Press
  (2017)

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Appendix 8, Cambridge University Press
  (2017)


## AUTHORS

(c) Vincenzo 'KatolaZ' Nicosia 2009-2017 `<v.nicosia@qmul.ac.uk>`.
