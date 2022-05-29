#ifndef RESIZABLE_CHAR_LIST_
#define RESIZABLE_CHAR_LIST_

#include <stdbool.h>
#include <string.h>  
#include "types.h"

resizable_string_t *string_add(resizable_string_t *string, char item);

char *string_close(resizable_string_t *string);

resizable_string_t *string_initalize();

#endif