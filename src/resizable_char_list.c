/** @file
 * Implementacja zbioru funkcji służących do obsługi powiększalniej 
 * listy znaków
 *
 * @author Wojciech Noskowiak <wn417909@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"

/**
 * @brief Funkcja określająca czy lista @p string jest pełna
 * 
 * @param string Badana lista 
 * @return true jeśli @p string jest pełna
 * @return false jeśli @p string nie jest pełna
 */
static inline bool string_is_full(resizable_string_t *string) {
      return (((size_t)string->last_filled == string->length));
}

resizable_string_t *string_initalize() {
      resizable_string_t *result =
          (resizable_string_t *)malloc(sizeof(resizable_string_t));
      if (!(result->char_list = (char *)malloc(3 * sizeof(char)))) {
            return NULL;
      }
      result->last_filled = 0;
      result->length = 3;
      return result;
}

/**
 * @brief Funkcja niszcząca listę @p string
 * 
 * @param string Lista do zniszczenia 
 */
static inline void string_destroy(resizable_string_t *string) {
      free(string->char_list);
      free(string);
}

/**
 * @brief Funkcja pomocnicza powiększajaca listę @p string
 * 
 * @param string Lista do powiększenia
 * @return Powiększona lista @p string lub NULL jeśli nie udało się 
 * zaalokować pamięci
 */
static inline resizable_string_t *string_resize(resizable_string_t *string) {
      char *narr = (char *)realloc(string->char_list,
                                   (string->length + 2) * sizeof(char));
      if (narr == NULL) {
            string_destroy(string);
            return NULL;
      }
      string->length += 2;
      string->char_list = narr;
      return string;
}

resizable_string_t *string_add(resizable_string_t *string, char item) {
      if (string_is_full(string)) {
            if (!(string = string_resize(string))) {
                  return NULL;
            }
      }
      string->char_list[string->last_filled] = item;
      string->last_filled += 1;
      return string;
}

char *string_close(resizable_string_t *string) {
      char *result;
      if(!(result=(char *)malloc((string->last_filled + 1) * sizeof(char)))){
          string_destroy(string);
          return NULL;
      }
      for (size_t i = 0; i < string->last_filled; i++) {
            result[i] = string->char_list[string->last_filled - i - 1];
      }
      result[string->last_filled] = '\0';
      string_destroy(string);
      return result;
}