#include <stdbool.h>
#include <string.h>
#include <stdlib.h> 

#include <stdio.h>
#include "types.h"

static bool string_is_full(resizable_string_t *string) {
      return (((size_t)string->last_filled == string->length));
}

resizable_string_t *string_initalize(){
    resizable_string_t *result = (resizable_string_t *)malloc(sizeof(resizable_string_t));
    if(!(result->char_list = (char*)malloc(3*sizeof(char)))){
        return NULL;
    }
    result->last_filled = 0;
    result->length = 3;
    return result;
}

void string_destroy(resizable_string_t *string){
    free(string->char_list);
    free(string);
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
        printf("sranie\n");
        if(!(string = string_resize(string))){
            return NULL;
        }
    }
    string->char_list[string->last_filled] = item;
    string->last_filled += 1;
    return string;
}

char *string_close(resizable_string_t *string){
    char *result = (char*)malloc((string->last_filled+1)*sizeof(char));
    for(size_t i = 0; i<string->last_filled; i++){
        result[i] = string->char_list[string->last_filled-i-1];
    }
    result[string->last_filled]='\0';
    string_destroy(string);
    return result;
}

resizable_string_t *string_reopen(resizable_string_t *string){
    string->last_filled -= 1;
    return  string;
}