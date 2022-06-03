/** @file 
 * Inrerfejs funkcji kontrolujących listę numerów telefonu
 * 
 * @author Wojciech Noskowiak <wn417909@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#ifndef PHONE_NUMBERS_MANAGER_
#define PHONE_NUMBERS_MANAGER_

#include "types.h"

/**
 * @brief Funkcja inicjalizująca listę numerów telefonu
 * 
 * @return zainicjalizowana lista lub NULL jeśli nie udało się zainicjalizować pamięci
 */
PhoneNumbers *pnum_initalize();

/**
 * @brief Funkcja dodająca numer @p num do listy @p pnum 
 * 
 * @param pnum Lista do której zostanie dodany @p num 
 * @param num Numer który dostanie zostanie dodany do @p pnum 
 * @return @p pnum z dodanym @p num lub NULL jeśnie nie udało się zaalokować pamięci
 */
PhoneNumbers *pnum_add(PhoneNumbers * pnum, char *num);

/**
 * @brief Funkcja niszcząca listę @p pnum
 * 
 * @param pnum lista do zniszczenia
 */
void pnum_destroy(PhoneNumbers *pnum);

/**
 * @brief Funkcja usuwająca powtórzenia z posortowaniej listy numerów
 * Funkcja usuwa powtarzające się numery z podsortowanej listy @p result
 * @param result Posostowana lista numerów z której usunięte zostaną powtórzenia
 */
void removeRepeats(PhoneNumbers *result);

/**
 * @brief Funkcja dodająca odpowiednie numery do wyniku działania funkcji reverse
 * Funkcja dodaje do listy numerów @p result będącej wynikiem działania funkcji reverse przekierowania 
 * z sufiksem @p num z prefikami określonymi przez @p redirects. @p lenght określa długość prefixu @p num.
 * @param redirects lista określająca prefiksy przekierowywyjących numerów
 * @param result lista numerów będąca wynikiem działania funkcji reverse
 * @param num numer określający sufiks przekierowywujących numerów
 * @param length długość sufiksu @p num
 * @return Uzupełnioną odpowiednio listę @p results lub @p NULL gdy nie udało się zaalokować pamięci
 */
PhoneNumbers *addRedirects(PhoneForwardList_t *redirects, PhoneNumbers *result,
                           char const *num, size_t length);

#endif
