#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alphabet_check.h"

bool alphabethOk(char character, bool *endOfWord) {
      *endOfWord = false;
      if (character == '\0') {
            *endOfWord = true;
      } else if (character - '0' < 0 || character - '0' > 9) {
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
            if (!num1End) {
                  if (!alphabethOk(number1[length], &num1End)) {
                        break;
                  }
                  if (num1End) {
                        *length1 = length;
                  }
            }
            if (!num2End) {
                  if (!alphabethOk(number2[length], &num2End)) {
                        break;
                  }
                  if (num2End) {
                        *length2 = length;
                  }
            }
            if (!different) {
                  if (number1[length] != number2[length]) {
                        different = true;
                  }
            }
      }
      return different & works;
}