/** @file
 * Funkcje do obsługi stosu
 * @author Wojciech Noskowiak <wn417909@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#ifndef STACK_H_
#define STACK_H_

#include "types.h"

/**
 * @brief Funkcja sprawdzająca czy stos jest pusty
 * Funkcja werifikuje czy stos na który wskazuje wskaźnik @p stack jest pusty
 * 
 * @param stack - wskaźnik na badany stos 
 * @return true kiedy stos jest pusty, true kiedy stos nie jest pusty
 */
bool is_empty(stack_t *stack);

/**
 * @brief Funkcja sprawdzająca czy stos jest pełny
 * Funkcja werifikuje czy stos na który wskazuje wskaźnik @p stack jest pełny
 * 
 * @param stack - wskaźnik na badany stos 
 * @return true kiedy stos jest pełny, true kiedy stos nie jest pełny
 */
bool is_full(stack_t *stack);

/**
 * @brief funkcja tworząca pusty stos
 * Funkcja tworzy stos o rozmiarze @p cap
 * @param cap - określa rozmiar tworzonego stosu
 * @return wskaźnik na nowo utworzony stos 
 */
stack_t *stack_initialize(size_t cap);

/**
 * @brief Funkcja usuwająca stos 
 * Funkcja usuwająca stos na który wskazuje @p stack
 * @param stack - wskaźnik na stos do usunięcia
 */
void stack_destroy(stack_t *stack);

/**
 * @brief Funkcja dodająca element do stosu
 * Funkcja dodająca element @p item do stosu na który wskazuje @p stack
 * @param stack - stos do którego dodany zostanie element
 * @param item - wskaźnik na element do dodania
 * @return wskaźnik na @p stack jeśli udało się zaalokować pamięć, w przeciwnym wypadku 
 * NULL
 */
stack_t *add(stack_t *stack, PhoneForward *item);

/**
 * @brief Funkcja wyciągająca element ze stosu
 * Funkcja wyciągająca ostatnio dodany element ze stosu na który wskazuje @p stack
 * @param stack - wskaźnik na stos z którego zostanie wyciągnięty element
 * @return wyciągnięty element ze sotsu
 */
PhoneForward *pop(stack_t *stack);

#endif