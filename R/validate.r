validate_ipv4 <- function(ip)
{
  if (length(ip) != 1L || !is.character(ip) || is.na(ip))
    return(FALSE)
  
  .Call(C_validate_ipv4, ip)
}
