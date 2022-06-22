/*
** June 17, 2022
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
***********************************************************************************************************************
**
** This file is part of the `errors` module. This module contains no implementation and is only a header. Only types
** are defined here, as well as #define'd constants and a few macros to handle errors.
**
*/

#ifndef __ERRORS
#define __ERRORS

#include <stdbool.h>

#define PAGE_SIZE 1000
#define ORDER 2;

typedef unsigned char          u8;
typedef unsigned short         u16;
typedef unsigned int           u32;
typedef unsigned long long int u64;
typedef char                   i8;
typedef short                  i16;
typedef int                    i32;
typedef long long int          i64;


typedef void(*printfunc)(const void *);
typedef bool(*cmpfunc)(const void *, const void *);
typedef u64 (*hashfunc)(const void *, u64);

#define err_abort(code, text) do { \
    fprintf(stderr, "%s at \"%s\":%d: %s\n", \
    text, __FILE__, __LINE__, strerror(code));\
                                 abort();  \
                                  \
} while(0)

#define errno_abort(text) do { \
    fprintf(stderr, "%s at \"%s\": %d: %s\n", text, __FILE__, __LINE__, strerror(errno)); abort();                            \
} while(0)

#endif