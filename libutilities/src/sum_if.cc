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

#include "sum_if.h"
#include "common.h"

/**
 * NOTA: Numeros tipo INTEGER devem ser long long ou int64_t
 */

#ifdef HAVE_DLOPEN

extern "C" {

using namespace ns_common_if;

my_bool sum_if_init( UDF_INIT* initid, UDF_ARGS* args, char* message )
{
  Buffer *buf = new Buffer;
  buf->value = 0.0;
  buf->acc = 0;
  initid->ptr = (char*)buf;
  initid->maybe_null = 1;

  if( args->arg_count == 3) {

    if( args->arg_type[0] == STRING_RESULT) {
      strcpy(message,"Arg.1: Type of agumente must be numeric");
      return 1;
    }
    if( args->arg_type[2] == STRING_RESULT) {
      strcpy(message,"Arg.2: Type of agumente must be numeric");
      return 1;
    }


    if( args->arg_type[0] == INT_RESULT ) {
        args->arg_type[2] = INT_RESULT;
    } else {
      if( args->arg_type[0] == DECIMAL_RESULT)
          args->arg_type[0] = REAL_RESULT;
      if( args->arg_type[2] == DECIMAL_RESULT)
          args->arg_type[2] = REAL_RESULT;
    }

    std::string s( (char*)args->args[1]);
    if(s == ">") buf->cond = eCondOperators::Gt;
    else if(s == ">=") buf->cond = eCondOperators::Ge;
    else if(s == "<") buf->cond = eCondOperators::Lt;
    else if(s == "<=") buf->cond = eCondOperators::Le;
    else if(s == "=") buf->cond = eCondOperators::Eq;
    else if(s == "<>") buf->cond = eCondOperators::Ne;
    else {
      sprintf(message,"Invalid operador [%s]. Valid are: > >= < <= = <>", s.c_str());
      return 1;
    }
  } else {
    strcpy(message,"Wrong number of arguments: (fieldname,'operator',value|fieldname)");
    return 1;
  }

	return 0;
}

void sum_if_deinit( UDF_INIT* initid )
{
	delete initid->ptr;
}

void sum_if_clear( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error )
{
  Buffer *buf = (Buffer*)initid->ptr;
  buf->value = 0.0;
  buf->acc = 0.0;
}

void sum_if_reset( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error )
{
  sum_if_clear( initid, args, is_null, error );
  sum_if_add(  initid,  args, is_null, error );
}

void sum_if_add( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error )
{
  Buffer *buf = (Buffer*)initid->ptr;

  std::feclearexcept(FE_ALL_EXCEPT);

  switch (buf->cond) {
    case eCondOperators::Gt:
      if( isgreater(*((double*)args->args[0]),
                    *((double*)args->args[2])) ) buf->value += *((double*)args->args[0]);
      break;
    case eCondOperators::Ge:
      if( isgreaterequal(*((double*)args->args[0]),
                         *((double*)args->args[2])) ) buf->value += *((double*)args->args[0]);
      break;
    case eCondOperators::Lt:
      if( isless(*((double*)args->args[0]),
                 *((double*)args->args[2])) ) buf->value += *((double*)args->args[0]);
      break;
    case eCondOperators::Le:
      if( islessequal(*((double*)args->args[0]),
                      *((double*)args->args[2])) ) buf->value += *((double*)args->args[0]);
      break;
    case eCondOperators::Eq: {
      if(args->arg_type[0] == INT_RESULT ) {
         if( *((int64_t*)args->args[0]) == *((int64_t*)args->args[2]) ) buf->value += *((double*)args->args[0]);
      } else {
        if( cmpdouble_equal(*((double*)args->args[0]),
                         *((double*)args->args[2])) ) buf->value += *((double*)args->args[0]);
      }
      break;
    }
    case eCondOperators::Ne: {
      if(args->arg_type[0] == INT_RESULT) {
         if( *((int64_t*)args->args[0]) != *((int64_t*)args->args[2]) ) buf->value += *((double*)args->args[0]);
      } else {
        if( !cmpdouble_equal(*((double*)args->args[0]),
                            *((double*)args->args[2])) ) buf->value += *((double*)args->args[0]);
      }
    }
  } // switch

  if(std::fetestexcept(FE_OVERFLOW))
    *error=1;
}

double sum_if( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* error )
{
  Buffer *buf = (Buffer*)initid->ptr;
  return buf->value;
}

} // extern "C"

#endif // HAVE_DLOPEN
