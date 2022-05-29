#include "types.h"
#include <stdlib.h>

static bool pnum_is_full(PhoneNumbers *string) {
      return (((size_t)string->max_size == string->len));
}

PhoneNumbers *pnum_initalize(){
    PhoneNumbers *result = (PhoneNumbers*)malloc(sizeof(PhoneNumbers));
    if(!(result->list = (char**)malloc(sizeof(char*)))){
        return NULL;
    }
    result->len = 0;
    result->max_size = 1;
    return result;
}

static PhoneNumbers *pnum_resize(PhoneNumbers *pnum) {
      char **narr = (char **)realloc(
          pnum->list, (pnum->max_size + 2) * sizeof(char *));
      if (narr == NULL) {
            return NULL;
      }
      pnum->max_size  += 2;
      pnum->list = narr;
      return pnum;
}

PhoneNumbers *pnum_add(PhoneNumbers * pnum, char *num){
    if(pnum_is_full(pnum)){
        if(!(pnum = pnum_resize(pnum))){
            return NULL;
        }
    }
    pnum->list[++pnum->len] = num;
    return pnum;
}

