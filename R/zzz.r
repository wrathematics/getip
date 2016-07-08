.onLoad <- function(libname, pkgname)
{
  library.dynam("getip", pkgname, libname)
  invisible()
}


.onUnload <- function(libpath)
{
  library.dynam.unload("getip", libpath)
  invisible()
}

