#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

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

typedef struct phfwdGet_helper
{
    size_t depth;
    char *prefix;
} phfwdGet_helper_t;

PhoneForward *phfwdNew(void)
{
    return (PhoneForward *)malloc(sizeof(PhoneForward));
}

void phfwdDelete(PhoneForward *pf)
{
    if (pf)
    {
        for (size_t i = 0; i < 10; i++)
        {
            if (pf->further[i])
            {
                phfwdDelete(pf->further[i]);
            }
        }
        if (pf->redirect)
        {
            free(pf->redirect);
        }
        free(pf);
    }
}

bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2)
{
    bool works= false, different= false, num1_end= false, num2_end = false;
    size_t length = 0, len1 = 0, len2 = 0;
    while (length < ULONG_MAX)
    {
        if (num1_end & num2_end)
        {
            works = true;
            break;
        }
        if (num1[length] == '\0')
        {
            len1 = length;
            num1_end = true;
        }
        if (num2[length] == '\0')
        {
            len2 = length;
            num2_end = true;
        }
        if (
            ((!num1_end) & (num1[length] - '0' < 0 || num1[length] - '0' > 9)) ||
            ((!num2_end) & (num2[length] - '0' < 0 || num2[length] - '0' > 9)))
        {
            break;
        }
        if (!different)
        {
            if (num1[length] != num2[length])
            {
                different = true;
            }
        }
        length++;
    }
    if (different & works)
    {
        bool mem_fail = false;
        PhoneForward *first_created = NULL;
        PhoneForward *proper_place = pf;
        for (size_t i = 0; i < len1; i++)
        {
            PhoneForward *temp = proper_place->further[(size_t)(num1[i] - '0')];
            if (temp)
            {
                proper_place = temp;
            }
            else
            {
                temp = phfwdNew();
                if (temp)
                {
                    if (!first_created)
                    {
                        first_created = temp;
                    }
                    proper_place->further[(size_t)(num1[i] - '0')] = temp;
                    proper_place = temp;
                }
                else
                {
                    mem_fail = true;
                    break;
                }
            }
        }
        if (!mem_fail)
        {
            proper_place->redirect = (char *)malloc((len2 + 1) * sizeof(char));
            strcpy(proper_place->redirect, num2);
            proper_place->redirect[len2] = '\0';
            return true;
        }
        else
        {
            phfwdDelete(first_created);
            return false;
        }
    }
    else
    {
        return false;
    }
}

void phfwdRemove(PhoneForward *pf, char const *num)
{
    bool works = true;
    PhoneForward *proper_place = pf;
    PhoneForward *deepest_empty = NULL;
    size_t i = 0;
    while (i < ULONG_MAX)
    {
        if (num[i] == '\0')
        {
            break;
        }
        if ((num[i] - '0' < 0 || num[i] - '0' > 9))
        {
            works = false;
            break;
        }
        PhoneForward *temp = proper_place->further[(size_t)(num[i] - '0')];
        if (temp)
        {
            if (!deepest_empty)
            {
                deepest_empty = temp;
            }
            for (size_t i = 0; i < 10; i++)
            {
                if (proper_place->further[i])
                {
                    deepest_empty = NULL;
                    break;
                }
            }
            proper_place = temp;
        }
        else
        {
            works = false;
            break;
        }
        i++;
    }
    if (works)
    {
        if (deepest_empty)
        {
            phfwdDelete(deepest_empty);
        }
        else
        {
            phfwdDelete(proper_place);
        }
    }
}

PhoneNumbers *phfwdGet(PhoneForward const *pf, char const *num)
{
    PhoneNumbers *result = (PhoneNumbers *)malloc(sizeof(PhoneNumbers));
    if (result)
    {
        bool reached_end= false , found_deepest = false;
        phfwdGet_helper_t deepest_redirect;
        deepest_redirect.depth = 0;
        PhoneForward const *proper_place = pf;
        size_t i = 0;
        while (i < ULONG_MAX)
        {
            if (num[i] == '\0')
            {
                reached_end = true;
                break;
            }
            if ((num[i] - '0' < 0 || num[i] - '0' > 9))
            {
                break;
            }
            if (!found_deepest)
            {
                if (proper_place->redirect)
                {
                    deepest_redirect.depth = i;
                    deepest_redirect.prefix = proper_place->redirect;
                }
                PhoneForward *temp = proper_place->further[(size_t)(num[i] - '0')];
                if (temp)
                {
                    proper_place = temp;
                }
                else
                {
                    found_deepest = true;
                    break;
                }
            }
            i++;
        }
        if (reached_end)
        {
            char *new_number;
            if (deepest_redirect.prefix)
            {
                size_t new_length = strlen(deepest_redirect.prefix) + i - deepest_redirect.depth;
                new_number = (char *)malloc(new_length * sizeof(char));
                if (new_number)
                {
                    strcpy(new_number, deepest_redirect.prefix);
                    strcpy(new_number, &num[i + 1]);
                    new_number[new_length - 1] = '\0';
                }
                else
                {
                    result = NULL;
                }
            }
            else
            {
                new_number = (char *)malloc((i + 1) * sizeof(char));
                if (new_number)
                {
                    strcpy(new_number, num);
                    new_number[i - 1] = '\0';
                }
                else
                {
                    result = NULL;
                }
            }
            if (new_number)
            {
                result->list = (char **)malloc(sizeof(char *));
                if (result->list)
                {
                    result->len = 1;
                    result->list[0] = new_number;
                }
                else
                {
                    result = NULL;
                }
            }
        }
        else
        {
            result->len = 0;
        }
    }
    return result;
}

char const *phnumGet(PhoneNumbers const *pnum, size_t idx)
{
    if (pnum->len >= idx)
    {
        return NULL;
    }
    return pnum->list[idx];
}

void phnumDelete(PhoneNumbers *pnum)
{
    for (size_t i = 0; i < pnum->len; i++)
    {
        free(pnum->list[i]);
    }
    free(pnum->list);
    free(pnum);
}