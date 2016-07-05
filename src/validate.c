/*  Copyright (c) 2016, Drew Schmidt
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


#include <R.h>
#include <Rinternals.h>
#include <stdbool.h>
#include <ctype.h>

#include "platform.h"

#define CHARPT(x) ((char*)CHAR(STRING_ELT(x,0)))

#if OS_WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>

#ifndef InetPton // for all my windows xp bros out there
static inline bool check_octet(char *buf, const char *ip, const int start, const int end)
{
  int i;
  int x;
  
  if (end-start > 3)
    return false;
  
  for (i=0; i<(end-start); i++)
    buf[i] = ip[i+start];
  
  buf[i] = '\0';
  
  x = atoi(buf);
  
  return (x >= 0 && x <= 255);
}

INT InetPton(int af, const char *ip, void *dst)
{
  bool check;
  int end_locs[4];
  char buf[4];
  int i = 0, j = 0;
  
  memset(end_locs, 0, 4*sizeof(int));
  
  while (ip[i] != '\0')
  {
    if (ip[i] == '.')
    {
      end_locs[j] = i;
      j++;
      
      if (j >= 4)
        break;
    }
    
    i++;
  }
  
  if (ip[i] != '\0')
    return false;
  
  end_locs[j] = i;
  
  
  j = 0;
  for (i=0; i<4; i++)
  {
    if (end_locs[i] == 0)
    {
      return false;
    }
    else
    {
      check = check_octet(buf, ip, j, end_locs[i]);
      if (!check)
        return(check);
      
      j = end_locs[i]+1;
    }
  }
  
  return check;
}
#endif

#else
#include <arpa/inet.h>
#endif

SEXP C_validate_ipv4(SEXP ip_)
{
  SEXP ret;
  const char *ip = CHARPT(ip_);
  int check;
  struct sockaddr_in sa;
  
#if OS_WINDOWS
  check = InetPton(AF_INET, ip, &(sa.sin_addr));
#else
  check = (inet_pton(AF_INET, ip, &(sa.sin_addr)) != 0);
#endif
  
  PROTECT(ret = allocVector(LGLSXP, 1));
  LOGICAL(ret)[0] = check;
  UNPROTECT(1);
  return ret;
}
