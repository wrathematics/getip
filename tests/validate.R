valid <- getip:::valididate_ipv4

# should be true
test <- "127.0.0.1"
stopifnot(valid(test))

test <- "8.8.8.8"
stopifnot(valid(test))

test <- "255.255.255.255"
stopifnot(valid(test))


# should be false
test <- "11127.0.0.1"
stopifnot(!valid(test))

test <- "8.8.8.8.8"
stopifnot(!valid(test))

test <- "-127.0.0.1"
stopifnot(!valid(test))

test <- "127.1"
stopifnot(!valid(test))

test <- "256.255.255.255"
stopifnot(!valid(test))

test <- "255.256.255.255"
stopifnot(!valid(test))

test <- "255.255.256.255"
stopifnot(!valid(test))

test <- "255.255.255.256"
stopifnot(!valid(test))
