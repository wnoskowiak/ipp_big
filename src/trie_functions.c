/** @file
 * Implementacja funkcji do edycji, odczytu danych i obsługi drzewa trie
 * @author Wojciech Noskowiak <wn417909@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#include "trie_functions.h"

#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "alphabet_check.h"
#include "phone_forward_list.h"
#include "resizable_char_list.h"
#include "stack.h"
#include "types.h"

void deleteBranch(PhoneForward *pf) {
      /*
       * Jeśli uda się zainicjalizować stos usuwanie przeprowadzane jest dfs-em
       * w przeciwnym wypadku struktura usuwana jest wolniejszą funkcją która
       * nie może się nie wykonać
       */
      // jeśli pf = NULL nic się nie dzieje
      if (!pf) {
            return;
      }
      // próbujemy zainicjalizować kolejkę
      stack_t *stack = stack_initialize(12);
      // jeśli się nie uda wywoływana jest bezpieczna funkcja usuwania
      if (!stack) {
            safeDelete(pf);
            return;
      }
      PhoneForward *temp = NULL;
      // dodajemy pf do stosu
      add(stack, pf);
      while (!is_empty(stack)) {
            //ściągamy pierwszy element ze stosu
            temp = pop(stack);
            // dodajemy na stos wszystkie elementy na które on wskazuje
            for (size_t i = 0; i < 12; i++) {
                  if (temp->further[i]) {
                        if (!add(stack, temp->further[i])) {
                              // jeśli nie uda się dodać elementu do kolejki
                              // jest on usuwany
                              //  bezpiecznym algorytmem
                              safeDelete(temp->further[i]);
                        }
                  }
            }
            // usuwamy sciągnięty element
            deleteLeaf(temp);
      }
      // niszczymy stos
      stack_destroy(stack);
}

/**
 * @brief Funkcja pomocnicza znajdująca element @p seek w tablicy następstw 
 * elementu @p pf . Jeśli @p seek nie występuje w następstwach @p pf funkcja zwraca -1.
 * 
 * @param pf element w którego następstwa zostaną przeszukane.
 * @param seek element szukany w następstwach.
 * @return indeks @p seek w następstwach @p pf lub -1 gdy go nie znaleziono.
 */
static inline int findInFurther(PhoneForward *pf, PhoneForward *seek) {
      if (!pf || !seek) {
            return -1;
      }
      int result = -1;
      for (int i = 0; i < 12; i++) {
            if (pf->further[i] == seek) {
                  result = i;
            }
      }
      return result;
}

char *treverseUp(PhoneForward *pf, size_t *length) {
      if (!pf) {
            NULL;
      }
      *length = 0;
      resizable_string_t *result;
      if(!(result = string_initalize())){
            return NULL;
      }
      PhoneForward *parent = pf->parent;
      PhoneForward *current = pf;
      while (parent) {
            if(
            !(result =
                string_add(result, numToChar(findInFurther(parent, current))))){
                      return NULL;
                }
            current = parent;
            parent = parent->parent;
            *length += 1;
      }
      return string_close(result);
}

PhoneForward *createNode(void) {
      // próbujemy zaalokować element zwracany przez funkcję
      PhoneForward *result = (PhoneForward *)malloc(sizeof(PhoneForward));
      if (!result) {
            return NULL;
      }
      // jeśli się uda inicjalizujemy go z wartościami NULL
      for (size_t i = 0; i < 12; i++) {
            result->further[i] = NULL;
      }
      result->redirects = NULL;
      result->redirect = NULL;
      result->parent = NULL;
      return result;
}

bool isLeaf(PhoneForward *pf, size_t *leftmostIndex, size_t ignore) {
      // inicjalizujemy zwracany bool jako false
      bool result = true;
      for (size_t i = 0; i < 12; i++) {
            if ((i != ignore) && (pf->further[i])) {
                  // jeśli którykolwiek indeks nie jest pusty ustwaiwamy result
                  // na true i leftmostIndex na odpowiedni indeks
                  result = false;
                  if (leftmostIndex) {
                        *leftmostIndex = i;
                  }
                  break;
            }
      }
      return result;
}

PhoneForward *getLeftmostLeaf(PhoneForward *pf, PhoneForward **parent,
                              size_t *leftmostIndex) {
      // zaczynamy od podanego korzenia
      PhoneForward *current = pf;
      while (true) {
            // jeśli podany węzeł jest liściem przerywamy pętle
            if (isLeaf(current, leftmostIndex, 12)) {
                  break;
            }
            // jeśli przechodzimy do jego najbardziej lewego syna i szukamy
            // dalej
            if (parent) {
                  *parent = current;
            }
            current = current->further[*leftmostIndex];
      }
      return current;
}

void deleteLeaf(PhoneForward *pf) {
      if (pf->redirect) {
            free(pf->redirect);
      }
      if (pf->redirects) {
            free_PhoneForwardList(pf->redirects);
      }
      free(pf);
}

void safeDelete(PhoneForward *pf) {
      PhoneForward *current = pf;
      PhoneForward *parent = pf;
      size_t leftmostIndex;
      // jeśli Parent != NULL to usuneliśmy juz wszystko
      while (parent) {
            parent = NULL;
            // szukamy najdajbardziej lewy liść drzewa
            current = getLeftmostLeaf(pf, &parent, &leftmostIndex);
            if (parent) {
                  parent->further[leftmostIndex] = NULL;
            }
            // i go usuwamy
            deleteLeaf(current);
      }
}

/**
 * @brief Funkcja pomocnicza znajdująca najbliższy w prawo niepusty element tablicy następstw @p pf
 * od @p last
 * 
 * @param pf Element którego tablica następst zostanie przeszukana 
 * @param last Element od którego zostanie rozpoczęte przeszukiwanie
 * @return Wskaźnik na najbliższy w prawo niepusty element tablicy następst @p pf lub NULL
 * jeśli takiego nie ma.
 */
static inline PhoneForward *getNextToRight(PhoneForward *pf, PhoneForward *last) {
      if (!pf) {
            return NULL;
      }
      PhoneForward *result = NULL;
      for (int i = 11; i >= 0; i--) {
            if (pf->further[i] == last) {
                  break;
            }
            if (pf->further[i]) {
                  result = pf->further[i];
            }
      }
      return result;
}

void removeForward(PhoneForward *root, PhoneForward *pf) {
      if (!pf->redirect) {
            return;
      }
      PhoneForward *redirect_destination = getElement(root, pf->redirect);
      redirect_destination->redirects =
          list_remove(redirect_destination->redirects, pf);
      cutHighestUseless(redirect_destination);
      free(pf->redirect);
      pf->redirect = NULL;
}

void buggie(PhoneForward *root, PhoneForward *pf) {
      size_t dump;
      PhoneForward *temp = getLeftmostLeaf(pf, NULL, &dump), *parent, *next;
      bool deleted;
      if (temp == pf) {
            return;
      }
      while (true) {
            removeForward(root, temp);
            parent = temp->parent;
            deleted = false;
            if (isLeaf(temp, NULL, 13) && !temp->redirects) {
                  deleteLeaf(temp);
                  deleted = true;
            }
            next = getNextToRight(parent, temp);
            if (deleted) {
                  parent->further[findInFurther(parent, temp)] = NULL;
            }
            if (parent == pf && !next) {
                  break;
            }
            if (next) {
                  temp = getLeftmostLeaf(next, NULL, &dump);
            } else {
                  temp = parent;
            }
      }
}

void cutHighestUseless(PhoneForward *pf) {
      if (!pf) {
            return;
      }
      PhoneForward *current = pf, *parent = pf->parent;
      bool empty;
      while (parent) {
            empty = isLeaf(current, NULL, 13);
            if (empty && !current->redirect && !current->redirects) {
                  deleteLeaf(current);
                  parent->further[findInFurther(parent, current)] = NULL;
                  current = parent;
                  parent = current->parent;
            } else {
                  break;
            }
      }
}

PhoneForward *getElement(PhoneForward *pf, char const *number) {
      size_t index;
      bool endOfWord;
      PhoneForward *result = pf;
      for (size_t i = 0; i < ULONG_MAX; i++) {
            alphabethOk(number[i], &endOfWord, &index);
            if (endOfWord) {
                  break;
            }
            if (!result->further[index]) {
                  return NULL;
            }
            result = result->further[index];
      }
      return result;
}

PhoneForward *createBranch(PhoneForward *pf, char const *number, size_t length,
                           PhoneForward **firstCreated, bool *memoryFailure) {
      *memoryFailure = false;
      *firstCreated = NULL;
      PhoneForward *properPlace = pf;
      PhoneForward *current = NULL;
      size_t index;
      // przechodzimy przez całe słowo
      for (size_t i = 0; i < length; i++) {
            alphabethOk(number[i], NULL, &index);
            // sprawdzamy czy zadany węzeł już istnieje
            if ((current = properPlace->further[index])) {
                  // jeśli tak przechodzimy do niego
                  properPlace = current;
            } else {
                  // jeśli nie to tworzymy węzeł i wstawiamy go w odpowiednie
                  // miejsce
                  if (!(current = createNode())) {
                        *memoryFailure = true;
                        break;
                  }
                  if (!firstCreated) {
                        // zapisujemy jeśli to pierwszy stworzony węzeł w pętli
                        *firstCreated = current;
                  }
                  properPlace->further[index] = current;
                  current->parent = properPlace;
                  properPlace = current;
            }
      }
      return properPlace;
}

PhoneForward *getDeepestEmpty(PhoneForward *pf, char const *num, bool *alright,
                              size_t *deepestIndex) {
      *alright = true;
      bool tmp, empty = true;
      PhoneForward *properPlace = pf;
      PhoneForward *deepestEmpty = NULL;
      PhoneForward *temp = NULL;
      size_t number;
      for (size_t i = 0; i < ULONG_MAX; i++) {
            // sprawdzamy czy znak jest poprawny
            *alright = alphabethOk(num[i], &tmp, &number);
            if (!(*alright) || tmp) {
                  break;
            }
            temp = properPlace->further[number];
            // jeśli nie ma dalszego syna w odpowiednim miejscu to przerywamy
            // działanie pętli
            if (!temp) {
                  *alright = false;
                  break;
            }
            // jeśli jest to sprawdzamy czy (z wyłączeniem indeksu number) jest
            // on liściem
            empty = isLeaf(properPlace, NULL, number);
            if (!empty || properPlace->redirect || !deepestEmpty ||
                properPlace->redirects) {
                  // jeśłi nie jest, nie zawiera przekierwania oraz deepest
                  // empty nie jest puste to ustawiamy go jako deepestEmpty
                  deepestEmpty = properPlace;
                  *deepestIndex = number;
            }
            properPlace = temp;
      }
      return deepestEmpty;
}

phfwdGet_helper_t getDeepestRedirect(size_t *final, bool *reachedEnd,
                                     PhoneForward const *pf, char const *num) {
      bool foundDeepest = false;
      PhoneForward const *properPlace = pf, *temp = NULL;
      phfwdGet_helper_t deepestRedirect;
      deepestRedirect.depth = 0;
      deepestRedirect.prefix = NULL;
      size_t index;
      for (size_t i = 0; i < ULONG_MAX; i++) {
            // weryfikujemy poprawność znaków
            if (!alphabethOk(num[i], reachedEnd, &index)) {
                  break;
            }
            if (!foundDeepest) {
                  // jeśli nie datarliśmy do końca napisu to
                  // sprawdzamy czy napotkaliśmy przekierowanie
                  if (properPlace->redirect) {
                        deepestRedirect.depth = i;
                        deepestRedirect.prefix = properPlace->redirect;
                  }
                  if (!(*reachedEnd)) {
                        if ((temp = properPlace->further[index])) {
                              properPlace = temp;
                        } else {
                              foundDeepest = true;
                        }
                  }
            }
            // jeśli dotarliśmy do kończa napisu to przerywamy pętle
            if (*(reachedEnd)) {
                  *final = i;
                  break;
            }
      }
      return deepestRedirect;
}