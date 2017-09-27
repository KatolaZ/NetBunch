label_prop(1) -- Find communities using label propagation
======

## SYNOPSIS

`label_prop` [SYNC|ASYNC] <graph_in> [<max_epochs>]

## DESCRIPTION

`label_prop` finds the communities in <graph_in> using the label
propagation algorithm proposed by Raghavan, Albert, and Kumara. The
program prints on STDOUT the partition obtained where no more label
flips are possible, and reports on STDERR the number of communities
and the corresponding value of modularity at each epoch. When used in
ASYNC mode, the algorithm is suitable to find communities in large
graphs.

## PARAMETERS

* `SYNC`|`ASYNC`:
    The first parameter is used to choose between synchronous (`SYNC`)
    or asynchronous (`ASYNC`) label update. 

* <graph_in>:
    undirected input graph (edge list). If is equal to `-` (dash), read
    the edge list from STDIN.
    
* <max_epochs>:
    Sets the maximum number of epochs to run (optional).

## OUTPUT

The program prints on STDOUT the partition obtained where no more
label flips are possible, in the format:

        ## nc: NUM_COMM Q_max: Q_MAX Epochs: EPOCHS
        node_1 comm_1
        node_2 comm_2
        node_3 comm_3
        ... 
        
where `comm_i` is the community to which `node_i` belongs. The first
output line reports the number of communities `NUM_COMM`,  the
corresponding value of modularity `Q_MAX`, and the number of epochs
`EPOCHS` used to find the partition.

The program prints on STDERR the epoch number, the value of modularity
of the partition at that epoch, and the number of flips made in that
epoch, in the format:

        1 Q_1 flips_1
        2 Q_2 flips_2
        3 Q_3 flips_3
        ....

where `Q_i` is the modularity of the partition at the i-th epoch, and
`flips_i` is the number of label flips performed during that
epoch. 

## EXAMPLES

We can use `label_prop` to find communities in the graph
`karate_club_unweighted.net` (Zachary Karate Club network) with the command:

        $ label_prop ASYNC karate_club_unweighted.net 2> karate_label-prop_trace
        ### nc: 2 Q_max: 0.371795 Epochs: 6
        0 0
        1 0
        2 0
        3 0
        ...
        30 1
        31 1
        32 1
        33 1
        $ 
        
The program has found a partition with 2 communities corrisponding to
a modularity Q=0.371795. Notice that node 0, 1, 2, 3, are in community
0, while node 30, 31, 32, 33 are in community 1.  In this example, we
have chosen to save the information about the modularity and number of
flips at each epoch in the file `karate_label-prop_trace`.

## SEE ALSO

modularity(1), gn(1), cnm(1)

## REFERENCES

* U\. N. Raghavan, R. Albert, and S. Kumara. "Near linear time
  algorithm to detect community structures in large-scale
  networks". Phys. Rev. E 76 (2007), 036106.

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Appendix 19, Cambridge University Press
  (2017)

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Chapter 9, Cambridge University Press
  (2017)

## AUTHORS

(c) Vincenzo 'KatolaZ' Nicosia 2009-2017 `<v.nicosia@qmul.ac.uk>`.
