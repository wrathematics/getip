### Ensures that the internal ipv4 validipation method operates correctly. Does
### not query any device or internet service (as getip() does).

library(getip)


# should be true
test = "127.0.0.1"
stopifnot(validip(test))

test = "8.8.8.8"
stopifnot(validip(test))

test = "255.255.255.255"
stopifnot(validip(test))


# should be false
test = "11127.0.0.1"
stopifnot(!validip(test))

test = "8.8.8.8.8"
stopifnot(!validip(test))

test = "-127.0.0.1"
stopifnot(!validip(test))

test = "127.1"
stopifnot(!validip(test))

test = "256.255.255.255"
stopifnot(!validip(test))

test = "255.256.255.255"
stopifnot(!validip(test))

test = "255.255.256.255"
stopifnot(!validip(test))

test = "255.255.255.256"
stopifnot(!validip(test))

test = "255.255.255.255, 127.0.0.1"
stopifnot(!validip(test))

test = 127.001
stopifnot(!validip(test))

test = NA
stopifnot(!validip(test))


# vector test
test = c("255.255.255.255", "127.0.0.1")
truth = c(TRUE, TRUE)
stopifnot(identical(validip(test), truth))

test = c("127.0.0.1", NA)
truth = c(TRUE, FALSE)
stopifnot(identical(validip(test), truth))
