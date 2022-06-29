#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "errors.h"
#include "defaults.h"
#include "lehman-yao.h"
#include "index.h"
#include "stringbuilder.h"

struct bloodborne_character {
    char name[50], location[100];
    u32  num_phases;
    u64  blood_echoes;
};

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


int main() {
    return 0;
}
