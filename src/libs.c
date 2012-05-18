#include <mz/libs.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

MZ_API void print_log(const char *type, const char *format, ...)
{
    char buf[256];
    va_list va;

    va_start(va, format);
    vsnprintf(buf, sizeof(buf), format, va);
    va_end(va);

    printf("%s: %s\r\n", type, buf);
    fflush(stdout);
}

MZ_API int mz_atoi(const char *v, int def)
{
    if (!v)
        return def;

    return atoi(v);
}
