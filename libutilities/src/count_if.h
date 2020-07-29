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

#ifndef VOLNEI_UTILITIES_H
#define VOLNEI_UTILITIES_H

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

/*
 * NAME: count_if
 * SUBJECT: Counts the number of rows based on the condition.
 * PARAMS:
 *  IN: arg 0: fieldname
 *      arg 1: condition (=,<>,<,<=,>,>=)
 *      arg 2: value to comparision
 *  OUT: int64_t (long long)
 * COMMENTS: The first argument must be a field.
 */
#ifdef HAVE_DLOPEN
extern "C" {
  my_bool count_if_init( UDF_INIT* initid, UDF_ARGS* args, char* message );
  void count_if_deinit( UDF_INIT* initid );
  void count_if_clear( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error );
  void count_if_reset( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error );
  void count_if_add( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error );
  int64_t count_if( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* error );
} // extern "C"

#endif // HAVE_DLOPEN

#endif // VOLNEI_UTILITIES_H
