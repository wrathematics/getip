library(getip)

### We can't know the IPs of CRAN machines ahead of time (or anywhere else this
### may be checked), so we only ensure that evaluation occurred without error.
getip("internal")
# getip("external")
