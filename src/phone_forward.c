/** @file
 * Implementacja klasy przechowującej przekierowania numerów telefonicznych
 * wykorzystujący drzewo trie
 *
 * @author Wojciech Noskowiak <wn417909@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#include <stdbool.h>
#include <string.h>

#include "alphabet_check.h"
#include "stack.h"
#include "trie_functions.h"
#include "types.h"

PhoneNumbers *phfwdReverse(PhoneForward const *pf, char const *num) {
      // ta funkcja nie robi nic, poniższe operacje mają powstrzymywać
      // kompilator od wyrzucania błędów
      pf = pf;
      num = num;
      return NULL;
}

PhoneForward *phfwdNew() { return createNode(); }

void phfwdDelete(PhoneForward *pf) {
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
      stack_t *stack = stack_initialize(10);
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
            for (size_t i = 0; i < 10; i++) {
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

bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2) {
      // jeśli którykowiek z inputów = NULL zwracamy false
      if (!(num1 && num2 && pf)) {
            return false;
      }
      size_t length1 = 0, lenght2 = 0;
      // weryfikujemy czy podane napisy reprezentują numery telefonów
      if (numbersOk(&length1, &lenght2, num1, num2) && length1 > 0 &&
          lenght2 > 0) {
            bool memoryFailure;
            PhoneForward *firstCreated;
            // próbujemy stworzyć węzeł w odpowiednim miejscu
            PhoneForward *properPlace =
                createBranch(pf, num1, length1, &firstCreated, &memoryFailure);
            if (!memoryFailure) {
                  // jeśli udało się je stworzyć dopisujemy do niego
                  // przekierowanie
                  if (properPlace->redirect) {
                        free(properPlace->redirect);
                  }
                  properPlace->redirect =
                      (char *)malloc((lenght2 + 1) * sizeof(char));
                  if (properPlace->redirect) {
                        strcpy(properPlace->redirect, num2);
                        properPlace->redirect[lenght2] = '\0';
                        return true;
                  }
            }
            // jeśli program dotarł do tej linijki ro znaczy że
            // wysytąpił problem z alokacją pamięci
            phfwdDelete(firstCreated);
      }
      return false;
}

void phfwdRemove(PhoneForward *pf, char const *num) {
      // jeśli którykowiek z inputów = NULL nic się nie dzieje
      if (!(pf && num)) {
            return;
      }
      bool alright;
      size_t deepestIndex;
      // próbujemy znaleźć najpłytszy węzeł znajdujący się na ścieżce określonej
      // przez
      //  num
      PhoneForward *deepestEmpty =
          getDeepestEmpty(pf, num, &alright, &deepestIndex);
      if (alright && deepestEmpty) {
            // jeśłi taki znajdzie to usuwa poddrzewo którego jest ono korzeniem
            phfwdDelete(deepestEmpty->further[deepestIndex]);
            deepestEmpty->further[deepestIndex] = NULL;
      }
}

/** @brief Pomocnicza funkcja łącząca dwa numery telefonu w jeden.
 * Pomocnicza fubkcja konkatynująca @p num1 i @p num2 w sposób bezpieczny. jeśli
 * nie uda się zaalokować pamięci funkcja zwraca NULL. Funkcja nie weryfikuje
 * poprawnośći numerów. @p num1length i
 * @p num2length określają długość konkatynowamych napisów
 * @param[in] num1 – wskaźnik na napis reprezentujący pierwszy numer.
 * @param[in] num2 – wskaźnik na napis reprezentujący drugi numer.
 * @param[in] num1length – długość pierwszego numeru.
 * @param[in] num2length – długość drugiego numeru.
 * @return wskaźnik na zkonkatynowany napis
 */
static inline char *joinNumbers(const char *num1, const char *num2,
                                size_t num1length, size_t num2length) {
      size_t newLength = num1length + num2length + 1;
      // próbujemy zaalokować odpowiednią ilość pamięci
      char *newNumber = (char *)malloc((newLength) * sizeof(char));
      if (!newNumber) {
            return NULL;
      }
      // jeśli się uda to zapisujemy do niej oba napisy
      strcpy(newNumber, num1);
      if (num2) {
            strcat(newNumber, num2);
      }
      newNumber[newLength - 1] = '\0';
      return newNumber;
}

PhoneNumbers *phfwdGet(PhoneForward const *pf, char const *num) {
      // jeśli pf == NULL zwracamy NULL
      if (!pf) {
            return NULL;
      }
      // inicjalizujemy wartość którą zwróci funkcja
      PhoneNumbers *result = (PhoneNumbers *)malloc(sizeof(PhoneNumbers));
      if (!result) {
            return NULL;
      }
      result->list = NULL;
      result->len = 0;
      // jeśli num == NULL zwracamy pustą strukturę
      if (!num) {
            return result;
      }
      bool reachedEnd;
      size_t final;
      // znajdujemy najdłuższe pasujące przekierowanie
      phfwdGet_helper_t deepestRedirect =
          getDeepestRedirect(&final, &reachedEnd, pf, num);
      if (reachedEnd && final != 0) {
            // jeśli uda sie je znaleźć to określamy czy nie jest puste i
            // składamy numer po przekierowaniu
            char *newNumber = NULL;
            const char *num1;
            const char *num2;
            size_t num1length = 0, num2length = 0;
            // jeśli przekierowanie nie jest puste ustawiamy numery w
            // odpowiedniej kolejności
            if (deepestRedirect.prefix) {
                  num1length = strlen(deepestRedirect.prefix);
                  num2length = final - deepestRedirect.depth;
                  num1 = deepestRedirect.prefix;
                  num2 = &num[deepestRedirect.depth];
            }
            // jeśli nie próbujemy łączyć wejściowy numer z pustym wyrazem
            else {
                  num1 = num;
                  num2 = NULL;
                  num1length = final;
            }
            if (!(newNumber =
                      joinNumbers(num1, num2, num1length, num2length))) {
                  free(result);
                  return NULL;
            }
            if (!(result->list = (char **)malloc(sizeof(char *)))) {
                  free(result);
                  return NULL;
            }
            result->len = 1;
            result->list[0] = newNumber;
      }
      return result;
}

char const *phnumGet(PhoneNumbers const *pnum, size_t idx) {
      // jeśli indeks poza zakresem struktury lub pnum = NULL zwarcamy NULL
      if ((!pnum) || (pnum->len <= idx)) {
            return NULL;
      }
      // w przeciwnym wypadku zwracamy
      return pnum->list[idx];
}

void phnumDelete(PhoneNumbers *pnum) {
      // jeśli pnum == NULL nic się nie dzieje
      if (!pnum) {
            return;
      }
      if (pnum->list) {
            // usuwamy każdy element tablicy
            for (size_t i = 0; i < pnum->len; i++) {
                  free(pnum->list[i]);
            }
            free(pnum->list);
      }
      // oraz całą strukturę
      free(pnum);
}