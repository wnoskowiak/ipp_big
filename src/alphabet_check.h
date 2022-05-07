#ifndef ALPHABET_CHECK_H_
#define ALPHABET_CHECK_H_
#include <stdlib.h>
#include <stdbool.h>

bool alphabethOk(char number, bool *endOfWord);

bool numbersOk(size_t *length1, size_t *length2, char const *number1,
                             char const *number2);

#endif