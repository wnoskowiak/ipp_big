#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "queue.h"
// unnececary import
#include <stdio.h>

struct PhoneForward;

typedef struct PhoneForward PhoneForward;

struct PhoneForward
{
    PhoneForward *parent;
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
    PhoneForward *res = (PhoneForward *)malloc(sizeof(PhoneForward));
    if (res)
    {
        for (size_t i = 0; i < 10; i++)
        {
            res->further[i] = NULL;
        }
        res->redirect = NULL;
        res->parent = NULL;
    }
    else
    {
        printf("chuj6\n");
    }
    return res;
}

void phfwdDelete(PhoneForward *pf)
{
    queue_t *queue = queue_initialize(10);
    PhoneForward *temp = NULL;
    add(queue, pf);
    while (is_empty(queue))
    {
        temp = pop(queue);
        if (temp->redirect)
        {
            free(temp->redirect);
        }
        for (size_t i = 0; i < 10; i++)
        {
            if (temp->further[i])
            {
                add(queue,temp->further[i]);
            }
        }
        free(temp);
    }
    queue_destroy(queue);
}

bool alphabethOk(char num, bool *endOfWord)
{
    if (num == '\0')
    {
        *endOfWord = true;
    }
    else
    {
        if (num - '0' < 0 || num - '0' > 9)
        {
            return false;
        }
    }
    return true;
}

bool numbersOk(size_t *len1, size_t *len2, char const *num1, char const *num2)
{
    bool works = false, different = false, num1_end = false, num2_end = false;
    for (size_t length = 0; length < ULONG_MAX; length++)
    {
        if (num1_end & num2_end)
        {
            works = true;
            break;
        }
        if (!num1_end)
        {
            if (!alphabethOk(num1[length], &num1_end))
            {
                break;
            }
            if (num1_end)
            {
                *len1 = length;
            }
        }
        if (!num2_end)
        {
            if (!alphabethOk(num2[length], &num2_end))
            {
                break;
            }
            if (num2_end)
            {
                *len2 = length;
            }
        }
        if (!different)
        {
            if (num1[length] != num2[length])
            {
                different = true;
            }
        }
    }
    return different & works;
}

bool phfwdAdd(PhoneForward *pf, char const *num1, char const *num2)
{
    size_t len1 = 0, len2 = 0;
    if (num1 && num2)
    {
        if (numbersOk(&len1, &len2, num1, num2) && len1 > 0 && len2 > 0)
        {
            bool mem_fail = false;
            PhoneForward *first_created = NULL;
            PhoneForward *proper_place = pf;
            PhoneForward *temp = NULL;
            for (size_t i = 0; i < len1; i++)
            {
                temp = proper_place->further[(num1[i] - '0')];
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
                        proper_place->further[(num1[i] - '0')] = temp;
                        temp->parent = proper_place;
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
                if (proper_place->redirect)
                {
                    free(proper_place->redirect);
                }
                proper_place->redirect = (char *)malloc((len2 + 1) * sizeof(char));
                if (proper_place->redirect)
                {
                    strcpy(proper_place->redirect, num2);
                    proper_place->redirect[len2] = '\0';
                    return true;
                }
                printf("chuj5\n");
                phfwdDelete(first_created);
            }
            else
            {
                printf("chuj\n");
                phfwdDelete(first_created);
            }
            return false;
        }
    }
    return false;
}

void phfwdRemove(PhoneForward *pf, char const *num)
{
    if (pf && num)
    {
        bool works = true, tmp = false, empty = true;
        PhoneForward *proper_place = pf;
        PhoneForward *deepest_empty = NULL;
        PhoneForward *temp = NULL;
        size_t deepest_index = 0;
        for (size_t i = 0; i < ULONG_MAX; i++)
        {
            works = alphabethOk(num[i], &tmp);
            if (!works || tmp)
            {
                break;
            }
            size_t number = (num[i] - '0');
            temp = proper_place->further[number];
            if (!temp)
            {
                works = false;
                break;
            }
            empty = true;
            for (size_t j = 0; j < 10; j++)
            {
                if (j != number)
                {
                    if (proper_place->further[j])
                    {
                        empty = false;
                        break;
                    }
                }
            }
            if ((!empty || proper_place->redirect) || (!deepest_empty))
            {
                deepest_empty = proper_place;
                deepest_index = number;
            }
            proper_place = temp;
        }
        if (works && deepest_empty)
        {
            phfwdDelete(deepest_empty->further[deepest_index]);
            deepest_empty->further[deepest_index] = NULL;
        }
    }
}

// phfwdGet_helper_t GetDeepestRedirect(size_t *final, bool *reached_end, PhoneForward const *pf, char const *num)
// {
//     bool found_deepest = false;
//     PhoneForward const *proper_place = pf, *temp = NULL;
//     phfwdGet_helper_t deepest_redirect;
//     deepest_redirect.depth = 0;
//     deepest_redirect.prefix = NULL;
//     for (size_t i = 0; i < ULONG_MAX; i++)
//     {
//         if (!alphabethOk(num[i], reached_end))
//         {
//             break;
//         }
//         if (!found_deepest)
//         {
//             if (proper_place->redirect)
//             {
//                 deepest_redirect.depth = i;
//                 deepest_redirect.prefix = proper_place->redirect;
//             }
//             if (!(*reached_end))
//             {
//                 temp = proper_place->further[(num[i] - '0')];
//                 if (temp)
//                 {
//                     proper_place = temp;
//                 }
//                 else
//                 {
//                     found_deepest = true;
//                 }
//             }
//         }
//         if (*(reached_end))
//         {
//             *final = i;
//             break;
//         }
//     }
// }

PhoneNumbers *phfwdGet(PhoneForward const *pf, char const *num)
{
    if (pf)
    {
        PhoneNumbers *result = (PhoneNumbers *)malloc(sizeof(PhoneNumbers));
        result->list = NULL;
        if (!result)
        {
            printf("chuj4\n");
        }
        if (result)
        {
            if (num)
            {
                bool reached_end = false, found_deepest = false;
                PhoneForward const *proper_place = pf;
                PhoneForward *temp = NULL;
                phfwdGet_helper_t deepest_redirect;
                deepest_redirect.depth = 0;
                deepest_redirect.prefix = NULL;
                size_t i = 0;
                while (i < ULONG_MAX)
                {
                    if (!alphabethOk(num[i], &reached_end))
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
                        if (!reached_end)
                        {
                            temp = proper_place->further[(num[i] - '0')];
                            if (temp)
                            {
                                proper_place = temp;
                            }
                            else
                            {
                                found_deepest = true;
                            }
                        }
                    }
                    if (reached_end)
                    {
                        break;
                    }
                    i++;
                }
                if (reached_end)
                {
                    char *new_number = NULL;
                    if (deepest_redirect.prefix)
                    {
                        size_t new_length = strlen(deepest_redirect.prefix) + i + 1 - deepest_redirect.depth;
                        new_number = (char *)malloc((new_length) * sizeof(char));
                        if (new_number)
                        {
                            strcpy(new_number, deepest_redirect.prefix);
                            strcat(new_number, &num[deepest_redirect.depth]);
                            new_number[new_length - 1] = '\0';
                        }
                        else
                        {
                            printf("chuj4\n");
                            free(result);
                            result = NULL;
                        }
                    }
                    else
                    {
                        new_number = (char *)malloc((i + 1) * sizeof(char));
                        if (new_number)
                        {
                            strcpy(new_number, num);
                            new_number[i] = '\0';
                        }
                        else
                        {
                            printf("chuj2\n");
                            free(result);
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
                            printf("chuj3\n");
                            result = NULL;
                        }
                    }
                }
                else
                {
                    result->len = 0;
                }
            }
            else
            {
                result->len = 0;
            }
        }
        return result;
    }
    return NULL;
}

char const *phnumGet(PhoneNumbers const *pnum, size_t idx)
{
    if (pnum->len <= idx)
    {
        return NULL;
    }
    return pnum->list[idx];
}

void phnumDelete(PhoneNumbers *pnum)
{
    if (pnum)
    {
        if (pnum->list)
        {
            for (size_t i = 0; i < pnum->len; i++)
            {
                free(pnum->list[i]);
            }
            free(pnum->list);
        }
        free(pnum);
    }
}