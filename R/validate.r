#' @useDynLib getip R_validate_ipv4
validate_ipv4 = function(ip)
{
  if (length(ip) != 1L || !is.character(ip) || is.na(ip))
    return(FALSE)
  
  .Call(R_validate_ipv4, ip)
}
