/*  Copyright (c) 2015-2016, Drew Schmidt, 
    Copyright (c) 2016 Wei-Chen
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
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>

// NOTE this SHOULD be in net/if.h, but doesn't get included for some insane reason
#ifndef IFF_LOOPBACK
#define IFF_LOOPBACK 0 // skip if undefined
#endif

#if HAS_IFADDRS
#include <ifaddrs.h>

// hope they don't do something weird lol
static inline SEXP ip_internal_nix()
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

#elif NO_IFADDRS
#if OS_SOLARIS
#include <unistd.h>
#include <stropts.h>
#include <sys/sockio.h>
#endif

static inline SEXP ip_internal_nix()
{
  SEXP ip;
  struct ifconf ifc;
  struct ifreq *ifr, *socket_p;
  struct sockaddr_in *pAddr;
  int sd, ifc_num, i;
  char *addr;
  
  sd = socket(PF_INET, SOCK_DGRAM, 0);
  if (sd > 0)
  {
    // find number of interfaces
    if (ioctl(sd, SIOCGIFCONF, &ifc) == 0)
    {
      ifr = malloc(ifc.ifc_len);
      ifc.ifc_ifcu.ifcu_req = ifr;
      
      // retrive the interface list
      if (ioctl(sd, SIOCGIFCONF, &ifc) == 0)
      {
        ifc_num = ifc.ifc_len / sizeof(struct ifreq);
        
        // do the work similar to ifaddrs_p
        for (i = 0; i < ifc_num; i++) {
          socket_p = &ifr[i];
          if (socket_p->ifr_addr.sa_family != AF_INET)
            continue;
          
          if (ioctl(sd, SIOCGIFADDR, socket_p) == 0)
          {
            pAddr = (struct sockaddr_in *) &socket_p->ifr_addr;
            
            addr = inet_ntoa(pAddr->sin_addr);
            
            if (strncmp(socket_p->ifr_name, "lo", 2)    != 0  && 
                strncmp(addr, LOCALHOST, LOCALHOST_LEN) != 0  && 
                !(socket_p->ifr_flags & IFF_LOOPBACK) )
            {
              SETRET(ip, addr);
              shutdown(sd, 2);
              free(ifr);
              return ip;
            }
          }
        }
      }
      
      // in case not found
      free(ifr);
    }
  }
  
  // found nothing
  shutdown(sd, 2);
  error(ERRMSG);
  
  return R_NilValue;
}
#endif // end of IF_ADDRS conditional
#endif // end #if !OS_WINDOWS



#if OS_WINDOWS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#define CHECK_ALLOC(ptr) \
  if (ptr == NULL) \
    error("Unable to allocate memory\n");

static inline SEXP ip_internal_win()
{
  SEXP ip;
  char *addr;
  
  PMIB_IPADDRTABLE pIPAddrTable, realloc_ptr;
  DWORD pdwSize = 0;
  DWORD check;
  IN_ADDR IPAddr;
  
  pIPAddrTable = malloc(sizeof(MIB_IPADDRTABLE));
  CHECK_ALLOC(pIPAddrTable);
  
  if (GetIpAddrTable(pIPAddrTable, &pdwSize, 0) == ERROR_INSUFFICIENT_BUFFER)
  {
    realloc_ptr = (MIB_IPADDRTABLE *) realloc(pIPAddrTable, pdwSize);
    CHECK_ALLOC(realloc_ptr);
    pIPAddrTable = realloc_ptr;
  }
  
  check = GetIpAddrTable(pIPAddrTable, &pdwSize, 0);
  if (check != NO_ERROR)
    error("GetIpAddrTable returned error number %d\n", check);
  
  const int num_entries = pIPAddrTable->dwNumEntries;
  for (int i=0; i<num_entries; i++)
  {
    IPAddr.S_un.S_addr = pIPAddrTable->table[i].dwAddr;
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
  
  free(pIPAddrTable);
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
