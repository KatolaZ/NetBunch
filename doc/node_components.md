node_components(1) -- Find the components associated to a specific node
======

## SYNOPSIS

`node_components` <graph_in> <node> [<component> [SHOW]]

## DESCRIPTION

`node_components` finds the components associated to the node <node>
of the graph <graph_in> provided as input, and prints on output the
size of those components. The value of the third parameter <component>
can be set to `IN`, `OUT`, `INOUT`, `SSC`, `WCC`, or `ALL`. According
to the value of <component>, the program will compute the IN-component
(`IN`), the OUT-component (`OUT`), both the IN- and OUT-component
(`INOUT`), the strongly connected components (`SCC`), or the
weakly-connected component (`WCC`) to which <node> belongs. If the
third parameter is omitted or equal to `ALL`, then `node_components`
computes all the components associated to <node>.  If `SHOW` is
specified as the fourth parameter, the program also shows the list of
nodes which belong to each of the required components.

## PARAMETERS

* <graph_in>:
    input graph (edge list) if equal to `-` (dash), read the edge list
    from STDIN.
* <node>:
    the label of the node.

* <component>: 
    the component to compute. Can be set to one of `IN`
    (IN-component), `OUT` (OUT-component), `INOUT` (both the IN- and
    the OUT-component), `SSC` (strongly connected component), `WCC`
    (weakly connected component), or `ALL` (all the above). If
    <component> is not set, compute all the components.

* SHOW: 
    If the (optional) fourth parameter is equal to `SHOW`, the program
    will dump on output the list of all the nodes belonging to each
    component.

## OUTPUT

`node_components` prints on the standard output the size of the
required components, one per line, in the format:

    comp_type_1: size_1
    comp_type_2: size_2
    .....

where `comp_type_1` is the type of component (one of `IN`, `OUT`,
`SCC`, or `WCC`) and `size_1` is its size, and so on. Notice that the
sizes are not sorted.  If `SHOW` is given, the program shows the list
of nodes belonging to each component, in the format:

    comp_type_1: size_1 node_1 node_2 node_3 ...
    comp_type_2: size_2 node_1 node_2 node_3 ...
    
## EXAMPLES

We can use `node_components` to compute the IN-component of a specific
node. For instance, the command:

          $ node_components web-NotreDame.net 2711 IN 
          IN: 53968
          $

computes the size of the IN-component of node 2711 in the graph
`www_nd.net` (WWW Notre Dame). This component contains 53968
nodes. Similarly:

          $ node_components web-NotreDame.net 2711 OUT SHOW
          OUT: 21 559 2711 2712 2713 2714 2715 2716 ... 33271 33272
          $

computes and shows the OUT-component of node 2711, which contains 21
nodes, including node 2711. Finally, we can obtain the size of all the
components associated to node 2711 by running:

          $ node_components web-NotreDame.net 2711
          IN: 53986
          OUT: 21
          WCC: 325729
          SCC: 18
          $

In this case, the IN-component of node 2711 contains 53986 nodes, the
OUT-component contains 21 nodes, the weakly connected components
contains 325729 nodes (the whole graph), and the strongly connected
component contains 18 nodes. It is also possible to show the list of
nodes belonging to each component by providing `SHOW` as fourth
parameter. IN this case, it is better to save the output of
`node_components` into a file, e.g. by using:

          $ node_components web-NotreDame.net 2711 ALL SHOW > web-NotreDame.net_components_2711

## SEE ALSO

components(1), strong_conn(1), largest_component(1)


## REFERENCES

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Chapter 6, Cambridge University Press
  (2017)

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Appendix 8, Cambridge University Press
  (2017)


## AUTHORS

(c) (c) Vincenzo 'KatolaZ' Nicosia 2009-2017 `<v.nicosia@qmul.ac.uk>`.
