/** @file
 * Funkcje służące do edycji, odczytu danych i obsługi drzewa trie
 * @author Wojciech Noskowiak <wn417909@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

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

/** @brief Funkcja określająca czy dany węzeł jest liściem.
 * Funkcja określa czy @p pf jest liściem. @p leftmostIndex jest wskaźnikiem na zmienną 
 * która, w przypadku gdy taki zostanie znaleziony, będzie przechowywać najmniejszy niepusty
 * index zawierający wskaźnik na dalsze elementy drzewa. @p ignore określ który indeks
 * ma zostać zignorowany przy weryfikacj. jeśli jest on większy niż 9 wszystkie możliwe wskaźniki
 * zostaną zweryfikowane 
 * @param[in] pf – wskaźnik weryfikowany węzeł.
 * @param[out] leftmostIndex – wskaźnik na zmienną w której zostanie zapisany najmniejszy niepusty 
 * wskaźnik.
 * @param[in] ignore – inder do zignorowania w czasie weryfikacji
 * @return Wartość @p true, podany węzeł jest liściem.
 *         Wartość @p false, podany węzeł nie jest liściem
 */

bool isLeaf(PhoneForward *pf, size_t *leftmostIndex, size_t ignore);

PhoneForward *getLeftmostLeaf(PhoneForward *pf, PhoneForward **parent,
                              size_t *leftmostIndex);

void deleteLeaf(PhoneForward *pf);

/** @brief Funkcja bezpiecznie usuwająca poddrzewo.
 * Funkcja usuwa poddrzewo o korzeniu @p pf i nie może się nie wykonać. Funkcja
 * działą w złożoności pamięciowej O(1). Funkcja znajduje najbardziej lewy liść
 * poddrzewa, usuwa go po czym przechodzi do węzła który na nie wskazywał i powtarza proces
 * do czasu usunięcia całej struktury.
 * @param[in] pf – wskaźnik korzeń poddrzewa do usunięcia.
 */
void safeDelete(PhoneForward *pf);

PhoneForward *createBranch(PhoneForward *pf, char const *number, size_t length,
                           PhoneForward **firstCreated, bool *memoryFailure);

PhoneForward *getDeepestEmpty(PhoneForward *pf, char const *num, bool *alright,
                              size_t *deepestIndex);

phfwdGet_helper_t getDeepestRedirect(size_t *final,
                                                   bool *reachedEnd,
                                                   PhoneForward const *pf,
                                                   char const *num);

#endif