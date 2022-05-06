/** @file
 * Implementacja klasy przechowującej przekierowania numerów telefonicznych
 * wykorzystujący drzewo trie
 *
 * @author Wojciech Noskowiak <wn417909@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

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

PhoneForward *phfwdNew(void) {
      PhoneForward *res = (PhoneForward *)malloc(sizeof(PhoneForward));
      if (!res) {
            return NULL;
      }
      for (size_t i = 0; i < 10; i++) {
            res->further[i] = NULL;
      }
      res->redirect = NULL;
      res->parent = NULL;
      return res;
}

bool isLeaf(PhoneForward *pf, size_t *leftmostIndex, size_t ignore) {
      bool result = true;
      for (size_t i = 0; i < 10; i++) {
            if ((i != ignore) && (pf->further[i])) {
                  result = false;
                  if (leftmostIndex) {
                        *leftmostIndex = i;
                  }
                  break;
            }
      }
      return result;
}

PhoneForward *getLeftmostLeaf(PhoneForward *pf, PhoneForward **parent,
                              size_t *leftmostIndex) {
      PhoneForward *temp = pf;
      while (true) {
            if (isLeaf(temp, leftmostIndex, 10)) {
                  break;
            }
            *parent = temp;
            temp = temp->further[*leftmostIndex];
      }
      return temp;
}

void deleteLeaf(PhoneForward *pf) {
      if (pf->redirect) {
            free(pf->redirect);
      }
      free(pf);
}

void safeDelete(PhoneForward *pf) {
      PhoneForward *temp = NULL;
      PhoneForward *parent = pf;
      size_t leftmostIndex;
      while (parent) {
            parent = NULL;
            temp = getLeftmostLeaf(pf, &parent, &leftmostIndex);
            if (parent) {
                  parent->further[leftmostIndex] = NULL;
            }
            deleteLeaf(temp);
            temp = parent;
      }
}

void phfwdDelete(PhoneForward *pf) {
      safeDelete(pf);
      // if (!pf) {
      //       return;
      // }
      // stack_t *stack = stack_initialize(10);
      // if (!stack) {
      //       if (!(stack = stack_initialize(1))) {
      //             // i tu jest przypał
      //             return;
      //       }
      // }
      // add(stack, pf);
      // PhoneForward *temp = NULL;
      // while (!is_empty(stack)) {
      //       if (!(temp = pop(stack))) {
      //             return;
      //       }
      //       for (size_t i = 0; i < 10; i++) {
      //             if (temp->further[i]) {
      //                   if (!add(stack, temp->further[i])) {
      //                         // fuck
      //                   }
      //             }
      //       }
      //       deleteLeaf(temp);
      // }
      // stack_destroy(stack);
}

static inline bool alphabethOk(char num, bool *endOfWord) {
      if (num == '\0') {
            *endOfWord = true;
      } else if (num - '0' < 0 || num - '0' > 9) {
            return false;
      }
      return true;
}

static inline bool numbersOk(size_t *len1, size_t *len2, char const *num1,
                             char const *num2) {
      bool works = false, different = false, num1_end = false, num2_end = false;
      for (size_t length = 0; length < ULONG_MAX; length++) {
            if (num1_end & num2_end) {
                  works = true;
                  break;
            }
            if (!num1_end) {
                  if (!alphabethOk(num1[length], &num1_end)) {
                        break;
                  }
                  if (num1_end) {
                        *len1 = length;
                  }
            }
            if (!num2_end) {
                  if (!alphabethOk(num2[length], &num2_end)) {
                        break;
                  }
                  if (num2_end) {
                        *len2 = length;
                  }
            }
            if (!different) {
                  if (num1[length] != num2[length]) {
                        different = true;
                  }
            }
      }
      return different & works;
}

PhoneForward *createBranch(PhoneForward *pf, char const *num, size_t len,
                           PhoneForward **first_created, bool *memfail) {
      *memfail = false;
      *first_created = NULL;
      PhoneForward *proper_place = pf;
      PhoneForward *temp = NULL;
      for (size_t i = 0; i < len; i++) {
            if ((temp = proper_place->further[(num[i] - '0')])) {
                  proper_place = temp;
            } else {
                  if (!(temp = phfwdNew())) {
                        *memfail = true;
                        break;
                  }
                  if (!first_created) {
                        *first_created = temp;
                  }
                  proper_place->further[(num[i] - '0')] = temp;
                  temp->parent = proper_place;
                  proper_place = temp;
            }
      }
      return proper_place;
}

bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2) {
      if (!(num1 && num2 && pf)) {
            return false;
      }
      size_t len1 = 0, len2 = 0;
      if (numbersOk(&len1, &len2, num1, num2) && len1 > 0 && len2 > 0) {
            bool memfail;
            PhoneForward *first_created;
            PhoneForward *proper_place =
                createBranch(pf, num1, len1, &first_created, &memfail);
            if (!memfail) {
                  if (proper_place->redirect) {
                        free(proper_place->redirect);
                  }
                  proper_place->redirect =
                      (char *)malloc((len2 + 1) * sizeof(char));
                  if (proper_place->redirect) {
                        strcpy(proper_place->redirect, num2);
                        proper_place->redirect[len2] = '\0';
                        return true;
                  }
            }
            phfwdDelete(first_created);
      }
      return false;
}

void phfwdRemove(PhoneForward *pf, char const *num) {
      if (!(pf && num)) {
            return;
      }
      bool works = true, tmp = false, empty = true;
      PhoneForward *proper_place = pf;
      PhoneForward *deepest_empty = NULL;
      PhoneForward *temp = NULL;
      size_t deepest_index = 0;
      for (size_t i = 0; i < ULONG_MAX; i++) {
            works = alphabethOk(num[i], &tmp);
            if (!works || tmp) {
                  break;
            }
            size_t number = (num[i] - '0');
            temp = proper_place->further[number];
            if (!temp) {
                  works = false;
                  break;
            }
            empty = isLeaf(proper_place, NULL, number);
            // ebe ebe
            // for (size_t j = 0; j < 10; j++) {
            //       if ((j != number) && (proper_place->further[j])) {
            //             empty = false;
            //             break;
            //       }
            // }
            if (!empty || proper_place->redirect || !deepest_empty) {
                  deepest_empty = proper_place;
                  deepest_index = number;
            }
            proper_place = temp;
      }
      if (works && deepest_empty) {
            phfwdDelete(deepest_empty->further[deepest_index]);
            deepest_empty->further[deepest_index] = NULL;
      }
}

static inline phfwdGet_helper_t getDeepestRedirect(size_t *final,
                                                   bool *reached_end,
                                                   PhoneForward const *pf,
                                                   char const *num) {
      bool found_deepest = false;
      PhoneForward const *proper_place = pf, *temp = NULL;
      phfwdGet_helper_t deepest_redirect;
      deepest_redirect.depth = 0;
      deepest_redirect.prefix = NULL;
      for (size_t i = 0; i < ULONG_MAX; i++) {
            if (!alphabethOk(num[i], reached_end)) {
                  break;
            }
            if (!found_deepest) {
                  if (proper_place->redirect) {
                        deepest_redirect.depth = i;
                        deepest_redirect.prefix = proper_place->redirect;
                  }
                  if (!(*reached_end)) {
                        if ((temp = proper_place->further[(num[i] - '0')])) {
                              proper_place = temp;
                        } else {
                              found_deepest = true;
                        }
                  }
            }
            if (*(reached_end)) {
                  *final = i;
                  break;
            }
      }
      return deepest_redirect;
}

static inline char *joinNumbers(const char *num1, const char *num2,
                                size_t num1len, size_t num2len) {
      size_t new_length = num1len + num2len + 1;
      char *new_number = (char *)malloc((new_length) * sizeof(char));
      if (!new_number) {
            return NULL;
      }
      strcpy(new_number, num1);
      if (num2) {
            strcat(new_number, num2);
      }
      new_number[new_length - 1] = '\0';
      return new_number;
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
      bool reached_end = false;
      size_t final = 0;
      phfwdGet_helper_t deepest_redirect =
          getDeepestRedirect(&final, &reached_end, pf, num);
      if (reached_end && final != 0) {
            char *new_number = NULL;
            const char *num1;
            const char *num2;
            size_t num1len = 0, num2len = 0;
            if (deepest_redirect.prefix) {
                  num1len = strlen(deepest_redirect.prefix);
                  num2len = final - deepest_redirect.depth;
                  num1 = deepest_redirect.prefix;
                  num2 = &num[deepest_redirect.depth];
            } else {
                  num1 = num;
                  num2 = NULL;
                  num1len = final;
            }
            if (!(new_number = joinNumbers(num1, num2, num1len, num2len))) {
                  free(result);
                  return NULL;
            }
            if (!(result->list = (char **)malloc(sizeof(char *)))) {
                  free(result);
                  return NULL;
            }
            result->len = 1;
            result->list[0] = new_number;
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