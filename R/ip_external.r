get_external <- function(url)
{
  ip <- tryCatch(readLines(url, warn=FALSE), warning=identity)
  
  if (inherits(ip, "simpleWarning"))
    return(-1L)
  
  return(ip)
}



### Services
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



### interface
ip_external <- function()
{
  services <- c("amazonaws", "httpbin", "ipify", "myexternalip")
  fns <- paste0("ip_external_", services, "()")
  
  # randomize order
  fns <- sample(fns)
  
  num_tries <- 1L
  alldone = FALSE
  for (try in 1:num_tries)
  {
    for (fn in fns)
    {
      ip <- eval(parse(text=fn))
      if (identical(ip, -1L))
        Sys.sleep(0.2)
      else
      {
        alldone = TRUE
        break
      }
    }
    
    if (alldone)
      break
  }
  
  if (identical(ip, -1L))
    stop("Unable to determine external IP")
  
  return(ip)
}
