/** @file
 * Funkcje służące do edycji, odczytu danych i obsługi drzewa trie
 * @author Wojciech Noskowiak <wn417909@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#ifndef TRIE_FUNCTIONS_H_
#define TRIE_FUNCTIONS_H_
#include <stdbool.h>
#include <stddef.h>

#include "types.h"

/** @brief Funkcja tworząca nowy węzeł w drzewie.
 * Funkcja tworzy nowy węzeł struktury i inicjalizuje jego przekierowanie
 * i wskaźniki na dalsze elementy dzrewa wartościami NULL.
 * @return Nowo utworzona struktura
 */
PhoneForward *createNode(void);

/** @brief Funkcja określająca czy dany węzeł jest liściem.
 * Funkcja określa czy @p pf jest liściem. @p leftmostIndex jest wskaźnikiem na
 * zmienną która, w przypadku gdy taki zostanie znaleziony, będzie przechowywać
 * najmniejszy niepusty index zawierający wskaźnik na dalsze elementy drzewa. @p
 * ignore określ który indeks ma zostać zignorowany przy weryfikacj. jeśli jest
 * on większy niż 9 wszystkie możliwe wskaźniki zostaną zweryfikowane.
 * @param[in] pf – wskaźnik na weryfikowany węzeł.
 * @param[out] leftmostIndex – wskaźnik na zmienną w której zostanie zapisany
 * najmniejszy niepusty wskaźnik.
 * @param[in] ignore – indeks do zignorowania w czasie weryfikacji
 * @return Wartość @p true, podany węzeł jest liściem.
 *         Wartość @p false, podany węzeł nie jest liściem
 */
bool isLeaf(PhoneForward *pf, size_t *leftmostIndex, size_t ignore);

/** @brief Funkcja znajdująca najbardziej lewy liść w zadanym poddrzewie.
 * Funkcja znajduje najbardziej lewy liść poddrzewa o korzeniu @p pf . @p parent
 * zawiera wskaźnik na zmienną w której zostanie zapisany wskaźnik na węzeł
 * zawierający wskaźnik na znaleziony liść. @p leftmostIndex zawiera wskaźnik na
 * zmienną w której zostanie zapisany index tego przekierowania.
 * @param[in] pf – wskaźnik na korzeń badanego poddrzewa.
 * @param[out] parent – wskaźnik na wskaźnik na węzeł wskazujący na znaleziony
 * liść
 * @param[out] leftmostIndex – wskaźnik na indeks w przekierowaniach węzła
 * "parent" wskazujący na znaleziony liść
 * @return Najbardiej lewy liść poddrzewa o zadanym korzeniu
 */
PhoneForward *getLeftmostLeaf(PhoneForward *pf, PhoneForward **parent,
                              size_t *leftmostIndex);

/** @brief Funkcja usuwająca węzeł w drzewie.
 * Funkcja usuwa węzeł na który wskazuje @p pf
 */
void deleteLeaf(PhoneForward *pf);

/** @brief Funkcja bezpiecznie usuwająca poddrzewo.
 * Funkcja usuwa poddrzewo o korzeniu @p pf i nie może się nie wykonać. Funkcja
 * działą w złożoności pamięciowej O(1). Funkcja znajduje najbardziej lewy liść
 * poddrzewa, usuwa go po czym przechodzi do węzła który na nie wskazywał i
 * powtarza proces do czasu usunięcia całej struktury.
 * @param[in] pf – wskaźnik korzeń poddrzewa do usunięcia.
 */
void safeDelete(PhoneForward *pf);

/** @brief Funkcja Tworząca węzeł na określonej pozycji w drzwie.
 * Funkcja tworzy węzeł w drzewie na którego korzeń wskazuje @p pf którego
 * pozycja jest określona przez napis na który wskazuje @p number . @p length
 * określa długość napisu na który wskazuje @p number. @p firstCreated wskazuje
 * na zmienną w której zapisany będzie pierwszu stworzony przez funkcję węzeł (w
 * ten sposób łatiwje wyczyścić strukturę ze zbędnych elementów w przypadku
 * braku możliwości zaalokowania pamięci). @p memoryFailure wskazuje na zmienną
 * określającą czy wystąpił błąd z zaalokowaniem pamięci.
 * @param[in] pf – wskaźnik na korzeń poddrzewa do którego zostanie dodany
 * węzeł.
 * @param[in] number – wskaźnik na napis reprezentujący lokalizację węzła
 * @param[in] length – długość napisu na który wskazuje @p number
 * @param[out] firstCreated – wskaźnik na zienną w której zostanie zapisany
 * wskaźnik na pierwszy stworzony przez funkcję węzeł.
 * @param[out] memoryFailure – wskaźnin na zmienną która po wykonaniu funkcji
 * określi czy udało się zaalokowac pamięć.
 * @return Utworzony węzeł w zadanym miejscu.
 */
PhoneForward *createBranch(PhoneForward *pf, char const *number, size_t length,
                           PhoneForward **firstCreated, bool *memoryFailure);

/** @brief Funkcja znajdująca najapłytszy węzeł nie zawieający innego
 * przekierowania na ścieżce Funkcja znajduje w drzewie na którego korzeń
 * wskazuje @p pf najpłytszy korzeń o co najwyżej jednym wskaźniku na dalszą
 * część drzewa nie zawierający przekierowania będący częścią ścieżki określonej
 * przez @p number . @p alright jest wskaźnikiem na zmienną w której zostanie
 * zapisane czy @p number reprezentje poprawną ścieżkę. @p deepestIndex określa
 * który index we wskaźnikach znalezionego węzła wskazuje na dalszą część
 * ścieżki.
 * @param[in] pf – wskaźnik na korzeń drzewa
 * @param[in] number – wskaźnik na napis reprezentujący lokalizację szukanego
 * węzła
 * @param[out] alright – wskaźnin na zmienną która określającą czy w @p number
 * reprezentje poprawną ścieżkę
 * @param[out] deepestIndex – wskaźnik na zmienną w której zostanie zapisany
 * index wskazujący na wskaźnik zawierający dalszą część drzewa.
 * @return Znaleziony najpłytszy węzeł.
 */
PhoneForward *getDeepestEmpty(PhoneForward *pf, char const *number,
                              bool *alright, size_t *deepestIndex);

/** @brief Funkcja znajdująca najapłytsze przekierowanie znajdujące sie na
 * scieżce oraz weryfikuje poprawniość napisu. Funkcja znajduje w drzewie na
 * którego korzeń wskazuje @p pf najpłytsze przekierowanie znajdujące się po
 * drodze określonej przez @p number oraz sprawdza czy reprezentuje on poprwaną
 * ścieżkę.
 * @p reachedEnd jest wskaźniekiem na zmienną określajacą czy @p number
 * reprezentowało poprawną ścieżkę.
 * @p final to wskaźnik na zmienną w kórej zosatnie zapisana ostateczna długość
 * numeru reprezentowanego przez @p number .
 * @param[in] pf – wskaźnik na korzeń drzewa
 * węzeł.
 * @param[in] number – wskaźnik na napis reprezentujący badaną ścieżkę.
 * @param[out] reachedEnd – wskaźnin na zmienną która określającą czy w @p
 * number reprezentje poprawną ścieżkę
 * @param[out] final – wskaźnik na zmienną w której zostanie zapisana długość
 * napisu reprezentowanego przez @p number
 * @return struktura zawierająca w sobie głębokość znalezionego przekierowania
 * oraz przekierowanie.
 */
phfwdGet_helper_t getDeepestRedirect(size_t *final, bool *reachedEnd,
                                     PhoneForward const *pf,
                                     char const *number);

char *treverseUp(PhoneForward* pf, size_t *length);

PhoneForward *getElement(PhoneForward *pf, char const *number);

void deleteBranch(PhoneForward* pf);

void cutHighestUseless(PhoneForward *pf);

void buggie(PhoneForward *root,PhoneForward *pf);

void removeForward(PhoneForward *root, PhoneForward *pf);

#endif