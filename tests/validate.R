val <- getip:::validate_ipv4

# should be true
test <- "127.0.0.1"
stopifnot(val(test))

test <- "8.8.8.8"
stopifnot(val(test))

test <- "255.255.255.255"
stopifnot(val(test))


# should be false
test <- "11127.0.0.1"
stopifnot(!val(test))

test <- "8.8.8.8.8"
stopifnot(!val(test))

test <- "-127.0.0.1"
stopifnot(!val(test))

test <- "127.1"
stopifnot(!val(test))

test <- "256.255.255.255"
stopifnot(!val(test))

test <- "255.256.255.255"
stopifnot(!val(test))

test <- "255.255.256.255"
stopifnot(!val(test))

test <- "255.255.255.256"
stopifnot(!val(test))
