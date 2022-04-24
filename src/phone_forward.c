#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/*

SPRAWDZAĆ CZY CHARY REPREZENTUJĄ LICZBY!!!!

*/

struct PhoneForward;

typedef struct PhoneForward PhoneForward;

struct PhoneForward
{
    PhoneForward *further[10];
    char *redirect;
};

struct PhoneNumbers
{
    size_t len;
    char **list;
};
typedef struct PhoneNumbers PhoneNumbers;

PhoneForward *phfwdNew(void)
{
    return (PhoneForward *)malloc(sizeof(PhoneForward));
}

void phfwdDelete(PhoneForward *pf)
{
    PhoneForward *temp = *pf->further;
    for (size_t i = 0; i < 9; i++)
    {
        if (temp)
        {
            phfwdDelete(temp);
        }
        temp++;
    }
    free(pf);
}

bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2)
{
    bool different = false;
    bool length_same = false;
    size_t length = 0;
    while (length < 10)
    {
        if (num1[length] == '\0')
        {
            if (num2[length] == '\0')
            {
                length_same = true;
            }
            break;
        }
        if (num1[length] != num2[length])
        {
            different = true;
        }
        length++;
    }
    if (different & length_same)
    {
        PhoneForward *proper_place = pf;
        for (size_t i = 0; i < length; i++)
        {
            PhoneForward *temp = proper_place->further[(size_t)(num1[i] - '0')];
            if (temp)
            {
                proper_place = temp;
            }
            else
            {
                // dodać fail w przypadku niezalokowania pamięci
                temp = phfwdNew();
                proper_place->further[(size_t)(num1[i] - '0')] = temp;
                proper_place = temp;
            }
        }
        // tu jednak trzeba kurwa kopiować
        proper_place->redirect = num2;
        return true;
    }
    else
    {
        return false;
    }
}

void phfwdRemove(PhoneForward *pf, char const *num)
{
    bool found = false;
    PhoneForward *proper_place = pf;
    size_t i = 0;
    while (i < 9)
    {
        if (num[i] == '\0')
        {
            found = true;
            break;
        }
        PhoneForward *temp = proper_place->further[(size_t)(num[i] - '0')];
        if (temp)
        {
            proper_place = temp;
        }
        else
        {
            break;
        }
        i++;
    }
    proper_place->redirect = NULL;
    bool empty = true;
    for (size_t i = 0; i < 9; i++)
    {
        if (proper_place->further[i])
        {
            empty = false;
            break;
        }
    }
    if (empty)
    {
        phfwdDelete(proper_place);
    }
}

PhoneNumbers *phfwdGet(PhoneForward const *pf, char const *num)
{
    PhoneNumbers *result = (PhoneNumbers *)malloc(sizeof(PhoneNumbers));
    size_t temp = 0;
    bool num_ok = false;
    while (temp < 10)
    {
        if (num[temp] == '\0')
        {
            if (temp == 9)
            {
                num_ok = true;
            }
            break;
        }
    }
    if (num_ok)
    {
        char* redirected = (char*)malloc(8*sizeof(char));
        PhoneForward *proper_place = pf;
        size_t i = 0;
        while (i < 9)
        {
            if (num[i] == '\0')
            {
                break;
            }
            PhoneForward *temp = proper_place->further[(size_t)(num[i] - '0')];
            if (temp)
            {
                proper_place = temp;
            }
            else
            {
                break;
            }
            i++;
        }
        if (proper_place->redirect)
        {
            // tutaj musisz konkatonować listy charów
        }
        else
        {
            result->len = 1;
            result->list = (char **)malloc(sizeof(char));
        }
    }
    else
    {
        result->len = 0;
        return result;
    }
}