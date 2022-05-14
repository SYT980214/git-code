#ifndef STR_H_
#define STR_H_
#include <os/config_env.h>

BEGIN_FBASE2_NAMESPACE

extern "C" API_EXPORT char *__strtok(char **s, const char c);

extern "C" API_EXPORT bool is_num( const char* sznum );

extern "C" API_EXPORT bool is_float( char* src );

END_FBASE2_NAMESPACE

#endif
