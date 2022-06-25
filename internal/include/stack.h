#ifndef __STACK
#define __STACK

#include <stdio.h>
#include <stdlib.h>

#include "errors.h"

struct stack_frame {
    void               *key;
    struct stack_frame *next;
};

struct stack {
    u64                size;
    struct stack_frame *top;
    printfunc          print;
};

void stack_push(struct stack *s, void *key);
void stack_print(struct stack *s);
void *stack_pop(struct stack *s);
void *stack_top(struct stack *s);
struct stack *stack(printfunc print);

#endif