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

#include "alphabet_check.h"
#include "types.h"

PhoneForward *createNode(void) {
      // próbujemy zaalokować element zwracany przez funkcję
      PhoneForward *result = (PhoneForward *)malloc(sizeof(PhoneForward));
      if (!result) {
            return NULL;
      }
      // jeśli się uda inicjalizujemy go z wartościami NULL
      for (size_t i = 0; i < 10; i++) {
            result->further[i] = NULL;
      }
      result->redirect = NULL;
      result->parent = NULL;
      return result;
}

bool isLeaf(PhoneForward *pf, size_t *leftmostIndex, size_t ignore) {
      // inicjalizujemy zwracany bool jako false
      bool result = true;
      for (size_t i = 0; i < 10; i++) {
            if ((i != ignore) && (pf->further[i])) {
                  // jeśli któruykolwiek indeks nie jest pusty ustwaiwamy result
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
            if (isLeaf(current, leftmostIndex, 10)) {
                  break;
            }
            // jeśli przechodzimy do jego najbardziej lewego syna i szukamy
            // dalej
            *parent = current;
            current = current->further[*leftmostIndex];
      }
      return current;
}

void deleteLeaf(PhoneForward *pf) {
      if (pf->redirect) {
            free(pf->redirect);
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

PhoneForward *createBranch(PhoneForward *pf, char const *number, size_t length,
                           PhoneForward **firstCreated, bool *memoryFailure) {
      *memoryFailure = false;
      *firstCreated = NULL;
      PhoneForward *properPlace = pf;
      PhoneForward *current = NULL;
      // przechodzimy przez całe słowo
      for (size_t i = 0; i < length; i++) {
            // sprawdzamy czy zadany węzeł już istnieje
            if ((current = properPlace->further[(number[i] - '0')])) {
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
                  properPlace->further[(number[i] - '0')] = current;
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
            *alright = alphabethOk(num[i], &tmp);
            if (!(*alright) || tmp) {
                  break;
            }

            number = (num[i] - '0');
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
            if (!empty || properPlace->redirect || !deepestEmpty) {
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
      for (size_t i = 0; i < ULONG_MAX; i++) {
            // weryfikujemy poprawność znaków
            if (!alphabethOk(num[i], reachedEnd)) {
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
                        if ((temp = properPlace->further[(num[i] - '0')])) {
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