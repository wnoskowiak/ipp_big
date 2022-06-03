/** @file
 * Implementacja zbioru funkcji służących do obsługi listy wskaźników na phone forward
 *
 * @author Wojciech Noskowiak <wn417909@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#include <stdbool.h>
#include <stdlib.h>

#include "types.h"

bool list_empty(PhoneForwardList_t *list) { return (list->last_index == 0); }

bool list_full(PhoneForwardList_t *list) {
      return (((size_t)list->last_index == list->len));
}

void free_PhoneForwardList(PhoneForwardList_t *list) {
      if (!list) {
            return;
      }
      if (list->list) {
            free(list->list);
      }
      free(list);
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

/**
 * @brief Funkcaja pomocnicza powiększająca listę @p list 
 * 
 * @param list Lista do powiększenia 
 * @return Powiększona lista @p list lub NULL jeśli nie udało się zaalokowiać pamięci
 */
static inline PhoneForwardList_t *list_resize(PhoneForwardList_t *list) {
      // próbujemy zaalokować większą tablicę i przepisać do niej starą
      PhoneForward **narr = (PhoneForward **)realloc(
          list->list, (list->len + 2) * sizeof(PhoneForward *));
      if (narr == NULL) {
            // jeśli się nie uda zwracamy NULL
            list_destroy(list);
            return NULL;
      }
      // jeśli się uda zwiększamy nominalny rozmiar lis
      list->len += 2;
      list->list = narr;
      return list;
}

/**
 * @brief Funkcja określa na którym miejscu w @p list znajduje sie @p elem.
 * jeśli @p elem nie znajduje się w @p list funkcja zwraca -1.
 * 
 * @param list List w której wyszukany zostanie element @p elem
 * @param elem Element który zostanie wyszukane w @p list
 * @return indeks @p elem w @p list lub -1 jeśli @p elem nie zawiera się w  @p list
 */
static inline int index_in_list(PhoneForwardList_t *list, PhoneForward *elem) {
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
      list->list[index] = list->list[list->last_index - 1];
      list->last_index -= 1;
      if (list_empty(list)) {
            list_destroy(list);
            list = NULL;
      }
      return list;
}

PhoneForwardList_t *list_add(PhoneForwardList_t *list, PhoneForward *item) {
      if (!list) {
            list = phoneForwardList_initialize(1);
            if(!list){
                  return NULL;
            }
      }
      // jeśli stos jest pełny to próbujemy go zwiększyć
      if (list_full(list)) {
            list = list_resize(list);
            if (!list) {
                  // jeśli się nie uda zwracamy NULL
                  return NULL;
            }
      }
      //check if in list
      if((index_in_list(list,item))>-1){
            return list;
      }
      // dopisujemy element na początek stosu i przesuwamy wskaźnik
      list->list[list->last_index] = item;
      list->last_index += 1;
      return list;
}