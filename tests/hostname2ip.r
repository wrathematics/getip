library(getip)

### errors
test = try(hostname2ip(1:10), silent=TRUE)
stopifnot(inherits(test, "try-error"))

test = try(hostname2ip("asdf"), silent=TRUE)
stopifnot(inherits(test, "try-error"))



### these should all work!
hosts = "localhost"
test = hostname2ip(hosts)[[1]][1]
stopifnot(identical(test, "127.0.0.1"))
