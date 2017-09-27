power_law(1) -- Sample N integers from a discrete power-law distribution
======

## SYNOPSIS

`power_law` <gamma> <k_min> <k_max> <N> 

## DESCRIPTION

`power_law` samples <N> elements from the discrete power-law
distribution 

    P(k) ~ k^{gamma}

where 
    
    k_min <= k <= k_max, gamma < 1

The program can be used to generate a power-law degree distribution
with an assigned value of the exponent <gamma>.


## PARAMETERS

* <gamma>:
    Exponent of the power-law distribution.

* <k_min>, <k_max>:
    Boundaries of the sampling interval.

* <N> 
    The number of samples to generate

## OUTPUT

`power_law` prints on the standard output the sampled values, one per
line, in the format:

    s1
    s2
    s3
     ...
    sN

The program returns the value `0` if the sum of the samples is even,
or returns `1` otherwise. The return value can be used to determine
whether the set of samples can correspond to a degree sequence (if the
sum of the sequence is odd, then the sequence cannot be a valid degree
sequence). See [RETURN VALUES][] below.

## EXAMPLES

To generate <N=1000> independent samples from the power-law
distribution `P(k) ~ k^(-3)`, where samples are in the interval
`[3, 50]`, we can use:

    $ power_law -3.0 3 50 1000
    11
    3
    3
    5
    6
    7
     ....
    8
    3
    $

To save the samples in the file `pl_-3.0_3_50_1000`, we redirect STDOUT:

    $ power_law -3.0 3 50 1000 > pl_-3.0_3_50_1000

## RETURN VALUES

The value returned by `power_law ` can be used to test whether the sum
of the resulting set of samples is even or odd. Under Windows
PowerShell, you can check the last exit code by inspecting the
variable `$lastExitCode` right after executing `power_law`, as in:
    
    > power_law -2.7 4 300 5000 > pl_-2.7_4_300_5000
    > $lastExitCode
    
    0
    >

In this case, the exit code is `0`, meaning that the resulting set of
samples has an even sum (and can be thus used as a degree
sequence). Under Linux/MacOS/Unix (and in general when using any
POSIX-compliant shell) you should check the value of the variable
`$?`, right after executing `power_law`, i.e.:

    $ power_law -2.5 3 500 5000 > pl_-2.5_3_500_5000
    $ echo $?
    1
    $

Notice that this particular run of `power_law` has produced a sequence
with an odd sum, which thus cannot correspond to a valid degree sequence.

## SEE ALSO

deg_seq(1), conf_model_deg(1), conf_model_deg_nocheck(1)


## REFERENCES

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Chapter 5, Cambridge University Press
  (2017)

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Appendix 9, Cambridge University Press
  (2017)


## AUTHORS

(c) Vincenzo 'KatolaZ' Nicosia 2009-2017 `<v.nicosia@qmul.ac.uk>`.
