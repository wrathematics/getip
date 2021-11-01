#' @useDynLib getip R_ip_internal
ip_internal = function()
{
  .Call(R_ip_internal)
}
