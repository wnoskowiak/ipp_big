#include <stdbool.h>
#include <string.h>
#include <stdlib.h>  
#include "types.h"

static char *strrev(char *str)
{
      char *p1, *p2;

      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}

static bool string_is_full(resizable_string_t *string) {
      return (((size_t)string->last_filled == string->length - 1));
}

resizable_string_t *string_initalize(){
    resizable_string_t *result = (resizable_string_t *)malloc(sizeof(resizable_string_t));
    if(!(result->char_list = (char*)malloc(3*sizeof(char)))){
        return NULL;
    }
    result->last_filled = -1;
    result->length = 3;
    return result;
}

static resizable_string_t *string_resize(resizable_string_t *string) {
      char *narr = (char *)realloc(
          string->char_list, (string->length + 2) * sizeof(char));
      if (narr == NULL) {
            return NULL;
      }
      string->length  += 2;
      string->char_list = narr;
      return string;
}

resizable_string_t *string_add(resizable_string_t *string, char item){
    if(string_is_full(string)){
        if(!(string = string_resize(string))){
            return NULL;
        }
    }
    string->char_list[++string->last_filled] = item;
    return string;
}

char *string_close(resizable_string_t *string){
    string = string_add(string,'\n');
    return  strrev(string->char_list);
}

resizable_string_t *string_reopen(resizable_string_t *string){
    string->last_filled -= 1;
    return  string;
}