#ifndef __STRINGBUILDER
#define __STRINGBUILDER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"

struct stringbuilder {
    const char *buf;
    void (*appendchar)(struct stringbuilder *sb, char c);
    void (*appendstring)(struct stringbuilder *sb, char *str);
    void (*appendshort)(struct stringbuilder *sb, short s);
    void (*appendint)(struct stringbuilder *sb, int i);
    void (*appendlong)(struct stringbuilder *sb, long long l);
    void (*appendptr)(struct stringbuilder *sb, void *ptr, size_t size);
    void (*print)(struct stringbuilder *sb);
};

struct stringbuilder *stringbuilder();

#endif