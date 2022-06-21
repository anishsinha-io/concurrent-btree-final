#ifndef __ERRORS
#define __ERRORS

typedef unsigned char          u8;
typedef unsigned short         u16;
typedef unsigned int           u32;
typedef unsigned long long int u64;
typedef char                   i8;
typedef short                  i16;
typedef int                    i32;
typedef long long int          i64;

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