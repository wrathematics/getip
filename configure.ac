## This file is free and unencumbered software released into the public domain.
## See the Unlicense specification for full details http://unlicense.org/

## Process with:
## autoconf configure.ac && autoreconf -if

AC_PREREQ([2.69])
AC_INIT([getip], [0.1-0])

AC_CHECK_HEADERS([ifaddrs.h],
  [IFADDRS="-DHAS_IFADDRS"],
  [IFADDRS="-DNO_IFADDRS"]
)

AC_SUBST(IFADDRS)
AC_OUTPUT(src/Makevars)

