/** @file
 * Implementacje funkcji do weryfikacji poprawnośći napisów reprezentujących
 * numery telefonów
 *
 * @author Wojciech Noskowiak <wn417909@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

/*obczaić funkcję memmove*/

#include "alphabet_check.h"

#include <limits.h>
#include <stdbool.h>

#include <stdio.h>

int charToNum(char chr) {
      int result = -1;
      if (chr - '0' >= 0 && chr - '0' <= 9) {
            result = (int)(chr - '0');
      }
      if (chr - '#' == 0) {
            result = 10;
      }
      if (chr - '*' == 0) {
            result = 11;
      }
      return result;
}

bool alphabethOk(char character, bool *endOfWord, size_t *index) {
      // inicjalizujemy endOfWord jako false
      if (endOfWord) {
            *endOfWord = false;
      }
      if (character == '\0') {
            // jeśli character = null character to endOfWord ustawiamy na true
            if (endOfWord) {
                  *endOfWord = true;
            }
            return true;
      }
      int temp = charToNum(character);
      if(index){
            *index = (size_t)temp;
      }
      if (temp>0){
            return true;
      }
      return false;
}

char numToChar(int number) {
      if (number < 0) {
            return '\n';
      }
      char result = '\n';
      if (number < 10) {
            result = (char)(number - '0');
      }
      if (number == 10) {
            result = '#';
      }
      if (number == 11) {
            result = '*';
      }
      return result;
}

bool numbersOk(size_t *length1, size_t *length2, char const *number1,
               char const *number2) {
      bool works = false, different = false, num1End = false, num2End = false;
      for (size_t length = 0; length < ULONG_MAX; length++) {
            if (num1End & num2End) {
                  works = true;
                  break;
            }
            // sprawdzamy czy dotarliśmy juz do końca napisu
            if (!num1End) {
                  // sprawdzamy czy znak numeru jest poprawny
                  if (!alphabethOk(number1[length], &num1End, NULL)) {
                        break;
                  }
                  if (num1End) {
                        // jeśli znak jest znakiem null zapisujemy długość
                        // napisu
                        *length1 = length;
                  }
            }
            // sprawdzamy czy dotarliśmy juz do końca napisu
            if (!num2End) {
                  // sprawdzamy czy znak numeru jest poprawny
                  if (!alphabethOk(number2[length], &num2End, NULL)) {
                        break;
                  }
                  if (num2End) {
                        // jeśli znak jest znakiem null zapisujemy długość
                        // napisu
                        *length2 = length;
                  }
            }
            // sprawdzamy czy numery są różne
            if (!different) {
                  if (number1[length] != number2[length]) {
                        different = true;
                  }
            }
      }
      return different & works;
}