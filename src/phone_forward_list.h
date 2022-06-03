/** @file
 * Zbiór funkcji służących do obsługi listy wskaźników na phone forward
 *
 * @author Wojciech Noskowiak <wn417909@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#ifndef PHONE_FORWARD_LIST_H_
#define PHONE_FORWARD_LIST_H_

#include <stdbool.h>
#include "types.h"

/**
 * @brief Funkcja określająca czy podana lista wskaźników na elementy drzewa jest pusta
 * Funkcja określa czy @p list zawiera w sobie jakiekolwiek elementy
 * 
 * @param list badana lista wskaźników na elementy drzewa
 * @return @p true jeśli lista jest pusta
 * @return @p false jeśli lista zawiera w sobie jakiekowliek elementy
 */
bool list_empty(PhoneForwardList_t *list);

/**
 * @brief Funkcja określająca czy podana lista wskaźników na elementy drzewa jest pełna
 * Funckja określająca czy @p list jest w pełni wypełniona
 * 
 * @param list lista badana lista wskaźników na elementy drzewa
 * @return @p true jeśli lista jest pełna
 * @return @p false jeśli lista nie jest pełna
 */
bool list_full(PhoneForwardList_t *list);

/**
 * @brief Funkcja inicjalizująca listę wskaźników na elementy drzewa
 * Funkcja inicjalizuje listę wskaźników na elementy drzewa o początkowej objętości @p cap
 * @param cap początkowa objętość listy
 * @return Stworzona lista lub NULL jeśli nie udalo się zaalokować pamięci
 */
PhoneForwardList_t *phoneForwardList_initialize(size_t cap);

/**
 * @brief funckcja zwalniająca pamięć listy @p list
 * @param list lista do zniszczenia
 */
void list_destroy(PhoneForwardList_t *list);

/**
 * @brief Funkcja dodająca do listy @p list element @p item
 * @param list Lista do której zostanie dodany element
 * @param item element który zostanie dodany do listy
 * @return Lista @p list rozszerzona o element @p item lub NULL jeśli nie udało się zaalokowac pamięci
 */
PhoneForwardList_t *list_add(PhoneForwardList_t *list, PhoneForward *item);

/**
 * @brief Funkcja usuwająca element @p item z listy @p list, o ile @p item jest elementem @p list
 * @param list Lista do której zostanie dodany element  
 * @param item Element który zostanie dodany do listy
 * @return Lista @p list na pewno nie zawierająca @p item   
 */
PhoneForwardList_t *list_remove(PhoneForwardList_t *list, PhoneForward *item);

/**
 * @brief Funkcja zwalniająca pamięć listę wskaźników na elementy drzewa
 * 
 * @param list Lista do usunięcia
 */
void free_PhoneForwardList(PhoneForwardList_t *list);

#endif