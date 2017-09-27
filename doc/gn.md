gn(1) -- Find communities using the Girvan-Newman algorithm
======

## SYNOPSIS

`gn` <graph_in> 

## DESCRIPTION

`gn` finds the communities in <graph_in> using the Girvan-Newman
algorithm, based on the successive removal of edges with high
betweenness. The program prints on STDOUT the partition corresponding
to the highest value of the modularity function, and reports on STDERR
the number of communities after each edge removal and the
corresponding value of modularity. 

`N.B.`: the program recomputes the edge betweenness of the graph after
the removal of each edge, so it is not feasible to use it on large
graphs.

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

The program prints on STDERR the number of communities (connected
components) after the removal of each edge, and the corresponding
value of modularity, in the format:

        nc_1 Q_1
        nc_2 Q_2
        nc_3 Q_3
        ....

where `nc_i` is the number of communities after the i-th edge has been
removed and `Q_i` is the corresponding value of modularity. 

## EXAMPLES

We can use `gn` to find communities in the graph `karate_club_unweighted.txt`
(Zachary Karate Club network) with the command:

        $ gn karate_club_unweighted.net 2> karate_gn_trace
        ### nc: 4 Q_max: 0.365631
        0 1
        1 1
        2 2
        3 1
        4 3
        5 3
        6 3
        ...
        30 2
        31 2
        32 2
        33 2
        $ 
        
In this run, the command has found a partition with 4 communities
corrisponding to a modularity Q=0.365631. Notice that node 0, 1, 3,
are in community 1, node 2 is in community 2, node 4,5,6, are in
community 3 and so forth. In general, different runs will provide
different partitions, since any tie in betweenness values is broke by
choosing one of the edges with equal betweenness uniformly at
random. In this example, we have chosen to save the information about
number of communities and modularity after each edge removal in the
file `karate_gn_trace`.

## SEE ALSO

modularity(1), cnm(1), label_prop(1)

## REFERENCES

* M\. Girvan and M. E. J. Newman. "Community structure in social and
  biological networks". P. Natl. Acad. Sci. USA 99 (2002), 7821--7826.

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Appendix 17, Cambridge University Press
  (2017)

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Chapter 9, Cambridge University Press
  (2017)

## AUTHORS

(c) Vincenzo 'KatolaZ' Nicosia 2009-2017 `<v.nicosia@qmul.ac.uk>`.
