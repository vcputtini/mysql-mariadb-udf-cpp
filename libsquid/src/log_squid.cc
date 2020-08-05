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

char* log_squid(UDF_INIT *initid, UDF_ARGS *args, char* result, unsigned long* length,	char *is_null, char *error)
{
  std::string slogformat(args->args[LOG_FORMAT], args->lengths[LOG_FORMAT]);
  std::string spart(args->args[LOG_PART], args->lengths[LOG_PART]);
  std::string slog(args->args[LOG_LINE], args->lengths[LOG_LINE]);

  LogSquid ls;
  ls.setFormat(slogformat);
  ls.setPart(spart);
  ls.setLogReg(slog);
  std::string str = ls.getPart();
  if(str.empty()) {
    *error = 1;
  }

  sprintf(result, str.c_str());
  *length = (uint) strlen(result);
  return result;
}

} // extern "C"

#endif // HAVE_DLOPEN
