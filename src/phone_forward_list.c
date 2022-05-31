/** @file
 * Zbiór funkcji służących do obsługi listy wskaźników na phone forward
 *
 * @author Wojciech Noskowiak <wn417909@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include "types.h"

bool list_empty(PhoneForwardList_t *list) { return (list->last_index == 0); }

bool list_full(PhoneForwardList_t *list) {
      return (((size_t)list->last_index == list->len ));
}

PhoneForwardList_t *phoneForwardList_initialize(size_t cap) {
      // próbujemy zaalokować listę
      PhoneForwardList_t *result =
          (PhoneForwardList_t *)malloc(sizeof(PhoneForwardList_t));
      if (!result) {
            return NULL;
      }
      // inicjalizujemy zmienne
      result->len = cap;
      result->last_index = 0;
      // próbujemy zaalokować właściwią listę wskaźników
      result->list = (PhoneForward **)calloc(cap, sizeof(PhoneForward *));
      if (result->list == NULL) {
            // jeśli sie nie uda czyścimy pamięć i zwracamy NULL
            free(result);
            return NULL;
      }
      return result;
}

void list_destroy(PhoneForwardList_t *list) {
      free(list->list);
      free(list);
}

static PhoneForwardList_t *list_resize(PhoneForwardList_t *stack) {
      // próbujemy zaalokować większą tablicę i przepisać do niej starą
      PhoneForward **narr = (PhoneForward **)realloc(
          stack->list, (stack->len + 2) * sizeof(PhoneForward *));
      if (narr == NULL) {
            // jeśli się nie uda zwracamy NULL
            return NULL;
      }
      // jeśli się uda zwiększamy nominalny rozmiar lis
      stack->len += 2;
      stack->list = narr;
      return stack;
}

int index_in_list(PhoneForwardList_t *list, PhoneForward *elem) {
      for (size_t i = 0; i < list->last_index; i++) {
            if (list->list[i] == elem) {
                  return i;
            }
      }
      return -1;
}

PhoneForwardList_t *list_remove(PhoneForwardList_t *list, PhoneForward *item) {
      if (!list) {
            return NULL;
      }
      int index = index_in_list(list, item);
      if (index == -1) {
            return list;
      }
      list->list[index] = list->list[list->last_index-1];
      list->last_index -= 1;
      if(list->last_index == 0){
          list_destroy(list);
          list = NULL;
      }
      return list;
}

PhoneForwardList_t *list_add(PhoneForwardList_t *list, PhoneForward *item) {
      if (!list) {
            list = phoneForwardList_initialize(1);
      }
      // jeśli stos jest pełny to próbujemy go zwiększyć
      if (list_full(list)) {
            list = list_resize(list);
            if (!list) {
                  // jeśli się nie uda zwracamy NULL
                  return NULL;
            }
      }
      list_remove(list,item);
      // dopisujemy element na początek stosu i przesuwamy wskaźnik
      list->list[list->last_index] = item;
      list->last_index += 1;
      return list;
}