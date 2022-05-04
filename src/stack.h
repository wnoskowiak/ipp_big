#ifndef STACK_H_
#define STACK_H_
#include <stdlib.h>
#include <stdbool.h>

struct PhoneForward;

typedef struct PhoneForward PhoneForward;

typedef struct stack stack_t;

// sprawdzanie czy kolejka jest pusta
bool is_empty(stack_t *stack);

// sprawdzanie czy kolejka pełna
bool is_full(stack_t *stack);

// inicjalizacja kolejki
stack_t *stack_initialize(size_t cap);

// zwalnianie pamięci kolejki
void stack_destroy(stack_t *stack);

// zwiększanie rozmiaru kolejki
stack_t *stack_resize(stack_t *stack);

// dodawanie elementu do kolejki
stack_t *add(stack_t *stack, PhoneForward *item);

// usuwanie elementu z kolejki
PhoneForward *pop(stack_t *stack);

#endif