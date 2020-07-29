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
#ifndef VOLNEI_LIBLOCATION_H
#define VOLNEI_LIBLOCATION_H

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

#include "locationtools.h"
#include <cstdio>
#include <boost/algorithm/string.hpp>

#ifdef HAVE_DLOPEN

/*
 * NAME: unit_convert
 * SUBJECT: Convenience function to convert between some measurement units.
 * PARAMS:
 *  IN:
 *      (value to be converted, [dec-places,] convertion_type)
 *  OUT: converted value.
 * COMMENTS: e.g.: unit_conv(100,3,"kmh_to_mih")
 * Valid convertion types are: kmh_to_mih|mih_to_kmh|mi_to_meters|meters_to_mi|
 *                             feet_to_meters|meters_to_feet
 * Adapted from orignal: <https://www.omnicalculator.com/other/azimuth>
 */
extern "C" {
  my_bool unit_convert_init( UDF_INIT* initid, UDF_ARGS* args, char* message );
  void unit_convert_deinit( UDF_INIT* initid);
  double unit_convert( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* error );
} // extern "C"

#endif // HAVE_DLOPEN

#endif // VCP_LIBLOCATION_H
