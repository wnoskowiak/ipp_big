#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "stack.h"

struct PhoneForward;

typedef struct PhoneForward PhoneForward;

struct PhoneForward
{
    PhoneForward *parent;
    PhoneForward *further[10];
    char *redirect;
};

typedef struct stack
{
    int front;
    size_t cap;
    struct PhoneForward **array;
} stack_t;

// sprawdzanie czy kolejka jest pusta
bool is_empty(stack_t *stack)
{
    return (stack->front == -1);
}

// sprawdzanie czy kolejka pełna
bool is_full(stack_t *stack)
{
    return (((size_t)stack->front == stack->cap -1)||(stack->front == -1));
}

// inicjalizacja kolejki
stack_t *stack_initialize(size_t cap)
{
    stack_t *stack = (stack_t *)malloc(sizeof(stack_t));
    stack->cap = cap;
    stack->front = -1;
    stack->array = (PhoneForward **)calloc(cap, sizeof(PhoneForward *));
    if (stack->array == NULL)
    {
        return NULL;
    }
    return stack;
}

// zwalnianie pamięci kolejki
void stack_destroy(stack_t *stack)
{
    PhoneForward *temp;
    while (!is_empty(stack))
    {
        temp = pop(stack);
        if (temp->redirect)
        {
            free(temp->redirect);
        }
        free(temp);
    }
    free(stack->array);
    free(stack);
}

// zwiększanie rozmiaru kolejki
stack_t *stack_resize(stack_t *stack)
{
    PhoneForward **narr = (PhoneForward **)realloc(stack->array, (stack->cap + 2) * sizeof(PhoneForward *));
    if (narr == NULL)
    {
        stack_destroy(stack);
        return NULL;
    }
    stack->cap += 2;
    stack->array = narr;
    return stack;
}

// dodawanie elementu do kolejki
stack_t *add(stack_t *stack, PhoneForward *item)
{
    // jeśli kolejka jest pełna to próbujemy ją zwiększyć
    if (is_full(stack))
    {
        stack = stack_resize(stack);
        if (!stack)
        {
            return NULL;
        }
    }
    // dodawanie elementu na koniec kolejki i ustawianie wskaźnika na ostatni element w kolejce
    // stack->front += 1;
    stack->array[++stack->front] = item;
    return stack;
}

// usuwanie elementu z kolejki
PhoneForward *pop(stack_t *stack)
{
    if (is_empty(stack))
    {
        return NULL;
    }
    // odczytywanie danych z początku kolejki i przesuwanie wskaźnika na początek kolejki do tyłu
    PhoneForward *res = (stack->array[stack->front--]);
    return res;
}