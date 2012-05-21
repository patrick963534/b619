#ifndef __MZ_LIBS_H__
#define __MZ_LIBS_H__

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <mz/defs.h>

#define mz_unused(arg)              (void)(arg)
#define mz_downcast(type)           type* self = (type*)self_

#define mz_memcpy(dst, src, size)   memcpy(dst, src, size)
#define mz_malloc(size)             calloc(1, size)
#define mz_realloc(ptr, size)       realloc(ptr, size)
#define mz_free(obj)                free(obj)
#define mz_assert(v)                assert(v)

#define mz_strdup(str)              strdup(str)
#define mz_strtok(str, delimiter)   strtok(str, delimiter)
#define mz_strchr(str, ch)          strchr(str, ch)
#define mz_strcmp(str1, str2)       strcmp(str1, str2)

MZ_API int      mz_atoi(const char *value, int def);
MZ_API char*    mz_strtrim(char *str, const char *delimit);
MZ_API int      mz_strequal(const char *str1, const char *str2);
MZ_API void     mz_snprintf(char *buf, int max_size, const char *format, ...);
MZ_API char*    mz_str_concat(char *buf, const char *str);
MZ_API int      mz_str_is_end_with(const char *str, const char *suffix);

MZ_API void     mz_print_log(const char *type, const char *format, ...);

MZ_API int      mz_log2_int(int v);
MZ_API int      mz_pow_int(int base, int x);
MZ_API int      mz_is_power_of_2(int v);
MZ_API int      mz_get_larger_power_of_2(int v);

MZ_API char*    mz_path_combine_path(char *ret_path, int max_path_size, const char *folder, const char *filename);
MZ_API char* mz_path_get_folder(char* buf, int max_size, const char *filename);

#define logI(args...) mz_print_log("Info", args) 
#define logE(args...) mz_print_log("Error", args) 

#define mz_max(a, b) max(a, b)
#define mz_min(a, b) min(a, b)

#define mz_fopen(path, mode)            fopen(path, mode)
#define mz_fprintf(fp, format, args...) fprintf(fp, format, args)
#define mz_fscanf(fp, format, args...)  fscanf(fp, format, args)
#define mz_fwrite(ptr, size, nmemb, fp) fwrite(ptr, size, nmemb, fp)
#define mz_fread(ptr, size, nmemb, fp)  fwrite(ptr, size, nmemb, fp)
#define mz_fclose(fp)                   fclose(fp)

MZ_API int      mz_file_write_string(FILE *fp, const char *str);
MZ_API int      mz_file_write_int(FILE *fp, int v);
MZ_API char*    mz_file_read_string(FILE *fp);
MZ_API int      mz_file_read_int(FILE *fp);

#endif
