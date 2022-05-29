/** @file
 * Zbiór funkcji służących do obsługi listy wskaźników na phone forward
 *
 * @author Wojciech Noskowiak <wn417909@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#ifndef PHONE_FORWARD_LIST_H_
#define PHONE_FORWARD_LIST_H_

#include <stdbool.h>
#include "types.h"

bool list_empty(PhoneForwardList_t *list);

bool list_full(PhoneForwardList_t *list);

PhoneForwardList_t *phoneForwardList_initialize(size_t cap);

void list_destroy(PhoneForwardList_t *list);

PhoneForwardList_t *list_add(PhoneForwardList_t *list, PhoneForward *item);

PhoneForwardList_t *list_remove(PhoneForwardList_t *list, PhoneForward *item);

#endif