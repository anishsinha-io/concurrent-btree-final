#include <stdio.h>
#include <time.h>

#include "errors.h"
#include "slice.h"

i32 compare_string(const void *first, const void *second) {
    return strncmp((const char *) first, (const char *) second,
                   strlen((const char *) first) > strlen((const char *) second) ? strlen((const char *) second)
                                                                                : strlen((const char *) first));
}

void print_string(const void *first) {
    printf("%s ", (const char *) first);
}

int main() {
    const char   *a    = "apple";
    const char   *b    = "book";
    struct slice *test = slice(compare_string, print_string);
    printf("%llu\n", test->length);
    printf("%llu\n", test->capacity);
    return 0;
}
