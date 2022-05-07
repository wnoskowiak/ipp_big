/** @file
 * deklaracje typów i struktur wykorzystywanych przez napisane moduły
 * @author Wojciech Noskowiak <wn417909@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#ifndef TYPES_H_
#define TYPES_H_
#include <stddef.h>

/**
 * To jest struktura i definicja typu reprezentującego stos
 */
typedef struct stack {
      int front; /**< największy zajęty indeks w array */
      size_t cap; /**< największy możliwy do zapisania indeks w array */
      struct PhoneForward **array; /**< lista przechowująca elementy stosu */
} stack_t;

/**
 * To jest struktura przechowująca przekierowania numerów telefonów i reprezętująca 
 * element drzewa trie.
 */
struct PhoneForward;
/**
 * To jest typ określający strukturę @p PhoneForward 
 */
typedef struct PhoneForward PhoneForward;
/**
 * przedefiniowanie struktury PhoneForward
 */
struct PhoneForward {
      PhoneForward *parent; /**< wskaźniki na element zawierający wskaźnik do tej konkretnej struktury 
      nie wykorzystywany w tej wersji programu (będzie wykorzystywane przez phfwdRemove)*/
      PhoneForward *further[10]; /**< wskaźniki na dalsze elementy drzewa */ 
      char *redirect; /**< wskaźniki na napis zawierający przekierowanie */ 
};

/**
 * To jest struktura przechowująca ciąg numerów telefonów.
 */
struct PhoneNumbers {
      size_t len; /**< maksymalny indeks zawarty w @p list */ 
      char **list; /**< lista wskaźników na napisy reprezentujące numery telefonów */ 
};
/**
 * To jest typ określający strukturę @p PhoneNumbers 
 */
typedef struct PhoneNumbers PhoneNumbers;

/**
 * To jest struktura pomocnicza wykorzystywana przez funkcję phfwdGet zawierająca 
 * głębokosć najpłytszego znalego przekierowania wraz z samym przekierowaniem
 */
typedef struct phfwdGet_helper {
      size_t depth; /**< głębokość znalezionego przekierowania */ 
      char *prefix; /**< wskaźnik na napis reprezentujący to przekierowanie */ 
} phfwdGet_helper_t;

#endif