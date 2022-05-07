#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie_functions.h"
#include "alphabet_check.h"


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

PhoneForward *createNode(void) {
      PhoneForward *result = (PhoneForward *)malloc(sizeof(PhoneForward));
      if (!result) {
            return NULL;
      }
      for (size_t i = 0; i < 10; i++) {
            result->further[i] = NULL;
      }
      result->redirect = NULL;
      result->parent = NULL;
      return result;
}

void safeDelete(PhoneForward *pf) {
      PhoneForward *current = NULL;
      PhoneForward *parent = pf;
      size_t leftmostIndex;
      while (parent) {
            parent = NULL;
            current = getLeftmostLeaf(pf, &parent, &leftmostIndex);
            if (parent) {
                  parent->further[leftmostIndex] = NULL;
            }
            deleteLeaf(current);
            current = parent;
      }
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
      PhoneForward *current = pf;
      while (true) {
            if (isLeaf(current, leftmostIndex, 10)) {
                  break;
            }
            *parent = current;
            current = current->further[*leftmostIndex];
      }
      return current;
}

void deleteLeaf(PhoneForward *pf) {
      if (pf->redirect) {
            free(pf->redirect);
      }
      free(pf);
}

PhoneForward *createBranch(PhoneForward *pf, char const *number, size_t length,
                           PhoneForward **firstCreated, bool *memoryFailure) {
      *memoryFailure = false;
      *firstCreated = NULL;
      PhoneForward *properPlace = pf;
      PhoneForward *current = NULL;
      for (size_t i = 0; i < length; i++) {
            if ((current = properPlace->further[(number[i] - '0')])) {
                  properPlace = current;
            } else {
                  if (!(current = createNode())) {
                        *memoryFailure = true;
                        break;
                  }
                  if (!firstCreated) {
                        *firstCreated = current;
                  }
                  properPlace->further[(number[i] - '0')] = current;
                  current->parent = properPlace;
                  properPlace = current;
            }
      }
      return properPlace;
}

PhoneForward *getDeepestEmpty(PhoneForward *pf, char const *num, bool *alright,
                              size_t *deepestIndex) {
      *alright = true;
      bool tmp, empty = true;
      PhoneForward *properPlace = pf;
      PhoneForward *deepestEmpty = NULL;
      PhoneForward *temp = NULL;
      for (size_t i = 0; i < ULONG_MAX; i++) {
            *alright = alphabethOk(num[i], &tmp);
            if (!(*alright) || tmp) {
                  break;
            }
            size_t number = (num[i] - '0');
            temp = properPlace->further[number];
            if (!temp) {
                  *alright = false;
                  break;
            }
            empty = isLeaf(properPlace, NULL, number);
            if (!empty || properPlace->redirect || !deepestEmpty) {
                  deepestEmpty = properPlace;
                  *deepestIndex = number;
            }
            properPlace = temp;
      }
      return deepestEmpty;
}

phfwdGet_helper_t getDeepestRedirect(size_t *final,
                                                   bool *reachedEnd,
                                                   PhoneForward const *pf,
                                                   char const *num) {
      bool foundDeepest = false;
      PhoneForward const *properPlace = pf, *temp = NULL;
      phfwdGet_helper_t deepestRedirect;
      deepestRedirect.depth = 0;
      deepestRedirect.prefix = NULL;
      for (size_t i = 0; i < ULONG_MAX; i++) {
            if (!alphabethOk(num[i], reachedEnd)) {
                  break;
            }
            if (!foundDeepest) {
                  if (properPlace->redirect) {
                        deepestRedirect.depth = i;
                        deepestRedirect.prefix = properPlace->redirect;
                  }
                  if (!(*reachedEnd)) {
                        if ((temp = properPlace->further[(num[i] - '0')])) {
                              properPlace = temp;
                        } else {
                              foundDeepest = true;
                        }
                  }
            }
            if (*(reachedEnd)) {
                  *final = i;
                  break;
            }
      }
      return deepestRedirect;
}