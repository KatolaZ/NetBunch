fitmle(1) -- Fit a set of values with a power-law distribution
======

## SYNOPSIS

`fitmle` <data_in> [<tol> [TEST [<num\_test>]]]

## DESCRIPTION

`fitmle` fits the data points contained in the file <data_in> with a
power-law function P(k) ~ k^(-gamma), using the Maximum-Likelihood
Estimator (MLE). In particular, `fitmle` finds the exponent `gamma`
and the minimum of the values provided on input for which the
power-law behaviour holds. The second (optional) argument <tol> sets
the acceptable statistical error on the estimate of the exponent.

If `TEST` is provided, the program associates a p-value to the
goodness of the fit, based on the Kolmogorov-Smirnov statistics
computed on <num_test> sampled distributions from the theoretical
power-law function. If <num_test> is not provided, the test is based
on 100 sampled distributions.


## PARAMETERS

* <data_in>:
    Set of values to fit. If is equal to `-` (dash), read the set from
    STDIN.

* <tol>: 
    The acceptable statistical error on the estimation of the
    exponent. If omitted, it is set to 0.1.
    
* TEST:
    If the third parameter is `TEST`, the program computes an estimate
    of the p-value associated to the best-fit, based on <num_test>
    synthetic samples of the same size of the input set.

* <num_test>:
    Number of synthetic samples to use for the estimation of the
    p-value of the best fit.

## OUTPUT

If `fitmle` is given less than three parameters (i.e., if `TEST` is
not specified), the output is a line in the format:

        gamma k_min ks

where `gamma` is the estimate for the exponent, `k_min` is the
smallest of the input values for which the power-law behaviour holds,
and `ks` is the value of the Kolmogorov-Smirnov statistics of the
best-fit. 

If `TEST` is specified, the output line contains also the estimate of
the p-value of the best fit:

        gamma k_min ks p-value

where `p-value` is based on <num_test> samples (or just 100, if
<num_test> is not specified) of the same size of the input, obtained
from the theoretical power-law function computed as a best fit.
 
## EXAMPLES

Let us assume that the file `AS-20010316.net_degs` contains the degree
sequence of the data set `AS-20010316.net` (the graph of the Internet
at the AS level in March 2001). The exponent of the best-fit power-law
distribution can be obtained by using:

        $ fitmle AS-20010316.net_degs 
        Using discrete fit
        2.06165 6 0.031626 0.17
        $

where `2.06165` is the estimated value of the exponent `gamma`, `6` is
the minimum degree value for which the power-law behaviour holds, and
`0.031626` is the value of the Kolmogorov-Smirnov statistics of the
best-fit. The program is also telling us that it decided to use the
discrete fitting procedure, since all the values in
`AS-20010316.net_degs` are integers. The latter information is printed
to STDERR.

It is possible to compute the p-value of the estimate by running:

        $ fitmle AS-20010316.net_degs 0.1 TEST
        Using discrete fit
        2.06165 6 0.031626 0.17
        $

which provides a p-value equal to 0.17, meaning that 17% of the
synthetic samples showed a value of the KS statistics larger than that
of the best-fit. The estimation of the p-value here is based on 100
synthetic samples, since <num_test> was not provided. If we allow a
slightly larger value of the statistical error on the estimate of the
exponent `gamma`, we obtain different values of `gamma` and `k_min`,
and a much higher p-value:

        $ fitmle AS-20010316.net_degs 0.15 TEST 1000
        Using discrete fit
        2.0585 19 0.0253754 0.924
        $

Notice that in this case, the p-value of the estimate is equal to
0.924, and is based on 1000 synthetic samples.

## SEE ALSO

deg_seq(1), power_law(1)


## REFERENCES

* A\. Clauset, C. R. Shalizi, and M. E. J. Newman. "Power-law
  distributions in empirical data". SIAM Rev. 51, (2007), 661-703.

* V\. Latora, V. Nicosia, G. Russo, "Complex Networks: Principles,
  Methods and Applications", Chapter 5, Cambridge University Press
  (2017)



## AUTHORS

(c) Vincenzo 'KatolaZ' Nicosia 2009-2017 `<v.nicosia@qmul.ac.uk>`.
