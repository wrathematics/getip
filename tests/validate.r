### Ensures that the internal ipv4 validation method operates correctly. Does
### not query any device or internet service (as getip() does).

valid = getip:::validate_ipv4

# should be true
test = "127.0.0.1"
stopifnot(valid(test))

test = "8.8.8.8"
stopifnot(valid(test))

test = "255.255.255.255"
stopifnot(valid(test))


# should be false
test = "11127.0.0.1"
stopifnot(!valid(test))

test = "8.8.8.8.8"
stopifnot(!valid(test))

test = "-127.0.0.1"
stopifnot(!valid(test))

test = "127.1"
stopifnot(!valid(test))

test = "256.255.255.255"
stopifnot(!valid(test))

test = "255.256.255.255"
stopifnot(!valid(test))

test = "255.255.256.255"
stopifnot(!valid(test))

test = "255.255.255.256"
stopifnot(!valid(test))

test = c("255.255.255.255, 127.0.0.1")
stopifnot(!valid(test))

test = 127.001
stopifnot(!valid(test))

test = NA
stopifnot(!valid(test))
