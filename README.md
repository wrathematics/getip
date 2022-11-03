# getip 

* **Version:** 0.1-2
* **License:** [BSD 2-Clause](https://opensource.org/licenses/BSD-2-Clause)
* **Project home**: https://github.com/wrathematics/getip
* **Bug reports**: https://github.com/wrathematics/getip/issues


A micro-package for getting your IP address, either the local/internal or the public/external one. Currently only IPv4 addresses are supported.



## Installation

You can install the stable version from CRAN using the usual `install.packages()

```r
install.packages("getip")
```

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

There are two other exported helper functions. One is `validip()` which will check if a vector of strings are potentially valid IP addresses (though not necessarily in active use --- i.e., it does not use the internet)

```r
ips = c("127.0.0.1", "25", "255.255.255.255", "255.555.255.1")
getip::validip(ips)
## [1]  TRUE FALSE  TRUE FALSE
```

The final exported function is `hostname2ip()`. This returns a list of vectors of IP addresses for a vector of hostnames. This queries the internet.

```r
hosts = c("www.google.com", "localhost", "www.yahoo.com")
getip::hostname2ip(hosts)
## [[1]]
## [1] "64.233.185.103" "64.233.185.99"  "64.233.185.147" "64.233.185.106"
## [5] "64.233.185.105" "64.233.185.104"
## 
## [[2]]
## [1] "127.0.0.1" "127.0.0.1"
## 
## [[3]]
## [1] "74.6.231.21" "74.6.231.20" "74.6.143.26" "74.6.143.25"
```
