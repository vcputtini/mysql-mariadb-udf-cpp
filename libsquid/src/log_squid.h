#ifndef VOLNEI_LOGSQUID_H
#define VOLNEI_LOGSQUID_H

#include <mysql.h>

/** NOTE
 * The USING_MARIADB flag, is defined in the Makefile
 * file as follows: -DUSING_MARIADB=1
 */
#ifdef USING_MARIADB
#include <mariadb_ctype.h>
#else
#include <m_ctype.h>  // maybe MySQL
#endif

#include <cstdio>
#include <iostream>
#include <sstream>
#include <cstring>
#include <cfenv>  // floating-point exceptions

#define LOG_FORMAT  0
#define LOG_PART  1
#define LOG_LINE  2

#ifdef HAVE_DLOPEN
extern "C" {
  my_bool log_squid_init( UDF_INIT* initid, UDF_ARGS* args, char* message );
  char* log_squid(UDF_INIT *initid, UDF_ARGS *args, char* result, unsigned long* length,	char *is_null, char *error);
} // extern "C"

#endif // HAVE_DLOPEN

#endif // VOLNEI_LOGSQUID_H
