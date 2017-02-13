#'  getip
#' 
#' A micro-package for getting your IP address, either the local/internal or the
#' public/external one.  Currently only IPv4 addresses are supported.
#'
#' @name getip-package
#' 
#' @useDynLib getip C_ip_internal C_validate_ipv4
#' 
#' @docType package
#' @author Drew Schmidt
#' @keywords package
NULL
