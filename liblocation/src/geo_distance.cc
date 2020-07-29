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
#include "geo_distance.h"

#ifdef HAVE_DLOPEN

extern "C" {

struct Temp{
    LocationTools::MetricalType mt;
};

my_bool geo_distance_init( UDF_INIT* initid, UDF_ARGS* args, char* message )
{
  if(args->arg_count != 5 ) {
    strcpy(message, "Wrong number of arguments: lat_a, lon_a, lat_b, lon_b, 'unit'");
    return 1;
  }
  if((args->arg_type[0] != REAL_RESULT) || (args->arg_type[1] != REAL_RESULT) ||
     (args->arg_type[2] != REAL_RESULT) || (args->arg_type[3] != REAL_RESULT)) {
    strcpy(message, "Latitude and Longitude must be a DOUBLE(M,D)");
    return 1;
  }

  Temp *temp = new Temp;
  initid->ptr = (char*)temp;

  if(args->arg_count == 5) {
    if(args->arg_type[4] != STRING_RESULT) {
      strcpy(message, "Valid arguments are only string.");
      return 1;
    }
    std::stringstream ss;
    ss << (char*)args->args[4];
    if(ss.str() == "km") temp->mt = LocationTools::MetricalType::Km;
    else if(ss.str() == "mi") temp->mt = LocationTools::MetricalType::Mi;
    else {
      strcpy(message, "Valid arguments are: km|mi");
      return 1;
    }
  }

  initid->maybe_null = 0; // don't return null;

	return 0;
}

void geo_distance_deinit( UDF_INIT* initid)
{
    delete initid->ptr;
}

double geo_distance( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* error )
{
  double d = 0.0;
  Temp *t = (Temp*)initid->ptr;

  if(args->arg_count == 5) {
    if( (*((double*)args->args[0]) < -90.0 || *((double*)args->args[0]) > 90.0 ) ||
        (*((double*)args->args[2]) < -90.0 || *((double*)args->args[2]) > 90.0 ) ) {
        *error=1;
    }
    if( (*((double*)args->args[1]) < -180.0 || *((double*)args->args[1]) > 180.0 ) ||
        (*((double*)args->args[3]) < -180.0 || *((double*)args->args[3]) > 180.0 ) ) {
        *error=1;
    }

    d = LocationTools::distance(*((double*)args->args[0]), *((double*)args->args[1]),
                        *((double*)args->args[2]), *((double*)args->args[3]),
                        t->mt);
  }
  return d;
}

} // extern "C"

#endif // HAVE_DLOPEN
