/** @file
 * Zbiór funkcji służących do obsługi powiększalniej listy znaków
 *
 * @author Wojciech Noskowiak <wn417909@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#ifndef RESIZABLE_CHAR_LIST_
#define RESIZABLE_CHAR_LIST_

#include <stdbool.h>
#include <string.h>  
#include "types.h"

/**
 * @brief Funkcja dodająca znak @p item do listy @p string
 * 
 * @param string Lista do której zostanie dodany @p item
 * @param item Znak który zostanie do @p string
 * @return @p string z dodanym @p item lub NULL jeśli nie udało się zaalokować
 * pamięci
 */
resizable_string_t *string_add(resizable_string_t *string, char item);

/**
 * @brief Funkcja zwracająca odwróconą tablicę znaków z @p string zakończoną
 * '/0' i niszczy @p string
 * 
 * @param string Lista znaków która zostanie zamknięta
 * @return Odwrócona lista znaków z @p string
 */
char *string_close(resizable_string_t *string);

/**
 * @brief Funkcja inicjalizująca powiększalną listę znaków
 * 
 * @return Zainicjalizowana lista lub NULL jeśli nie udało się 
 * zaalokować pamięci
 */
resizable_string_t *string_initalize();

#endif