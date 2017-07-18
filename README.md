# getip 

* **Version:** 0.1-0
* **Status:** [![Build Status](https://travis-ci.org/wrathematics/getip.png)](https://travis-ci.org/wrathematics/getip)
* **License:** [BSD 2-Clause](http://opensource.org/licenses/BSD-2-Clause)
* **Author:** Drew Schmidt and Wei-Chen Chen
* **Project home**: https://github.com/wrathematics/getip
* **Bug reports**: https://github.com/wrathematics/getip/issues



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
