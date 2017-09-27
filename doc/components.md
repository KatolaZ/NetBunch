components(1) -- Find the connected components of a graph
======

## SYNOPSIS

`components` <graph_in> [SHOW]

## DESCRIPTION

`components` finds the connected components of the undirected graph
given as input using the Depth-First Search algorithm, and prints the
size of each of them. If the optional second parameter `SHOW` is
provided, the program dumps on output also the list of nodes belonging
to each component.

## PARAMETERS

* <graph_in>:
    input graph (edge list) if equal to `-` (dash), read the edge list
    from STDIN.


* SHOW: 
    If the (optional) second parameter is equal to `SHOW`, the program
    will dump on output the list of all the nodes belonging to each
    connected component.

## OUTPUT

`components` prints on the standard output the size of all the
connected components of the undirected graph given as input, one per
line:

    size_1
    size_2
    size_3
    .....

where `size_1` is the size of the first component, `size_2` is the
size of the second component, and so on. Notice that the sizes are not
sorted.  If `SHOW` is given, the program shows the list of nodes
belonging to each component, in the format:

    size_1: node_1 node_2 node_3 ...
    size_2: node_1 node_2 node_3 ...
    
## EXAMPLES

The following command:

          $ components er_1000_5000.txt 
          1000
          $

shows on output the size of the only connected component of the graph
`er_1000_5000.txt`. In this case the graph has only one connected
component (it is a super-critical Erdos-Renyi random graph with 1000
nodes and 5000 edges). A more interesting example can be obtained
using the graph `er_1000_2000.txt`:

          $ components er_1000_2000.txt 
          985
          1
          1
          1
		  1
          1
          1
          1
	      1
	      1
	      1
	      1
	      1
	      1
	      1
	      1
          $

In this case, the graph has 16 connected components: one of those
components contains 985 nodes, while the other 15 components consist
of isolated nodes. If we want to know who are the nodes belonging to
each connected component, we run:

          $ components er_1000_2000.txt SHOW
          985: 0 1 2 3 4 5 6 7 8 9 10 11 12.....
          ...
          1: 63
          1: 75
          1: 218
          1: 222
          1: 368
          1: 398
          1: 441
          1: 566
          1: 572
          1: 663
          1: 715
          1: 756
          1: 863
          1: 883
          1: 917
          $

If we run:

          $ components er_1000_2000.txt SHOW > er_1000_2000.txt_components

the result of `components` will be saved in the file
`er_1000_2000.txt_components`.

## SEE ALSO 

strong_conn(1), node_components(1), largest_component(1)


## REFERENCES

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Chapter 3, Cambridge University Press
  (2017)

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Appendix 8, Cambridge University Press
  (2017)


## AUTHORS

(c) Vincenzo 'KatolaZ' Nicosia 2009-2017 `<v.nicosia@qmul.ac.uk>`.
