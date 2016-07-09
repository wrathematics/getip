#' ip_internal
#' 
#' Get your local/internal IP address.
#' 
#' @details
#' The lookup is done in-process, i.e., it does not call
#' \code{system()} or the like.
#' 
#' @return
#' Returns the local IP address as a string.
#' 
#' @examples
#' \dontrun{
#' ip_internal()
#' }
#' 
#' @export
ip_internal <- function()
{
  ip <- .Call(C_ip_internal, PACKAGE = "getip")
  return(ip)
}


ip_internal_solaris <- function()
{
  ip <- NULL

  if (Sys.info()["sysname"] == "SunOS")
  {
    ### For Solaris 10 only. Solaris 11 may have "ifaddrs.h"
    cmd <- "/usr/sbin/ifconfig -a4"
    ret <- suppressWarnings(system(cmd, intern = TRUE))

    if (length(ret) == 0)
      stop("ifconfig is not avaliable")
    else
    {
      pattern <- "inet .* netmask .* broadcast .*"
      id <- grepl(pattern, ret, perl = TRUE)

      if (sum(id) == 0)
        stop("inet is not found")
      else
      {
        ### Take the first matched for internal (may have more adaptors)
        tmp <- ret[id][1]
        pattern <- ".*inet (.*) netmask .* broadcast .*"
        replacement <- "\\1"
        ip <- gsub(pattern, replacement, tmp, perl = TRUE)
      }
    }
  }
  else
    stop("OS is not Solaris")

  return(ip)
}

