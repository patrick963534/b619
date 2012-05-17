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
#define mz_free(obj) free(obj)
#define mz_assert(v) assert(v)

#define mz_strdup(str) strdup(str)

MZ_API void print_log(const char *type, const char *format, ...);

#define logI(args...) print_log("Info", args) 
#define logE(args...) print_log("Error", args) 

#endif
