#include <stdio.h>
#include <time.h>

#include "errors.h"
#include "lehman-yao.h"
#include "slice.h"
#include "table.h"

bool compare_string(const void *first, const void *second) {
    return !strncmp((const char *) first, (const char *) second,
                    strlen((const char *) first) > strlen((const char *) second) ? strlen((const char *) second)
                                                                                 : strlen((const char *) first));
}

i32 compare_string_slice(const void *first, const void *second) {
    return strncmp((const char *) first, (const char *) second,
                   strlen((const char *) first) > strlen((const char *) second) ? strlen((const char *) second)
                                                                                : strlen((const char *) first));
}

void print_string(const void *first) {
    printf("%s ", (const char *) first);
}

u64 hash_string(const void *str, u64 size) {
    u64        sum     = 0;
    const char *string = str;
    for (int   i       = 0; i < strlen(string); i++) sum += string[i];
    return sum % size;
}

int main() {
    struct slice *s = slice(compare_string_slice, print_string);
    return 0;
}
