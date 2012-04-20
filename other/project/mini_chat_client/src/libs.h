#ifndef __MC_LIBS_H__
#define __MC_LIBS_H__

#include "defs.h"
#include <stdio.h>
#include <memory.h>

#define mc_memset(dst, value, size) memset(dst, value, size)

MC_API void print_log(const char *type, const char *format, ...);

#define logI(arg...) print_log("Info", arg)
#define logE(arg...) print_log("Error", arg)

#endif
