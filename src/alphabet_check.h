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
 * Funkcja określa czy @p character jest cyfrą pomiędzy 0 a 9 lub znakiem '#' lub '*', skutecznie
 * weryfikując czy może być ona częścią reprezentacji numeru. Jeśli podany znak
 * jest znakiem null to @p endOfWord zostanie ustawiona na true
 * @param[in] character   – wskaźnik na znak do weryfikacji.
 * @param[out] endOfWord   – wskaźnik na zmienną określającą czy podany znak
 * jest znakiem null.
 * @param[out] index   – wskaźnik na zmienną w którym zostanie zapisany index
 * na który wskazuje znak w tablicy dzieci węzła. Jeśli znak nie jest odpowiedni w tym miejscu
 * nic nie jest zapisywane.
 * @return Wartość @p true, podany znak reprezentuje cyfrę.
 *         Wartość @p false, podany znak nie reprezentuje cyfry
 */
bool alphabethOk(char character, bool *endOfWord, size_t* index);

/** @brief Funkcja zwraca znak reprezentujący liczbę określoną przez @p number
 * Funkcja zwraca char reprezentujący wartosć liczby określonej przez @p number
 * @param[in] number  – liczba  do zinterpretowania.
 * @return znk określający liczbę.
 */
char numToChar(int number);

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

/** @brief Funkcja zwraca wartość reprezentowaną przez znak @p chr
 * Funkcja zwraca wartość znaku reprezentowanego przez @p chr jako integer.
 * @param[in] chr  – znak do zinterpretowania.
 * @return Wrtość jaką określa znak.
 */
int charToNum(char chr);

/** @brief Funkcja porównuje dwa numery w kolejnośći określonej w poleceniu zadania
 * funkcja określa porządek leksykograficzny @p str1 w stosunku do @p str2
 * @param[in] str1  – wskaźnik na napis reprezentujący pierwszy numer.
 * @param[in] str2  – wskaźnik na napis reprezentujący drugi numer.
 * @return Wartość @p -1 jeśli @p str1 <  @p str2 , @p 1 w przypadku przeciwnym.
 *         Jeśli @p str1 =  @p str2 funkcja zwraca @p 0 .
 */
int numCompare (const char* str1, const char* str2);

/** @brief Funkcja łącząca dwa numery telefonu w jeden.
 * Funkcja konkatynująca @p num1 i @p num2 w sposób bezpieczny. jeśli
 * nie uda się zaalokować pamięci funkcja zwraca NULL. Funkcja nie weryfikuje
 * poprawnośći numerów. @p num1length i
 * @p num2length określają długość konkatynowamych napisów
 * @param[in] num1 – wskaźnik na napis reprezentujący pierwszy numer.
 * @param[in] num2 – wskaźnik na napis reprezentujący drugi numer.
 * @param[in] num1length – długość pierwszego numeru.
 * @param[in] num2length – długość drugiego numeru.
 * @return wskaźnik na zkonkatynowany napis
 */
char *joinNumbers(const char *num1, const char *num2,
                                size_t num1length, size_t num2length);

#endif