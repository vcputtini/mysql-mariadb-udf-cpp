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
#include "unit_conv.h"

#ifdef HAVE_DLOPEN

extern "C" {
my_bool unit_convert_init( UDF_INIT* initid, UDF_ARGS* args, char* message )
{
  if(args->arg_count > 3 ) {
    strcpy(message, "Syntax: unit_convert(value, [dec,] \"convertion_type\")");
    return 1;
  }

  if( args->arg_type[0] != REAL_RESULT) {
    strcpy(message, "Argument 1: Must be a DOUBLE(M,D)");
    return 1;
  }

  if(args->arg_type[1] == INT_RESULT) {
     if(*((int*)args->args[1]) < 1) {
       strcpy(message, "Decimal place must be grather than or equal 1.");
       return 1;
     }
     if(*((int*)args->args[1]) > 10) initid->decimals = 10; // limit to 10 dec
     else initid->decimals = *((int*)args->args[1]);
     if(args->arg_type[2] != STRING_RESULT) {
       strcpy(message, "convertion_type wasn't informed: e.g.: \"kmh_to_mih\"");
       return 1;
     }
  } else if(args->arg_type[1] != STRING_RESULT) {
    strcpy(message, "convertion_type wasn't informed: e.g.: \"kmh_to_mih\"");
    return 1;
  }

  initid->maybe_null = 0; // don't return null;
	return 0;
}

void unit_convert_deinit( UDF_INIT* initid)
{
}

double unit_convert( UDF_INIT* initid, UDF_ARGS* args, char* is_null, char* error )
{
  std::string s;
  if(args->arg_type[1] == STRING_RESULT) {
    std::string sx((char*)args->args[1]);
    s = sx;
  } else {
    std::string sx((char*)args->args[2]);
    s = sx;
  }
  double d = *((double*)args->args[0]);

  boost::algorithm::to_lower(s);
  if( s == "kmh_to_mih") {
    return UnitConverter::Kmh2Mph(d);
  } else if( s == "mih_to_kmh") {
    return UnitConverter::Mph2Kmh(d);
  } else if( s == "mi_to_meters") {
    return UnitConverter::Mi2Meters(d);
  } else if( s == "meters_to_mi") {
    return UnitConverter::Meters2Mi(d);
  } else if( s == "feet_to_meters") {
    return UnitConverter::Feet2Meters(d);
  } else if( s == "meters_to_feet") {
    return UnitConverter::Meters2Feet(d);
  }

  return d;
}

} // extern "C"

#endif // HAVE_DLOPEN
