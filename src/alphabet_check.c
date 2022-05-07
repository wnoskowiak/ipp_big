/** @file
 * Implementacje funkcji do weryfikacji poprawnośći napisów reprezentujących
 * numery telefonów
 *
 * @author Wojciech Noskowiak <wn417909@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#include "alphabet_check.h"

#include <limits.h>
#include <stdbool.h>

bool alphabethOk(char character, bool *endOfWord) {
      // inicjalizujemy endOfWord jako false
      *endOfWord = false;
      if (character == '\0') {
            // jeśli character = null character to endOfWord ustawiamy na true
            *endOfWord = true;
      } else if (character - '0' < 0 || character - '0' > 9) {
            // jeśli character nie zawiera się pomiędy 0 a 9 zwracyamy false
            return false;
      }
      return true;
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
                  if (!alphabethOk(number1[length], &num1End)) {
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
                  if (!alphabethOk(number2[length], &num2End)) {
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