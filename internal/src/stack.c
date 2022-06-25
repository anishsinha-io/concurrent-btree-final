#include "stack.h"

struct stack *stack(printfunc print) {
    struct stack *stack = malloc(sizeof(struct stack));
    stack->print = print;
    stack->size  = 0;
    stack->top   = NULL;
    return stack;
}

static struct stack_frame *stack_frame(void *key) {
    struct stack_frame *sf = malloc(sizeof(struct stack_frame));
    sf->key  = key;
    sf->next = NULL;
    return sf;
}

void stack_push(struct stack *s, void *key) {
    if (!s || !key) return;
    struct stack_frame *sf = stack_frame(key);
    sf->next = s->top;
    s->top   = sf;
    s->size++;
}

void *stack_pop(struct stack *s) {
    struct stack_frame *top = s->top;
    if (top) {
        s->top = s->top->next;
        s->size--;
        return top->key;
    }
    return NULL;
}

void *stack_top(struct stack *s) {
    return s->top;
}

void stack_print(struct stack *s) {
    struct stack_frame *itr = s->top;
    while (itr) {
        s->print(itr->key);
        itr = itr->next;
    }
    printf("\n");
}