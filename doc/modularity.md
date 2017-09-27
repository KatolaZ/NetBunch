modularity(1) -- Compute the modularity of a partition of a graph
======

## SYNOPSIS

`modularity` <graph_in> <partition>

## DESCRIPTION

`modularity` computes the value of the modularity function associated
to a partition of the nodes of the graph given as input. 

## PARAMETERS

* <graph_in>:
    undirected input graph (edge list). If is equal to `-` (dash), read
    the edge list from STDIN.

* <partition>: 
    The name of the file containing the association of nodes to
    communities (the partition of which one wants to compute the
    modularity). See [INPUT FORMAT][] for details.

## INPUT FORMAT

<graph_in> is an edge-list. The file <partition> is in the format:

        node_0 community_0
        node_1 community_1
        ....

where 'node_0' is the label of the first node and 'community_0' is the
community to which 'node_0' belongs, and so on.

## OUTPUT

`modularity` prints on STDOUT the value of the modularity function
associated to the partition of the graph provided as input. The
program also prints on STDERR one line in the format:

        ## nc: NUM_COMMUNITIES
        
where 'NUM_COMMUNITIES' is the number of communities in the partition
provided as input.


## REFERENCES

* M\. E. J. Newman and M. Girvan. "Finding and evaluating community
  structure in networks". Phys. Rev. E 69, (2004), 026113.

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Chapter 9, Cambridge University Press
  (2017)


## AUTHORS

(c) Vincenzo 'KatolaZ' Nicosia 2009-2017 `<v.nicosia@qmul.ac.uk>`.
