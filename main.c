#include <stdio.h>
#include <time.h>

#include "errors.h"
#include "ioutil.h"
#include "table.h"

struct song {
    i32  size;
    char name[50];
};


int main() {
    i32 status;
    status = format_table("songs", sizeof(struct song));
    printf("%d\n", status);
    return 0;
}
