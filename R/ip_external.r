#' ip_external
#' 
#' Get your external/public IP address.
#' 
#' @details
#' The function returns a lookup from one of several services, including
#' Amazon AWS \url{http://checkip.amazonaws.com/}, 
#' httpbin \url{http://httpbin.org/ip}, ipify \url{https://www.ipify.org/},
#' and "My External IP address is ..." \url{http://myexternalip.com/}.
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
  services <- c("amazonaws", "httpbin", "ipify", "myexternalip")
  fns <- paste0("ip_external_", services, "()")
  
  for (fn in c(fns, fns))
  {
    ip <- eval(parse(text=fn))
    if (identical(ip, -1L))
      Sys.sleep(0.2)
    else
      break
  }
  
  if (identical(ip, -1L))
    stop("Unable to determine external IP")
  
  return(ip)
}

get_external <- function(url)
{
  ip <- tryCatch(readLines(url, warn=FALSE), warning=identity)
  
  if (inherits(ip, "simpleWarning"))
    return(-1L)
    # stop("Unable to determine external IP")
  
  return(ip)
}

ip_external_amazonaws <- function()
{
  url <- "http://checkip.amazonaws.com/"
  ip <- get_external(url)
  
  return(ip)
}

ip_external_httpbin <- function()
{
  url <- "http://httpbin.org/ip"
  ip <- get_external(url)
  
  if (identical(ip, -1L))
    return(ip)
  
  ip <- gsub("(  \"origin\": \"|\")", "", ip[2L])
  return(ip)
}

ip_external_ipify <- function()
{
  url <- "https://api.ipify.org/?format=json"
  ip <- get_external(url)
  
  if (identical(ip, -1L))
    return(ip)
  
  ip <- gsub("({\"ip\":\"|\"})", "", ip, perl=TRUE)
  return(ip)
}

ip_external_myexternalip <- function()
{
  url <- "http://myexternalip.com/raw"
  ip <- get_external(url)
  
  return(ip)
}
