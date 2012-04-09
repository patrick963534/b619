#ifndef __MZ_LIBS_H__
#define __MZ_LIBS_H__

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <mz/defs.h>

#define mz_malloc(size) malloc(size)

MZ_API void print_log(const char *type, const char *format, ...);

#define logI(format, args...) print_log("Info", format, args) 

#endif
