/** @file
 * Implementacja klasy przechowującej przekierowania numerów telefonicznych
 * wykorzystujący drzewo trie
 *
 * @author Wojciech Noskowiak <wn417909@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "alphabet_check.h"
#include "phone_forward_list.h"
#include "phone_numbers_manager.h"
#include "resizable_char_list.h"
#include "stack.h"
#include "trie_functions.h"
#include "types.h"

PhoneNumbers *phfwdReverse(PhoneForward const *pf, char const *num) {
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
      for (size_t i = 0; i < length1; i++) {
            if (num[i] == '\n') {
                  break;
            }
            if (!(temp = temp->further[charToNum(num[i])])) {
                  break;
            }
            if (temp->redirects) {
                  if (!(result =
                            addRedirects(temp->redirects, result, &num[i + 1],
                                         (length1 - i - 1)))) {
                        return NULL;
                  }
            }
      }
      removeRepeats(result);
      return result;
}

PhoneForward *phfwdNew() { return createNode(); }

void phfwdDelete(PhoneForward *pf) { deleteBranch(pf); }

bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2) {
      // jeśli którykowiek z inputów = NULL zwracamy false
      if (!(num1 && num2 && pf)) {
            return false;
      }
      size_t length1 = 0, length2 = 0;
      // weryfikujemy czy podane napisy reprezentują numery telefonów
      if (!(numbersOk(&length1, &length2, num1, num2) && length1 > 0 &&
            length2 > 0)) {
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
            return false;
      }
      // jeśli udało się je stworzyć dopisujemy do niego
      // przekierowanie
      removeForward(pf, properPlace);
      properPlace->redirect = (char *)malloc((length2 + 1) * sizeof(char));
      if (!properPlace->redirect) {
            phfwdDelete(firstCreated1);
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
            return false;
      }
      if (!(redirectPlace->redirects =
                list_add(redirectPlace->redirects, properPlace))) {
            free(properPlace->redirect);
            properPlace->redirect = NULL;
            phfwdDelete(firstCreated1);
            phfwdDelete(firstCreated2);
            return false;
      }
      return true;
}

void phfwdRemove(PhoneForward *pf, char const *num) {
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