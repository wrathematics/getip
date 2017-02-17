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
#include <windows.h>

// Every windows before vista lacks InetPton, and there's no reliable way
// to build this conditionally since Windows doesn't really have a build system.
// tldr: windows continues to be an annoying joke
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

static int inet_pton_v4(const char *ip)
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

int inet_pton(int af, const char *src, void *dst)
{
  return inet_pton_v4(src);
}

#else
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#endif

SEXP R_validate_ipv4(SEXP ip_)
{
  SEXP ret;
  const char *ip = CHARPT(ip_);
  int check;
  struct sockaddr_in sa;
  
  check = (inet_pton(AF_INET, ip, &(sa.sin_addr)) != 0);
  
  PROTECT(ret = allocVector(LGLSXP, 1));
  LOGICAL(ret)[0] = check;
  UNPROTECT(1);
  return ret;
}
