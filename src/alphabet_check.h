/** @file
 * Funkcje służące do weryfikacji poprawnośći napisów reprezentujących numery
 * telefonów
 *
 * @author Wojciech Noskowiak <wn417909@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#ifndef ALPHABET_CHECK_H_
#define ALPHABET_CHECK_H_
#include <stdbool.h>
#include <stdlib.h>

/** @brief Weryfikuje czy podany znak reprezentuje cyfrę.
 * Funkcja określa czy @p character jest cyfrą pomiędzy 0 a 9, skutecznie
 * weryfikując czy może być ona częścią reprezentacji numeru. Jeśli podany znak
 * jest znakiem null to @p endOfWord zostanie ustawiona na true
 * @param[in] character   – wskaźnik na znak do weryfikacji.
 * @param[out] endOfWord   – wskaźnik na zmienną określającą czy podany znak
 * jest znakiem null.
 * @return Wartość @p true, podany znak reprezentuje cyfrę.
 *         Wartość @p false, podany znak nie reprezentuje cyfry
 */
bool alphabethOk(char character, bool *endOfWord);

/** @brief Weryfikuje czy podane napist reprezentują parę różnych numerów telefonu
 *  i określa ich długość
 * Funkcja weryfikuje czy @p number1 i @p number2 są napisami składającymi się
 * z cyfr między 1 a 9 i różnią się co najmniej jednym znakiem. Funkcja oreśla
 * długość napisów i zapisuje je kolejno w zmiennych na które wskazują @p length1
 * i @p length2
 * @param[in] number1  – wskaźnik na napis reprezentujący pierwszy numer.
 * @param[in] number2  – wskaźnik na napis reprezentujący drugi numer.
 * @param[out] length1   – wskaźnik na zmienną określającą długość napisu pierwszego
 * @param[out] length2   – wskaźnik na zmienną określającą długość napisu drugiego
 * @return Wartość @p true, podane napisy reprezentują różne numery.
 *         Wartość @p false, w przeciwnym wypadku
 */
bool numbersOk(size_t *length1, size_t *length2, char const *number1,
               char const *number2);

#endif