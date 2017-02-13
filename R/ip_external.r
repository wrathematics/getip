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
