#ifndef __MZ_LIBS_H__
#define __MZ_LIBS_H__

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <mz/defs.h>

#define mz_unused(arg) (void)(arg)
#define mz_downcast(type) type* self = (type*)self_

#define mz_memcpy(dst, src, size) memcpy(dst, src, size)
#define mz_malloc(size) calloc(1, size)
#define mz_realloc(ptr, size) realloc(ptr, size)
#define mz_free(obj) free(obj)
#define mz_assert(v) assert(v)

#define mz_strdup(str) strdup(str)
#define mz_strtok(str, delimiter) strtok(str, delimiter) 
#define mz_strchr(str, ch) strchr(str, ch)
#define mz_strcmp(str1, str2) strcmp(str1, str2)

MZ_API int      mz_atoi(const char *value, int def);
MZ_API char*    mz_strtrim(char *str, const char *delimit);
MZ_API int      mz_strequal(const char *str1, const char *str2);

MZ_API void     mz_print_log(const char *type, const char *format, ...);

MZ_API int      mz_log2_int(int v);
MZ_API int      mz_pow_int(int base, int x);
MZ_API int      mz_is_power_of_2(int v);
MZ_API int      mz_get_larger_power_of_2(int v);

#define logI(args...) mz_print_log("Info", args) 
#define logE(args...) mz_print_log("Error", args) 

#endif
