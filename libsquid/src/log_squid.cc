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

#include "log_squid.h"
#include "libsquid.h"

#ifdef HAVE_DLOPEN

extern "C" {

using namespace nsLogFormat;

my_bool log_squid_init( UDF_INIT* initid, UDF_ARGS* args, char* message )
{
  initid->maybe_null = 1;

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
    std::string f="squidcommoncombinedreferreruseragent";
    std::size_t found = f.find(s);
    if( found == std::string::npos) {
      strcpy(message, "Arg.1 (LOGFORMAT): Invalid!. Valid are: squid|common|combined|referrer|useragent" );
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
    ReservedWords rw;
    if( !rw.is_valid(s)) {
      sprintf(message,"Arg.2 (PART): not valid. [ %s ]", (char*)args->args[LOG_PART] );
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

char* log_squid(UDF_INIT *initid, UDF_ARGS *args, char* result,
                            unsigned long* length,	char *is_null, char *error)
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

  /*
   * In order to avoid problems with very complex and long URLs it is necessary
   * to make the correct allocation of memory, before copying the
   * content to 'result'
   * Based in: https://stackoverflow.com/questions/7352099/stdstring-to-char
   *
   */
  result = std::strcpy((char*)malloc(str.length()+1), str.c_str());
  *length = (unsigned long) std::strlen(result);
  return result;
}

} // extern "C"

#endif // HAVE_DLOPEN
