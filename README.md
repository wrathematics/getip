# getip 

* **Version:** 0.1-1
* **License:** [BSD 2-Clause](https://opensource.org/licenses/BSD-2-Clause)
* **Project home**: https://github.com/wrathematics/getip
* **Bug reports**: https://github.com/wrathematics/getip/issues


A micro-package for getting your IP address, either the local/internal or the public/external one. Currently only IPv4 addresses are supported.



## Installation

<!--
You can install the stable version from CRAN using the usual `install.packages()

```r
install.packages("getip")
``` -->

The development version is maintained on GitHub:

```r
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

The argument is case-insensitive and operates by partial matching. So if you're very lazy, you can just type `getip("e")` for the external IP.
