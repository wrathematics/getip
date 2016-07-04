/*  Copyright (c) 2015-2016, Drew Schmidt
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "platform.h"

#define ERRMSG "Could not determine local IP"

#define LOCALHOST "127."
#define LOCALHOST_LEN 4

#define SETRET(ip,addr) \
  PROTECT(ip = allocVector(STRSXP, 1)); \
  SET_STRING_ELT(ip, 0, mkChar(addr)); \
  UNPROTECT(1);




#if !OS_WINDOWS
#include <sys/types.h>
#include <ifaddrs.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>

// FIXME this SHOULD be in net/if.h, but doesn't get included for some insane reason
#ifndef IFF_LOOPBACK
#define IFF_LOOPBACK 0 // skip if undefined
#endif

// hope they don't do something weird lol
SEXP ip_internal_nix()
{
  SEXP ip;
  struct ifaddrs *ifaddrs_p, *start;
  struct sockaddr_in *pAddr;
  char *addr;
  
  getifaddrs(&start);
  // keep track of the start so we can free it properly later
  ifaddrs_p = start;
  
  while (ifaddrs_p)
  {
    if (ifaddrs_p->ifa_addr && ifaddrs_p->ifa_addr->sa_family == AF_INET)
    {
      pAddr = (struct sockaddr_in *) ifaddrs_p->ifa_addr;
      
      addr = inet_ntoa(pAddr->sin_addr);
      
      if (strncmp(ifaddrs_p->ifa_name, "lo", 2)   != 0  && 
          strncmp(addr, LOCALHOST, LOCALHOST_LEN) != 0  && 
          !(ifaddrs_p->ifa_flags & IFF_LOOPBACK) )
      {
        SETRET(ip, addr);
        freeifaddrs(start);
        
        return ip;
      }
    }
    
    ifaddrs_p = ifaddrs_p->ifa_next;
  }
  
  freeifaddrs(start);
  error(ERRMSG);
  
  return R_NilValue;
}
#endif


#if OS_WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

// with much help from https://msdn.microsoft.com/en-us/library/aa365949(VS.85).aspx
SEXP ip_internal_win()
{
  SEXP ip;
  int i;
  char *addr;
  
  PMIB_IPADDRTABLE pIPAddrTable;
  DWORD dwSize = 0;
  DWORD dwRetVal = 0;
  IN_ADDR IPAddr;
  
  LPVOID lpMsgBuf;
  
  pIPAddrTable = (MIB_IPADDRTABLE *) malloc(sizeof (MIB_IPADDRTABLE));

  // Allocate workspace
  if (pIPAddrTable)
  {
    if (GetIpAddrTable(pIPAddrTable, &dwSize, 0) == ERROR_INSUFFICIENT_BUFFER)
    {
      free(pIPAddrTable);
      pIPAddrTable = (MIB_IPADDRTABLE *) malloc(dwSize);
    }
    if (pIPAddrTable == NULL)
      error("Memory allocation failed\n");
  }
  
  // Get the data
  if ( (dwRetVal = GetIpAddrTable(pIPAddrTable, &dwSize, 0)) != NO_ERROR )
  { 
    REprintf("GetIpAddrTable failed with error %d\n", dwRetVal);
    if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
                      NULL, dwRetVal, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) & lpMsgBuf, 0, NULL) )
    {
        REprintf("\tError: %s", lpMsgBuf);
        LocalFree(lpMsgBuf);
        error(""); //FIXME
    }
  }
  
  const int nentries = (int) pIPAddrTable->dwNumEntries;
  for (i=0; i<nentries; i++)
  {
    IPAddr.S_un.S_addr = (u_long) pIPAddrTable->table[i].dwAddr;
    addr = inet_ntoa(IPAddr);
    
    const BOOL is_primary = pIPAddrTable->table[i].wType & MIB_IPADDR_PRIMARY;
    if (strncmp(addr, LOCALHOST, LOCALHOST_LEN) != 0 && is_primary)
    {
      SETRET(ip, addr);
      if (pIPAddrTable)
        free(pIPAddrTable);
      
      return ip;
    }
  }
  
  if (pIPAddrTable)
  {
    free(pIPAddrTable);
    pIPAddrTable = NULL;
  }
  
  error(ERRMSG);
  
  return R_NilValue;
}
#endif



SEXP C_ip_internal()
{
  SEXP ret;
  #if OS_WINDOWS
  ret = ip_internal_win();
  #else
  ret = ip_internal_nix();
  #endif
  
  return ret;
}
