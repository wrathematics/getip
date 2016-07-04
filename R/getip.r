#' ip_internal
#' 
#' Get the local/internal IP address.
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
  ip <- .Call(C_ip_internal)
  return(ip)
}



#' ip_internal
#' 
#' Get the local/internal IP address.
#' 
#' @details
#' The function returns a lookup from \url{http://httpbin.org/ip}.
#' You must be connected to the internet for this to work.
#' 
#' @return
#' Returns the local IP address as a string.
#' 
#' @examples
#' \dontrun{
#' ip_external()
#' }
#' 
#' @export
ip_external <- function()
{
  url <- "http://httpbin.org/ip"
  ip <- tryCatch(readLines(url)[2L], warning=identity)
  
  if (inherits(ip, "simpleWarning"))
    stop("Unable to determine external IP")
  
  ip <- gsub("(  \"origin\": \"|\")", "", ip)
  return(ip)
}
