/** @file 
 * Implementacja funkcji kontrolujących listę numerów telefonu
 * 
 * @author Wojciech Noskowiak <wn417909@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "alphabet_check.h"
#include "trie_functions.h"

/**
 * @brief Funkcja określająca czy lista @p pnum jest pełna
 * 
 * @param pnum Badana lista
 * @return true jeśli @p pnum jest pełna
 * @return false jeśli @p pnum nie jest pełna
 */
static inline bool pnum_is_full(PhoneNumbers *pnum) {
      return (((size_t)pnum->max_size == pnum->len));
}

PhoneNumbers *pnum_initalize(){
    PhoneNumbers *result = (PhoneNumbers*)malloc(sizeof(PhoneNumbers));
    if(!(result->list = (char**)malloc(sizeof(char*)))){
        return NULL;
    }
    result->len = 0;
    result->max_size = 1;
    return result;
}

void pnum_destroy(PhoneNumbers *pnum){
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

/**
 * @brief Funkcja powiększająca listę @p pnum
 * 
 * @param pnum Lista do powiększenia
 * @return Powiększona lista @p pnum lub NULL jeśli nie udało sie 
 * zaalokować pamięci
 */
static inline PhoneNumbers *pnum_resize(PhoneNumbers *pnum) {
      char **narr = (char **)realloc(
          pnum->list, (pnum->max_size + 2) * sizeof(char *));
      if (narr == NULL) {

            return NULL;
      }
      pnum->max_size  += 2;
      pnum->list = narr;
      return pnum;
}

PhoneNumbers *pnum_add(PhoneNumbers * pnum, char *num){
    if(pnum_is_full(pnum)){
        if(!(pnum = pnum_resize(pnum))){
            pnum_destroy(pnum);
            return NULL;
        }
    }
    pnum->list[pnum->len] = num;
    pnum->len += 1;
    return pnum;
}

/**
 * @brief funkcja pomocnicza odpowiednio przemapowująca numery 
 * do porównania w taki sposób żeby mogła ona być wykorzystana przez 
 * qsort
 * 
 * 
 * @param[in] a pierwszy numer do porównania
 * @param[in] b drugi numer do porównania
 * @return wynik działania @ref numCompare na tych numerach
 */
static inline int cstrcmp(const void *a, const void *b) {
      return numCompare(*(char **)a, *(char **)b);
}

void removeRepeats(PhoneNumbers *result) {
      if (result->len > 1) {
            qsort(result->list, result->len, sizeof(char *), cstrcmp);
      }
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
}

PhoneNumbers *addRedirects(PhoneForwardList_t *redirects, PhoneNumbers *result,
                           char const *num, size_t length) {
      size_t len;
      char *prefix;
      for (size_t j = 0; j < redirects->last_index; j++) {
            if (!(prefix = treverseUp(redirects->list[j], &len))) {
                  pnum_destroy(result);
                  return NULL;
            }
            if (!(result = pnum_add(
                      result,
                      joinNumbers(prefix, num, strlen(prefix), length)))) {
                  free(prefix);
                  return NULL;
            }
            free(prefix);
      }
      return result;
}