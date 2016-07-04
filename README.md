# getip 

* **Version:** 0.1-0
* **URL**: https://github.com/wrathematics/getip
* **License:** [![License](http://img.shields.io/badge/license-BSD%202--Clause-orange.svg?style=flat)](http://opensource.org/licenses/BSD-2-Clause)
* **Author:** Drew Schmidt

A micro-package for getting your IP address, either the local or external one.

## Installation

<!-- #### Stable Version
```r
install.packages("getip")
``` -->

#### Development Version
```r
### Pick your preference
devtools::install_github("wrathematics/getip")
ghit::install_github("wrathematics/getip")
remotes::install_github("wrathematics/getip")
```



## Usage

To get your local (internal) IP address:

```r
getip::ip_internal()
```

To get your external IP address:

```r
getip::ip_external()
```
