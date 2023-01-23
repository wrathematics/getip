## This file is free and unencumbered software released into the public domain.
## See the Unlicense specification for full details http://unlicense.org/

## Process with:
## autoconf configure.ac && autoreconf -if

AC_PREREQ([2.69])
AC_INIT([getip],[0.1-0])

: ${R_HOME=`R RHOME`}
if test -z "${R_HOME}"; then
  AC_MSG_ERROR([Unable to determine RHOME])
fi
CC=`"${R_HOME}/bin/R" CMD config CC`
CFLAGS=`"${R_HOME}/bin/R" CMD config CFLAGS`
CPPFLAGS=`"${R_HOME}/bin/R" CMD config CPPFLAGS`



AC_CHECK_HEADERS([ifaddrs.h],
  [IFADDRS="-DHAS_IFADDRS"],
  [IFADDRS="-DNO_IFADDRS"]
)
AC_CHECK_LIB([c], [socket], [SOCKET_IN_C="yes"], [SOCKET_IN_C="no"])
AC_CHECK_LIB([socket], [socket], [SOCKET="yes"], [SOCKET="no"])

SOCKET_LIBS=""
if test "X${IFADDRS}" = "X-DNO_IFADDRS" ; then
  if test "X${SOCKET_IN_C}" = "Xyes"; then
    SOCKET_LIBS="-lc"
  fi
  if test "X${SOCKET}" = "Xyes"; then
    SOCKET_LIBS="-lsocket"
  fi
  if test "X${SOCKET_LIBS}" = "X"; then
    AC_MSG_ERROR([socket function is not found in libc.so or libsocket.so])
  fi
fi

dnl ifr may not be in an array format; use ifr_addr.sa_len to find the next ifr
if test "X${IFADDRS}" = "X-DNO_IFADDRS" ; then
  AC_CHECK_MEMBER([struct sockaddr.sa_len], [], [], [#include <netinet/in.h>])
  SA_LEN=""
  if test "X${ac_cv_member_struct_sockaddr_sa_len}" = "Xyes"; then
    SA_LEN="-DHAVE_STRUCT_SOCKADDR_SA_LEN"
  fi
fi



echo " "
echo "******************* Results of getip package configure *******************"
echo "* Socket Report"
echo "*   >> IFADDRS = ${IFADDRS}"
echo "*   >> SA_LEN = ${SA_LEN}"
echo "*   >> SOCKET_LIBS = ${SOCKET_LIBS}"
echo "**************************************************************************"
echo " "

AC_SUBST(IFADDRS)
AC_SUBST(SOCKET_LIBS)
AC_SUBST(SA_LEN)
AC_CONFIG_FILES([src/Makevars])
AC_OUTPUT
