/** @file
 * 
 * @author Wojciech Noskowiak <wn417909@students.mimuw.edu.pl>
 * @copyright Uniwersytet Warszawski
 * @date 2022
 */

#ifndef PHONE_NUMBERS_MANAGER_
#define PHONE_NUMBERS_MANAGER_

#include "types.h"

PhoneNumbers *pnum_initalize();

PhoneNumbers *pnum_add(PhoneNumbers * pnum, char *num);

void pnum_destroy(PhoneNumbers *pnum);

#endif
