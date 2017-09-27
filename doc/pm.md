pm(1) -- Compute the leading eigenvalue and eigenvector of a graph
======

## SYNOPSIS

`pm` <graph_in> <is_dir> <eps>

## DESCRIPTION

`pm` computes the leading eigenvalue and the corresponding eigenvector
of the matrix given as input, using the Power Method. In particular,
this implementation uses the Rayleigh iteration, which allows faster
convergence on undirected graphs. 

## PARAMETERS

* <graph_in>:
    input graph (edge list) if equal to `-` (dash), read the edge list
    from STDIN (standard input).

* <is_dir>:
    set either to `0` (zero) for undirected graphs, or to `1` (one)
    for directed graphs.

* <eps>: 
    Required relative error on the approximation of the leading
    eigenvalue. The program terminates when the relative change in the
    approximation of the eigenvalue is smaller than `eps`

## EXAMPLES

The following command:

          $ pm er_1000_5000.net 0 0.0000001

computes the leading eigenvalue and the corresponding eigenvector of
the undirected graph stored in the file `er_1000_5000.txt`. We can
store the leading eigenvector in a file, e.g. by using the command:

          $ pm er_1000_5000.net 0 0.0000001 > er_1000_5000.net_eig
          11.0335794552533
          $

which will save the leading eigenvector in the file
`er_1000_5000.net_eig`, one component for each row, and shown on
output the leading eigenvalue of the graph.

## REFERENCES 

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Appendix 5, Cambridge University Press
  (2017)


## AUTHORS

(c) Vincenzo 'KatolaZ' Nicosia 2009-2017 `<v.nicosia@qmul.ac.uk>`.
