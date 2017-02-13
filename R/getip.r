#' ip_external
#' 
#' Get your internal/local or external/public IP address.
#' 
#' @details
#' The internal/local IP lookup is done in-process, i.e., it does not call
#' \code{system()} or the like.
#' 
#' The function returns a lookup from one of several services, including
#' Amazon AWS \url{http://checkip.amazonaws.com/}, 
#' httpbin \url{http://httpbin.org/ip}, ipify \url{https://www.ipify.org/},
#' and "My External IP address is ..." \url{http://myexternalip.com/}.
#' You must be connected to the internet for this to work.
#' 
#' @return
#' Returns the requested IP address as a string.
#' 
#' @examples
#' \dontrun{
#' # internal/local address
#' getip("internal")
#' 
#' # external/public
#' getip("external")
#' }
#' 
#' @export
getip <- function(type="internal")
{
  type <- match.arg(tolower(type), c("internal", "external"))
  
  if (type == "internal")
    ip <- ip_internal()
  else if (type == "external")
    ip <- ip_external()
  
  ip
}
