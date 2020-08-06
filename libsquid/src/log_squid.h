/***************************************************************************
 * Copyright (c) 2020                                                      *
 *      Volnei Cervi Puttini.  All rights reserved.                        *
 *      vcputtini@gmail.com
 *                                                                         *
 * Redistribution and use in source and binary forms, with or without      *
 * modification, are permitted provided that the following conditions      *
 * are met:                                                                *
 * 1. Redistributions of source code must retain the above copyright       *
 *    notice, this list of conditions and the following disclaimer.        *
 * 2. Redistributions in binary form must reproduce the above copyright    *
 *    notice, this list of conditions and the following disclaimer in the  *
 *    documentation and/or other materials provided with the distribution. *
 * 4. Neither the name of the Author     nor the names of its contributors *
 *    may be used to endorse or promote products derived from this software*
 *    without specific prior written permission.                           *
 *                                                                         *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND *
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE   *
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR      *
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS  *
 * BE LIABLEFOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR   *
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF    *
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS*
 * INTERRUPTION)                                                           *
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,     *
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING   *
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE      *
 * POSSIBILITY OFSUCH DAMAGE.                                              *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

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

/*
 * NAME: log_squid
 * SUBJECT: Returns the selected part of the Squid log line
 * PARAMS:
 *  IN: arg 0: squid | common | combined | referrer | useragent
 *      arg 1: Reserved Words: see list docs/reserved-words.txt<br>
 *      arg 2: Log line
 *  OUT: STRING
 * COMMENTS: The function will always return the values in the string type.
 * If the value returned is a number you should use CAST(expression AS type)
 * to do the conversion.
 */
#ifdef HAVE_DLOPEN
extern "C" {
  my_bool log_squid_init( UDF_INIT* initid, UDF_ARGS* args, char* message );
  void log_squid_deinit( UDF_INIT* initid );
  char* log_squid(UDF_INIT *initid, UDF_ARGS *args, char* result, unsigned long* length,	char *is_null, char *error);
} // extern "C"

#endif // HAVE_DLOPEN

#endif // VOLNEI_LOGSQUID_H
