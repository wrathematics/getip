# getip 

* **Version:** 0.1-0
* **URL**: https://github.com/wrathematics/getip
* **Status:** [![Build Status](https://travis-ci.org/wrathematics/getip.png)](https://travis-ci.org/wrathematics/getip)
* **License:** [![License](http://img.shields.io/badge/license-BSD%202--Clause-orange.svg?style=flat)](http://opensource.org/licenses/BSD-2-Clause)
* **Author:** Drew Schmidt


A micro-package for getting your IP address, either the local/internal or the public/external one.  Currently only IPv4 addresses are supported.



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
getip::getip("internal")
```

To get your public (external) IP address:

```r
getip::getip("external")
```


The argument is case-insensitive and operates by partial matching.  So if you're very lazy, you can just type `getip("e")` for the external IP.
