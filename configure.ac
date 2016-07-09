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
AC_CHECK_LIB([c], [socket], [SOCKET_IN_C="yes"], [SOCKET_IN_C="no"])
AC_CHECK_LIB([socket], [socket], [SOCKET="yes"], [SOCKET="no"])

SOCKET_LIBS=""
if test "X$IFADDRS" = "X-DNO_IFADDRS" ; then
  if test "X$SOCKET_IN_C" = "Xyes"; then
    SOCKET_LIBS="-lc"
  fi
  if test "X$SOCKET" = "Xyes"; then
    SOCKET_LIBS="-lsocket"
  fi
  if test "X$SOCKET_LIBS" = "X"; then
    AC_MSG_ERROR([socket function is not found in libc.so or libsocket.so])
  fi
fi

AC_SUBST(IFADDRS)
AC_SUBST(SOCKET_LIBS)
AC_OUTPUT(src/Makevars)

