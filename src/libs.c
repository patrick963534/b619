#include <mz/libs.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

MZ_API void mz_print_log(const char *type, const char *format, ...)
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

MZ_API int mz_is_power_of_2(int v)
{
    return (v & (v - 1)) == 0;
}

MZ_API int mz_log2_int(int v)
{
    int power = 0;
    int temp = v;

    assert(v >= 0);

    for (power = 0; temp != 0; power++)
        temp /= 2;
 
    return power;
}

MZ_API int mz_pow_int(int base, int x)
{
    int i;
    int result = 1;

    assert(x >= 0);

    if (base == 0)
        return 0;

    if (x == 0)
        return 1;

    for (i = x; i != 0; i--)
        result *= base;

    return base;
}

MZ_API int mz_get_larger_power_of_2(int v)
{
    if (mz_is_power_of_2(v))
        return v;
    else
        return mz_pow_int(2, mz_log2_int(v)+ 1);
}
