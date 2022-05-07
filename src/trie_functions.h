#ifndef TRIE_FUNCTIONS_H_
#define TRIE_FUNCTIONS_H_
#include <stdlib.h>
#include <stdbool.h>

struct PhoneForward;

typedef struct PhoneForward PhoneForward;


struct PhoneNumbers; 
typedef struct PhoneNumbers PhoneNumbers;

typedef struct phfwdGet_helper phfwdGet_helper_t;

PhoneForward *createNode(void);

void safeDelete(PhoneForward *pf);

bool isLeaf(PhoneForward *pf, size_t *leftmostIndex, size_t ignore);

PhoneForward *getLeftmostLeaf(PhoneForward *pf, PhoneForward **parent,
                              size_t *leftmostIndex);

void deleteLeaf(PhoneForward *pf);

PhoneForward *createBranch(PhoneForward *pf, char const *number, size_t length,
                           PhoneForward **firstCreated, bool *memoryFailure);

PhoneForward *getDeepestEmpty(PhoneForward *pf, char const *num, bool *alright,
                              size_t *deepestIndex);

phfwdGet_helper_t getDeepestRedirect(size_t *final,
                                                   bool *reachedEnd,
                                                   PhoneForward const *pf,
                                                   char const *num);

#endif