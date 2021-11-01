/*  Copyright (c) 2017, Drew Schmidt.
    All rights reserved.
    
    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    
    1. Redistributions of source code must retain the above copyright notice,
    this list of conditions and the following disclaimer.
    
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    
    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
    TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
    PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
    CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
    EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
    PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
    PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
    LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


#include <Rinternals.h>
#include <string.h>
#include <stdint.h>

#include "include/platform.h"
#include "include/reactor.h"

#define STR(x,i) ((char*)CHAR(STRING_ELT(x,i)))

#if OS_NIX
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#elif OS_WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#endif


// -----------------------------------------------------------------------------
// *nix
// -----------------------------------------------------------------------------

#if OS_NIX

#define IS_IPV4(p) (p->ai_family == AF_INET)

// Adapted from "Beej's Guide to Network Programming" http://beej.us/guide/bgnet/output/html/multipage/syscalls.html#getaddrinfo
// which is public domain.  If BSD doesn't work for you (lol?), I'm happy to
// release this under different terms, in the spirit of the source material.
static SEXP hostname2ip(SEXP s_)
{
  SEXP ret;
  const R_xlen_t len = LENGTH(s_);
  struct addrinfo hints, *res, *p;
  char ipstr[INET_ADDRSTRLEN+1];
  
  PROTECT(ret = allocVector(VECSXP, len));
  
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC; // AF_INET or AF_INET6 to force version
  hints.ai_socktype = SOCK_STREAM;
  
  for (R_xlen_t i=0; i<len; i++)
  {
    SEXP ipvec;
    const char *const s = STR(s_, i);
    uint8_t num_addrs = 0;
    
    int status = getaddrinfo(s, NULL, &hints, &res);
    if (status != 0)
    {
      UNPROTECT(1);
      error("getaddrinfo() failed with error \"%s\"\n      host:  %s\n      index: %i\n", gai_strerror(status), s, i);
    }
    
    
    // count number of addrs, then set return vector
    for (p=res; p != NULL; p=p->ai_next)
    {
      if (IS_IPV4(p))
        num_addrs++;
    }
    
    if (num_addrs == 0)
    {
      SET_VECTOR_ELT(ret, i, Rf_ScalarString(NA_STRING));
      freeaddrinfo(res);
      continue;
    }
    
    PROTECT(ipvec = allocVector(STRSXP, num_addrs));
    num_addrs = 0;
    
    for (p=res; p != NULL; p=p->ai_next)
    {
      if (IS_IPV4(p))
      {
        struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
        void *addr = &(ipv4->sin_addr);
        
        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        SET_STRING_ELT(ipvec, num_addrs, mkChar(ipstr));
        num_addrs++;
      }
    }
    
    SET_VECTOR_ELT(ret, i, ipvec);
    UNPROTECT(1);
    
    freeaddrinfo(res);
  }
  
  
  UNPROTECT(1);
  return ret;
}



// -----------------------------------------------------------------------------
// Windows
// -----------------------------------------------------------------------------

#elif OS_WINDOWS

static SEXP hostname2ip(SEXP s_)
{
  WSADATA wsad;
  struct hostent *host;
  struct in_addr addr;
  SEXP ret;
  const R_xlen_t len = LENGTH(s_);
  
  PROTECT(ret = allocVector(VECSXP, len));
  
  for (R_xlen_t i=0; i<len; i++)
  {
    SEXP ipvec;
    const char *const s = STR(s_, i);
    int num_addrs = 0;
    
    memset(&addr, 0, sizeof(struct in_addr));
    
    // start winsock
    int wserr = WSAStartup(MAKEWORD(2, 2), &wsad);
    if (wserr != 0)
    {
      UNPROTECT(1);
      error("WSAStartup() failed with error: %d\n", wserr);
    }
    
    host = gethostbyname(s);
    if (host == NULL)
    {
      DWORD errnum = WSAGetLastError();
      
      if (errnum != 0)
      {
        UNPROTECT(1);
        
        if (errnum == WSAHOST_NOT_FOUND || errnum == WSANO_DATA)
          error("gethostbyname() failed with error \"Name or service not known\"\n      host:  %s\n      index: %i\n", s, i);
        else
          error("gethostbyname() failed with error: %ld\n", errnum);
      }
    }
    
    
    // count number of addrs, then set return vector
    if (host->h_addrtype == AF_INET)
    {
      while (host->h_addr_list[num_addrs])
        num_addrs++;
    }
    
    if (num_addrs == 0)
    {
      SET_VECTOR_ELT(ret, i, Rf_ScalarString(NA_STRING));
      continue;
    }
    
    PROTECT(ipvec = allocVector(STRSXP, num_addrs));
    num_addrs = 0;
    
    while (host->h_addr_list[num_addrs])
    {
      // have to manually set the field length for some reason ???
      addr.s_addr = *(u_long*) host->h_addr_list[num_addrs];
      SET_STRING_ELT(ipvec, num_addrs, mkChar(inet_ntoa(addr)));
      num_addrs++;
    }
    
    SET_VECTOR_ELT(ret, i, ipvec);
    UNPROTECT(1);
  }
  
  
  UNPROTECT(1);
  return ret;
}



// -----------------------------------------------------------------------------
// Some other, mysterious, unsupported platform
// -----------------------------------------------------------------------------

#else

static SEXP hostname2ip(SEXP s_)
{
  error("OS is not detectable as one of Windows or *NIX; platform unsupported");
  return R_NilValue;
}

#endif



// -----------------------------------------------------------------------------
// Interface
// -----------------------------------------------------------------------------

SEXP R_hostname2ip(SEXP hosts)
{
  CHECK_IS_STRINGS(hosts);
  
  return hostname2ip(hosts);
}
