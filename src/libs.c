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

    printf("[%s] %s\r\n", type, buf);
    fflush(stdout);
}

MZ_API int mz_atoi(const char *v, int def)
{
    if (!v)
        return def;

    return atoi(v);
}

MZ_API int mz_strequal(const char *str1, const char *str2)
{
    return strcmp(str1, str2) == 0;
}

MZ_API char* mz_strtrim(char *str, const char *delimit)
{
    return str;
}
