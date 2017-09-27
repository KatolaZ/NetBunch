f3m(1) -- Count all the 3-node subgraphs of a directed graph
======

## SYNOPSIS

`f3m` <graph_in> [<num_random>]

## DESCRIPTION

`f3m` performs a motif analysis on <graph_in>, i.e., it counts all the
3-node subgraphs and computes the z-score of that count with respect
to the corresponding configuration model ensemble.

## PARAMETERS

* <graph_in>:
    input graph (edge list). It must be an existing file.

* <num_random>: 
    The number of random graphs to sample from the configuration model
    for the computation of the z-score of the motifs.


## OUTPUT

`f3m` prints on the standard output a table with 13 rows, one for each
of the 13 possible 3-node motifs. Each line is in the format:

        motif_number  count  mean_rnd  std_rnd  z-score

where `motif_number` is a number between 1 and 13 that identifies the
motif (see [MOTIF NUMBERS][] below), `count` is the number of
subgraphs ot type `motif_number` found in <graph_in>, `mean_rnd` is
the average number of subgraphs of type `motif_number` in the
corresponding configuration model ensemble, and `std_rnd` is the
associated standard deviation. Finally, `z-score` is the quantity:

       (count - mean_rnd) / std_rnd

The program also prints a progress bar on STDERR.


## MOTIF NUMBERS

We report below the correspondence between the 13 possible 3-node
subgraphs and the corresponding `motif_number`. In the diagrams,
'O--->O' indicates a single edge form the left node to the right node,
while 'O<==>O' indicates a double (bi-directional) edge between the
two nodes:
            
       (1)  O<---O--->O
            
       (2)  O--->O--->O

       (3)  O<==>O--->O

       (4)  O--->O<---O
       
       (5)  O--->O--->O
             \        ^
              \_______|
        
       (6)  O<==>O--->O
             \        ^
              \_______|
              
       (7)  O<==>O<---O
          
       (8)  O<==>O<==>O
           
       (9)  O<---O<---O
             \        ^
              \_______|
        
      (10)  O<==>O<---O
             \        ^
              \_______|
              
      (11)  O--->O<==>O
             \        ^
              \_______|
     
      (12)  O<==>O<==>O
             \        ^
              \_______|

      (13)  O<==>O<==>O
            ^\       ^/ 
             \\_____//
              \_____/



## EXAMPLES

To perform a motif analysis on the E.coli transcription regulation
graph, using 1000 randomised networks, we run the command:

        $ f3m e_coli.net 1000
        1          4760         4400.11    137.679     +2.614
        2           162          188.78      8.022     -3.338
        3             0            0.89      3.903     -0.228
        4           226          238.32      7.657     -1.609
        5            40            6.54      2.836    +11.800
        6             0            0.01      0.077     -0.078
        7             0            0.12      0.642     -0.192
        8             0            0.00      0.032     -0.032
        9             0            0.01      0.109     -0.110
        10            0            0.00      0.000     +0.000
        11            0            0.00      0.032     -0.032
        12            0            0.00      0.000     +0.000
        13            0            0.00      0.000     +0.000
        $

Notice that the motif `5` (the so-called "feed-forward loop") has a
z-score equal to 11.8, meaning that it is highly overrepresented in
the E.coli graph with respect to the corresponding configuration model
ensemble. Conversely, the motif `2` (three-node chain) is
underrepresented, as made evident by value of the z-score (-3.338).

## SEE ALSO

johnson_cycles(1)

## REFERENCES

* R\. Milo et al. "Network Motifs: Simple Building Blocks of Complex
  Networks".  Science 298 (2002), 824-827.

* R\. Milo et al. "Superfamilies of evolved and designed networks."
  Science 303 (2004), 1538-1542

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Chapter 8, Cambridge University Press
  (2017)

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Appendix 16, Cambridge University Press
  (2017)


## AUTHORS

(c) Vincenzo 'KatolaZ' Nicosia 2009-2017 `<v.nicosia@qmul.ac.uk>`.
