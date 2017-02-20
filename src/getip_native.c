/* Automatically generated. Do not edit by hand. */

#include <R.h>
#include <Rinternals.h>
#include <R_ext/Rdynload.h>
#include <stdlib.h>

extern SEXP R_hostname2ip(SEXP hosts);
extern SEXP R_ip_internal();
extern SEXP R_validate_ipv4(SEXP ip_);

static const R_CallMethodDef CallEntries[] = {
  {"R_hostname2ip", (DL_FUNC) &R_hostname2ip, 1},
  {"R_ip_internal", (DL_FUNC) &R_ip_internal, 0},
  {"R_validate_ipv4", (DL_FUNC) &R_validate_ipv4, 1},
  {NULL, NULL, 0}
};
void R_init_getip(DllInfo *dll)
{
  R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
  R_useDynamicSymbols(dll, FALSE);
}
