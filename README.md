# grc

`grc` is an R function that calculates the 'global reaching centrality'  of a hierarchy or collection of hierarchies. The 'global reaching centrality' is a measure of the degree of hierarchy in a network. See the paper below for a detailed explanation of the metric. 

* Mones, E., Vicsek, L., & Vicsek, T. (2012). Hierarchy measure for complex networks. PloS one, 7(3).  

This implementation of grc is meant to be applied to human hierarchies, in which one knows the number of subordinates below each individual


### Inputs

* `n_subordinates` = a vector containing the number of subordinates below each individual in a hierarchy (or in multiple hierarchies)


### Output
Returns a value for the grc index, ranging between 0 (no hierarchy) and 1 (perfect hierarchy). Here, a 'perfect hierarchy' is one in which 1 person controls all other individuals.


### Example:

We'll calculate the global reaching centrality of the hierarchy below. 

![Example hierarchy](https://economicsfromthetopdown.files.wordpress.com/2019/05/hierarchy_model-1.png)


```R
library(Rcpp)
library(RcppArmadillo)

sourceCpp("grc.cpp")

# vector of number of subordinates of each person
n_sub = c(rep(14, 1), # top rank
          rep(6, 2),  # 3rd rank
          rep(2, 4),  # 2nd rank
          rep(0, 8)   # 1st rank
          )


[1] 0.8979592
```


### Installation
To use `grc`, install the following R packages:
 * [Rcpp](https://cran.r-project.org/web/packages/Rcpp/index.html) 
 * [RcppArmadillo](https://cran.r-project.org/web/packages/RcppArmadillo/index.html) 

Put the source code (`grc.cpp`) in the directory of your R script. Then source it with the command `sourceCpp('grc.cpp')`.




