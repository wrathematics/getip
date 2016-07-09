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
  ip <- .Call(C_ip_internal)
  return(ip)
}
