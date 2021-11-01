#' hostname2ip
#' 
#' Converts a hostname (or vector of hostnames) to 
#' 
#' @details
#' Only IPv4 addresses will be returned. If only IPv6 addresses exist (and if
#' that is the case, allow me to be the first to greet you, person from the
#' distant future) then \code{NA} is returned for that hostname.
#' 
#' The function uses \code{getaddrinfo()} on *NIX systems, and
#' \code{gethostbyname()} on Windows.
#' 
#' @param hosts
#' A vector of hostnames as strings (or just one).
#' 
#' @return
#' A list of vectors of IP's.  Each IP is stored as a string.
#' 
#' @examples
#' \donttest{
#' hosts = c("www.google.com", "localhost", "www.yahoo.com")
#' hostname2ip(hosts)
#' }
#' 
#' @useDynLib getip R_hostname2ip
#' @export
hostname2ip = function(hosts)
{
  .Call(R_hostname2ip, hosts)
}
