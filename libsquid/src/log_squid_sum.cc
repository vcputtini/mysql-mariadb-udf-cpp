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

#include "log_squid_sum.h"
#include "libsquid.h"

#ifdef HAVE_DLOPEN
extern "C" {

using namespace nsLogFormat;

my_bool log_squid_sum_init( UDF_INIT* initid, UDF_ARGS* args, char* message )
{
  int64_t *i = new int64_t;
  *i = 0;
  initid->ptr = (char*)i;
  initid->maybe_null = 0;

  if(args->arg_count < 3) {
    strcpy(message, "Wrong number of arguments: ");
    return 1;
  } else {
    if((args->arg_type[LOG_FORMAT] != STRING_RESULT) ||
       (args->arg_type[LOG_PART] != STRING_RESULT) ||
       (args->arg_type[LOG_LINE] != STRING_RESULT)) {
       std::strcpy(message, "Only type string arguments are accepted");
       return 1;
     }
  }

  if(args->arg_type[LOG_FORMAT] == STRING_RESULT) { // formato do log
    std::string s(args->args[LOG_FORMAT], args->lengths[LOG_FORMAT]);
    std::string f="squidcommoncombined";
    std::size_t found = f.find(s);
    if( found == std::string::npos) {
      strcpy(message, "Arg.1 (LOGFORMAT): Invalid!. Valid are: squid|common|combined" );
      return 1;
    }
  } else {
    strcpy(message, "Arg.1 (LOGFORMAT): Must be a STRING ");
    return 1;
  }

  if(args->arg_type[LOG_PART] == STRING_RESULT) { // log's part
    if( args->lengths[LOG_PART] == 0) {
      strcpy(message,"Arg.2 (PART): Cannot be empty. ");
      return 1;
    }
    std::string s(args->args[LOG_PART], args->lengths[LOG_PART]);
    std::string f="total_size_reqresponse_time";
    std::size_t found = f.find(s);
    if( found == std::string::npos) {
      strcpy(message, "Arg.1 (LOGPART): Invalid!. Valid are: total_size_req|response_time" );
      return 1;
    }
  } else {
    strcpy(message, "Arg.2 (PART): Must be a STRING ");
    return 1;
  }

  if(args->arg_type[LOG_LINE] == STRING_RESULT) { // log's line
    if( args->lengths[LOG_LINE] == 0) {
      strcpy(message,"Arg.3 (LOG LINE): Cannot be empty. ");
      return 1;
    }
  } else {
    strcpy(message, "Arg.3 (LOG LINE): Must be a STRING ");
    return 1;
  }
  return 0;
}

void log_squid_sum_deinit( UDF_INIT* initid )
{
  delete (int64_t*)initid->ptr;
}

void log_squid_sum_clear( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error )
{
  *((int64_t*)initid->ptr) = 0;
}

void log_squid_sum_reset( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error )
{
  log_squid_sum_clear(initid, args, is_null, error );
  log_squid_sum_add(initid, args, is_null, error );
}

void log_squid_sum_add( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char *error )
{

  std::string slogformat(args->args[LOG_FORMAT], args->lengths[LOG_FORMAT]);
  std::string spart(args->args[LOG_PART], args->lengths[LOG_PART]);
  std::string slog(args->args[LOG_LINE], args->lengths[LOG_LINE]);

  LogSquid ls;
  ls.setFormat(slogformat);
  ls.setPart(spart);
  ls.setLogReg(slog);
  std::string str(ls.getPart());
  if(str.empty()) {
    *error = 1;
  }

  int64_t total=0;
  std::istringstream ss(str);
  ss >> std::fixed >> total;

  if(*((int64_t*)initid->ptr) > 0 && total > LONG_MAX - *((int64_t*)initid->ptr) )  {
    *error=1; // overflow
  }

  *((int64_t*)initid->ptr) +=  total;
}

int64_t log_squid_sum( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* error )
{
  return *((int64_t*)initid->ptr);
}

} // extern "C"

#endif // HAVE_DLOPEN
