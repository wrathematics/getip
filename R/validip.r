#' validip
#' 
#' Check if a string is a valid IP address. Currently only IPv4 addresses are
#' supported.
#' 
#' @details
#' This function is not like a 'ping'. It merely checks if the string is
#' correctly formatted and could theoretically be a valid IP address.
#' 
#' @param ip
#' A string or vector of strings.
#' 
#' @return
#' Returns \code{TRUE} if the input is a potentially valid IP address, and
#' \code{FALSE} otherwise.
#' 
#' @examples
#' library(getip)
#' 
#' validip(c("192.168.1.1", "700.168.1.1"))
#' 
#' @useDynLib getip R_validate_ipv4
#' @export
validip = function(ip)
{
  # v = as.integer(v)
  # if (!is.numeric(v) || is.na(v) || length(v) != 1 || v != 4)
  #   stop("argument 'v' should be 4 or 6")
  
  if (length(ip) == 0)
    return(logical(0))
  if (!is.character(ip))
    return(rep(FALSE, length(ip)))
  else
    .Call(R_validate_ipv4, ip)
}
