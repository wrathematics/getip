#' ip_external
#' 
#' Get your internal/local or external/public IP address. Currently only IPv4
#' addresses are supported.
#' 
#' @details
#' The internal/local IP lookup is done in-process, i.e., it does not call
#' \code{system()} or the like.
#' 
#' For an external address, the function returns a lookup from one of several
#' services, including
#' Amazon AWS \url{http://checkip.amazonaws.com/}, 
#' httpbin \url{http://httpbin.org/ip},
#' ipify \url{https://www.ipify.org/},
#' and "My External IP address is ..." \url{http://myexternalip.com/}.
#' You must be connected to the internet for this to work. Please note that
#' pathological use could end up in your getting banned from these services, 
#' rendering the function useless. So don't call this function a billion times
#' in a loop or something.
#' 
#' @param type
#' One of \code{"local"} or \code{"internal"} for the local/internal IP, or
#' one of \code{"external"} or \code{"public"} for the external/public IP.
#' 
#' @return
#' Returns the requested IP address as a string.
#' 
#' @examples
#' library(getip)
#' 
#' # internal/local address
#' getip("local") # same as getip("internal")
#' 
#' \donttest{
#' # external/public
#' getip("public") # same as getip("external")
#' }
#' 
#' @export
getip = function(type="local")
{
  type = match.arg(tolower(type), c("local", "internal", "external", "public"))
  
  if (type == "internal" || type == "local")
    ip = ip_internal()
  else if (type == "external" || type == "public")
    ip = ip_external()
  
  ip
}
