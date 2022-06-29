#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "errors.h"
#include "defaults.h"
#include "lehman-yao.h"
#include "index.h"
#include "stringbuilder.h"

typedef struct bloodborne_character {
    char name[50], location[100];
    u32  num_phases;
    u64  blood_echoes;
} b_char;

// lol
struct bloodborne_character *bbc(char *name, char *location, u32 num_phases, u64 blood_echoes) {
    // lol
    struct bloodborne_character *bbc = malloc(sizeof(*bbc));
    strncpy(bbc->name, name, sizeof(bbc->name));
    strncpy(bbc->location, location, sizeof(bbc->location));
    bbc->num_phases = num_phases;
    return bbc;
}

static int compare_u64(const void *first, const void *second) {
    return (int) (*(u64 *) first - *(u64 *) second);
}

static void print_u64(const void *el) {
    printf("%llu ", *(u64 *) el);
}

static void print_character(const void *character) {
    printf("%s ", (*(struct bloodborne_character *) character).name);
}

static int compare_character(const void *first, const void *second) {
    return (int) ((*(struct bloodborne_character *) first).blood_echoes -
                  (*(struct bloodborne_character *) second).blood_echoes);
}

int main() {
    format_index("/Users/anishsinha/Home/scratch/lock-table-final/test_database/test_index.bin",
                 "bloodborne_characters", sizeof(b_char));
    struct b_link_entry test_entry = {
            .id=613, .loc=0
    };
    b_link_insert("/Users/anishsinha/Home/scratch/lock-table-final/test_database/test_index.bin", &test_entry);
    return 0;
}
