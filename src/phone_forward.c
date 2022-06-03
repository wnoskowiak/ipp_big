/** @file
 * Implementacja klasy przechowującej przekierowania numerów telefonicznych
 * wykorzystujący drzewo trie
 *
 * @author Wojciech Noskowiak <wn417909@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alphabet_check.h"
#include "phone_forward_list.h"
#include "phone_numbers_manager.h"
#include "resizable_char_list.h"
#include "stack.h"
#include "trie_functions.h"
#include "types.h"
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

int numCompare (const char* str1, const char* str2){
      int ch1, ch2;
      size_t i=0;
      while(true){
            if(str1[i] == '\0'){
                  return -1;
            }
            if(str2[i] == '\0'){
                  return 1;
            }
            ch1 = charToNum(str1[i]);
            ch2 = charToNum(str2[i]);
            if(ch1 != ch2){
                  return (((int)(ch1>ch2))*2)-1;
            }
            i++;
      }
}

int pstrcmp(const void *a, const void *b) {
      return numCompare(*(char **)a, *(char **)b);
}

PhoneNumbers *phfwdReverse(PhoneForward const *pf, char const *num) {
      // printf("reverse\n");
      if (!pf) {
            return NULL;
      }
      PhoneNumbers *result;
      if (!(result = pnum_initalize())) {
            return NULL;
      }
      size_t length1 = 0, dump;
      if (!pf || !num) {
            return result;
      }
      if (!numbersOk(&length1, &dump, num, NULL)) {
            return result;
      }
      if (length1 == 0) {
            return result;
      }
      if (!pnum_add(result, joinNumbers(num, NULL, length1, 0))) {
            return NULL;
      }
      PhoneForward const *temp = pf;
      size_t len;
      char *prefix;
      for (size_t i = 0; i < length1; i++) {
            if (num[i] == '\n') {
                  break;
            }
            temp = temp->further[charToNum(num[i])];
            if (!temp) {
                  break;
            }
            if (temp->redirects) {
                  for (size_t j = 0; j < temp->redirects->last_index; j++) {
                        if (!(prefix =
                                  treverseUp(temp->redirects->list[j], &len))) {
                              pnum_destroy(result);
                              return NULL;
                        }
                        if (!(result = pnum_add(
                                  result, joinNumbers(prefix, &num[i + 1],
                                                      strlen(prefix),
                                                      (length1 - i - 1))))) {
                              free(prefix);
                              return NULL;
                        }
                        free(prefix);
                  }
            }
      }
      if (result->len > 1) {
            qsort(result->list, result->len, sizeof(char *), pstrcmp);
      }
      // remove repeats
      size_t removed = 0;
      for (size_t j = 1; j < result->len - removed; j++) {
            if (strcmp(result->list[j - 1], result->list[j]) == 0) {
                  free(result->list[j]);
                  result->list[j] = NULL;
                  for (size_t k = j; k < result->len - removed - 1; k++) {
                        result->list[k] = result->list[k + 1];
                  }
                  removed++;
                  j--;
            }
      }
      result->len -= removed;
      return result;
}

PhoneForward *phfwdNew() { return createNode(); }

void phfwdDelete(PhoneForward *pf) {
      deleteBranch(pf);
}

bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2) {
      // printf("add\n");
      // printf("add start\n");
      // jeśli którykowiek z inputów = NULL zwracamy false
      if (!(num1 && num2 && pf)) {
            // printf("add end 1\n");
            return false;
      }
      size_t length1 = 0, length2 = 0;
      // weryfikujemy czy podane napisy reprezentują numery telefonów
      if (!(numbersOk(&length1, &length2, num1, num2) && length1 > 0 &&
            length2 > 0)) {
                  // printf("add end 2\n");
            return false;
      }
      
      bool memoryFailure;
      PhoneForward *firstCreated1, *firstCreated2;
      // próbujemy stworzyć węzeł w odpowiednim miejscu
      PhoneForward *properPlace =
          createBranch(pf, num1, length1, &firstCreated1, &memoryFailure);
      if (memoryFailure) {
            // jeśli program dotarł do tej linijki to znaczy że
            // wysytąpił problem z alokacją pamięci
            phfwdDelete(firstCreated1);
            // printf("add end 3\n");
            return false;
      }
      // jeśli udało się je stworzyć dopisujemy do niego
      // przekierowanie
      removeForward(pf, properPlace);
      properPlace->redirect = (char *)malloc((length2 + 1) * sizeof(char));
      if (!properPlace->redirect) {
            phfwdDelete(firstCreated1);
            // printf("add end 4\n");
            return false;
      }
      strcpy(properPlace->redirect, num2);
      properPlace->redirect[length2] = '\0';
      PhoneForward *redirectPlace =
          createBranch(pf, num2, length2, &firstCreated2, &memoryFailure);
      if (memoryFailure) {
            // jeśli program dotarł do tej linijki to znaczy że
            // wysytąpił problem z alokacją pamięci
            free(properPlace->redirect);
            properPlace->redirect = NULL;
            phfwdDelete(firstCreated2);
            phfwdDelete(firstCreated1);
            // printf("add end 5\n");
            return false;
      }
      // printf("breaker\n");
      if (!(redirectPlace->redirects =
                list_add(redirectPlace->redirects, properPlace))) {
                  //     printf("breaker\n");
            free(properPlace->redirect);
            properPlace->redirect = NULL;
            phfwdDelete(firstCreated1);
            phfwdDelete(firstCreated2);
            // printf("add end 6\n");
            return false;
      }
      // printf("add end 7\n");
      return true;
}

void phfwdRemove(PhoneForward *pf, char const *num) {
      // printf("remove\n");
      if (!(pf && num)) {
            return;
      }
      PhoneForward *redirect_place = getElement(pf, num);
      if (!redirect_place) {
            return;
      }
      removeForward(pf, redirect_place);
      buggie(pf, redirect_place);
      cutHighestUseless(redirect_place);
}

PhoneNumbers *phfwdGet(PhoneForward const *pf, char const *num) {
      // printf("get\n");
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

void phnumDelete(PhoneNumbers *pnum) { pnum_destroy(pnum); }