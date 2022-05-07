/** @file
 * implementacja funkcji do obsługi stosu
 * @author Wojciech Noskowiak <wn417909@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#include <stdbool.h>
#include <stdlib.h>
#include "types.h"
#include "stack.h"
#include "trie_functions.h"

bool is_empty(stack_t *stack) { return (stack->front == -1); }

bool is_full(stack_t *stack) {
      return (((size_t)stack->front == stack->cap - 1));
}

stack_t *stack_initialize(size_t cap) {
      //próbujemy zaalokować stos
      stack_t *stack = (stack_t *)malloc(sizeof(stack_t));
      if(!stack){
            return NULL;
      }
      //inicjalizujemy zmienne
      stack->cap = cap;
      stack->front = -1;
      //próbujemy zaalokować tablicę przechowującą elementy stosu
      stack->array = (PhoneForward **)calloc(cap, sizeof(PhoneForward *));
      if (stack->array == NULL) {
            //jeśli sie nie uda czyścimy pamięć i zwracamy NULL
            free(stack);
            return NULL;
      }
      return stack;
}

void stack_destroy(stack_t *stack) {
      PhoneForward *temp;
      //niszczymy każdy element stosu
      while (!is_empty(stack)) {
            temp = pop(stack);
            deleteLeaf(temp);
      }
      // po czym usuwamy cały stos
      free(stack->array);
      free(stack);
}

/**
 * @brief Funkcja zwiększająca rozmiar stosu
 * Funkcja próbuje zwiększyć rozmiar stosu na który wskazuje @p stack
 * @param stack - wskaźnik na stos do powiększenia
 * @return wskaźnik na @p stack jeśli uda sie powiększyć stos. jeśli nie NULL
 */
static stack_t *stack_resize(stack_t *stack) {
      //próbujemy zaalokować większą tablicę i przepisać do niej starą
      PhoneForward **narr = (PhoneForward **)realloc(
          stack->array, (stack->cap + 2) * sizeof(PhoneForward *));
      if (narr == NULL) {
            //jeśli się nie uda zwracamy NULL
            return NULL;
      }
      //jeśli się uda zwiększamy cap kolejki
      stack->cap += 2;
      stack->array = narr;
      return stack;
}

stack_t *add(stack_t *stack, PhoneForward *item) {
      // jeśli stos jest pełny to próbujemy go zwiększyć
      if (is_full(stack)) {
            stack = stack_resize(stack);
            if (!stack) {
                  //jeśli się nie uda zwracamy NULL
                  return NULL;
            }
      }
      //dopisujemy element na początek stosu i przesuwamy wskaźnik
      stack->array[++stack->front] = item;
      return stack;
}

PhoneForward *pop(stack_t *stack) {
      // prawdzamy czy stos nie jest pusty
      if (is_empty(stack)) {
            return NULL;
      }
      // odczytujemy dane z początku stosu i przesuwamy zmienną wskazująca na początek
      //stosu
      PhoneForward *res = (stack->array[stack->front--]);
      return res;
}