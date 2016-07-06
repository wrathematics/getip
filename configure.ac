## This is free and unencumbered software released into the public domain.
## See the Unlicense specification for full details http://unlicense.org/

## Process with:
## autoconf configure.ac && autoreconf -if

AC_PREREQ([2.69])
AC_INIT([getip], [0.1-0])

AC_CHECK_HEADERS(
  [ifaddrs.h], , 
  [echo "ERROR: required C header 'ifaddrs.h' missing"; exit 1]
)
