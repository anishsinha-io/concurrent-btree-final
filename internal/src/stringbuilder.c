#include "stringbuilder.h"

static void stringbuilder_resize_buffer(struct stringbuilder *sb, size_t new_size) {
    sb->buf = realloc((void *) sb->buf, new_size);
}

void sb_print_buf(struct stringbuilder *sb) {
    printf("%s\n", sb->buf);
}

void appendchar(struct stringbuilder *sb, char c) {
    if (strlen(sb->buf) + sizeof(char) > sizeof(sb->buf))
        stringbuilder_resize_buffer(sb, (size_t) ((double) (sizeof(sb->buf)) * 1.15));
    memcpy((void *) (sb->buf + strlen(sb->buf)), &c, sizeof(c));
}

void appendstring(struct stringbuilder *sb, char *str) {
    if (strlen(sb->buf) + strlen(str) > sizeof(sb->buf))
        stringbuilder_resize_buffer(sb, (size_t) ((double) (sizeof(sb->buf)) * 1.15));
    memcpy((void *) (sb->buf + strlen(sb->buf)), str, strlen(str));
}

void appendshort(struct stringbuilder *sb, short s) {
    if (strlen(sb->buf) + sizeof(short) > sizeof(sb->buf))
        stringbuilder_resize_buffer(sb, (size_t) ((double) (sizeof(sb->buf)) * 1.15));
    memcpy((void *) (sb->buf + strlen(sb->buf)), &s, sizeof(s));
}

void appendint(struct stringbuilder *sb, int i) {
    if (strlen(sb->buf) + sizeof(int) > sizeof(sb->buf))
        stringbuilder_resize_buffer(sb, (size_t) ((double) (sizeof(sb->buf)) * 1.15));
    sprintf((void *) (sb->buf + strlen(sb->buf)), "%d", i);
}

void appendlong(struct stringbuilder *sb, long long l) {
    if (strlen(sb->buf) + sizeof(long) > sizeof(sb->buf))
        stringbuilder_resize_buffer(sb, (size_t) ((double) (sizeof(sb->buf)) * 1.15));
    sprintf((void *) (sb->buf + strlen(sb->buf)), "%lld", l);
}

void appendptr(struct stringbuilder *sb, void *ptr, size_t size) {
    if (strlen(sb->buf) + size > sizeof(sb->buf))
        stringbuilder_resize_buffer(sb, (size_t) ((double) (sizeof(sb->buf)) * 1.15));
    memcpy((void *) (sb->buf + strlen(sb->buf)), ptr, size);
}

struct stringbuilder *stringbuilder() {
    struct stringbuilder *sb = malloc(sizeof(struct stringbuilder));
    sb->buf          = malloc(1000);
    sb->appendchar   = appendchar;
    sb->appendstring = appendstring;
    sb->appendshort  = appendshort;
    sb->appendint    = appendint;
    sb->appendlong   = appendlong;
    sb->appendptr    = appendptr;
    sb->print        = sb_print_buf;
    return sb;
}

