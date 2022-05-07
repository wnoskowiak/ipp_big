#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "alphabet_check.h"
#include "stack.h"
#include "trie_functions.h"

struct PhoneForward;

typedef struct PhoneForward PhoneForward;

struct PhoneForward {
      PhoneForward *parent;
      PhoneForward *further[10];
      char *redirect;
};

struct PhoneNumbers {
      size_t len;
      char **list;
};
typedef struct PhoneNumbers PhoneNumbers;

typedef struct phfwdGet_helper {
      size_t depth;
      char *prefix;
} phfwdGet_helper_t;

PhoneNumbers *phfwdReverse(PhoneForward const *pf, char const *num) {
      pf = pf;
      num = num;
      return NULL;
}

PhoneForward *phfwdNew() { return createNode(); }

void phfwdDelete(PhoneForward *pf) {
      if (!pf) {
            return;
      }
      stack_t *stack = stack_initialize(10);
      if (!stack) {
            safeDelete(pf);
            return;
      }
      PhoneForward *temp = NULL;
      add(stack, pf);
      while (!is_empty(stack)) {
            temp = pop(stack);
            for (size_t i = 0; i < 10; i++) {
                  if (temp->further[i]) {
                        if (!add(stack, temp->further[i])) {
                              safeDelete(temp->further[i]);
                        }
                  }
            }
            deleteLeaf(temp);
      }
      stack_destroy(stack);
}

bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2) {
      if (!(num1 && num2 && pf)) {
            return false;
      }
      size_t length1 = 0, lenght2 = 0;
      if (numbersOk(&length1, &lenght2, num1, num2) && length1 > 0 &&
          lenght2 > 0) {
            bool memoryFailure;
            PhoneForward *firstCreated;
            PhoneForward *properPlace =
                createBranch(pf, num1, length1, &firstCreated, &memoryFailure);
            if (!memoryFailure) {
                  if (properPlace->redirect) {
                        free(properPlace->redirect);
                  }
                  properPlace->redirect =
                      (char *)malloc((lenght2 + 1) * sizeof(char));
                  if (properPlace->redirect) {
                        strcpy(properPlace->redirect, num2);
                        properPlace->redirect[lenght2] = '\0';
                        return true;
                  }
            }
            phfwdDelete(firstCreated);
      }
      return false;
}

void phfwdRemove(PhoneForward *pf, char const *num) {
      if (!(pf && num)) {
            return;
      }
      bool alright;
      size_t deepestIndex;
      PhoneForward *deepestEmpty =
          getDeepestEmpty(pf, num, &alright, &deepestIndex);
      if (alright && deepestEmpty) {
            phfwdDelete(deepestEmpty->further[deepestIndex]);
            deepestEmpty->further[deepestIndex] = NULL;
      }
}

static inline char *joinNumbers(const char *num1, const char *num2,
                                size_t num1length, size_t num2lenght) {
      size_t newLength = num1length + num2lenght + 1;
      char *newNumber = (char *)malloc((newLength) * sizeof(char));
      if (!newNumber) {
            return NULL;
      }
      strcpy(newNumber, num1);
      if (num2) {
            strcat(newNumber, num2);
      }
      newNumber[newLength - 1] = '\0';
      return newNumber;
}

PhoneNumbers *phfwdGet(PhoneForward const *pf, char const *num) {
      if (!pf) {
            return NULL;
      }
      PhoneNumbers *result = (PhoneNumbers *)malloc(sizeof(PhoneNumbers));
      if (!result) {
            return NULL;
      }
      result->list = NULL;
      result->len = 0;
      if (!num) {
            return result;
      }
      bool reachedEnd;
      size_t final;
      phfwdGet_helper_t deepestRedirect =
          getDeepestRedirect(&final, &reachedEnd, pf, num);
      if (reachedEnd && final != 0) {
            char *newNumber = NULL;
            const char *num1;
            const char *num2;
            size_t num1length = 0, num2length = 0;
            if (deepestRedirect.prefix) {
                  num1length = strlen(deepestRedirect.prefix);
                  num2length = final - deepestRedirect.depth;
                  num1 = deepestRedirect.prefix;
                  num2 = &num[deepestRedirect.depth];
            } else {
                  num1 = num;
                  num2 = NULL;
                  num1length = final;
            }
            if (!(newNumber =
                      joinNumbers(num1, num2, num1length, num2length))) {
                  free(result);
                  return NULL;
            }
            if (!(result->list = (char **)malloc(sizeof(char *)))) {
                  free(result);
                  return NULL;
            }
            result->len = 1;
            result->list[0] = newNumber;
      }
      return result;
}

char const *phnumGet(PhoneNumbers const *pnum, size_t idx) {
      if ((!pnum) || (pnum->len <= idx)) {
            return NULL;
      }
      return pnum->list[idx];
}

void phnumDelete(PhoneNumbers *pnum) {
      if (!pnum) {
            return;
      }
      if (pnum->list) {
            for (size_t i = 0; i < pnum->len; i++) {
                  free(pnum->list[i]);
            }
            free(pnum->list);
      }
      free(pnum);
}