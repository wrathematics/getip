ip_internal <- function()
{
  ip <- .Call(C_ip_internal)
  return(ip)
}
