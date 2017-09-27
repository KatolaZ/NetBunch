cnm(1) -- Find communities using greedy modularity optimisation
======

## SYNOPSIS

`cnm` <graph_in> 

## DESCRIPTION

`cnm` finds the communities in <graph_in> using the greedy modularity
optimisation algorithm proposed by Clauset, Newman and Moore. The
program prints on STDOUT the partition corresponding to the highest
value of the modularity function, and reports on STDERR the number of
communities and the corresponding value of modularity at each
step. The algorithm is quite eficient and thus suitable to find
communities in large graphs.

## PARAMETERS

* <graph_in>:
    undirected input graph (edge list). If is equal to `-` (dash), read
    the edge list from STDIN.

## OUTPUT

The program prints on STDOUT the partition corresponding to the
highest value of modularity, in the format:

        ## nc: NUM_COMM Q_max: Q_MAX 
        node_1 comm_1
        node_2 comm_2
        node_3 comm_3
        ... 
        
where `comm_i` is the community to which `node_i` belongs. The first
output line reports the number of communities `NUM_COMM` and the
corresponding value of modularity `Q_MAX` of the partition.

The program prints on STDERR the number of communities and the
corresponding value of modularity at each step, in the format:

        nc_1 Q_1
        nc_2 Q_2
        nc_3 Q_3
        ....

where `nc_i` is the number of communities after the i-th marge and
`Q_i` is the corresponding value of modularity. Since the algorithm
merges two communities at each step, the values `nc_1`, `nc_2`,
`nc_3`, etc.  will be equal to `N-1`, `N-2`, `N-3`, etc.

## EXAMPLES

We can use `cnm` to find communities in the graph
`karate_club_unweighted.net` (Zachary Karate Club network) with the
command:

        $ cnm karate_club_unweighted.net 2> karate_cnm_trace
        ### nc: 3 Q_max: 0.380671
        0 16
        1 2
        2 2
        3 2
        4 16
        5 16
        6 16
        ...
        30 26
        31 26
        32 26
        33 26
        $ 
        
The program has found a partition with 3 communities corrisponding to
a modularity Q=0.380671. Notice that node 0, 4, 5, 6 are in community
16, node 1, 2, 3 are in community 2, and so forth.  In this example,
we have chosen to save the information about number of communities and
modularity at each step in the file `karate_cnm_trace`.

## SEE ALSO

modularity(1), gn(1), label_prop(1)

## REFERENCES

* A\. Clauset, M. E. J. Newman, and C. Moore. "Finding community
  structure in very large networks". Phys. Rev. E 70 (2004), 066111.

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Appendix 18, Cambridge University Press
  (2017)

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Chapter 9, Cambridge University Press
  (2017)

## AUTHORS

(c) Vincenzo 'KatolaZ' Nicosia 2009-2017 `<v.nicosia@qmul.ac.uk>`.
