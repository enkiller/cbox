/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-07-19     tyx          first implementation
 */

#include "cb_path.h"

cb_inline const char* cb_fnbracket_isnot(const char* p, int *isnot)
{
    p++;
    if (*p == '!' || *p == '^')
    {
        *isnot = 1;
        p++;
    }
    return p;
}

cb_inline int cb_fnbracket_cmp(const char* pattern, char c, const char **endptr)
{
    const char* prev = cb_null;
    const char* p = pattern;
    int match = 0;

    while (*p != '\0' && *p != ']')
    {
        if (*p == '-' && prev != cb_null)
        {
            if (c >= *prev && c <= *++p)
            {
                match = 1;
            }
            prev = cb_null;
        }
        else if (*p == c)
        {
            match = 1;
        }
        prev = p++;
    }
    *endptr = p;
    return match;
}

cb_inline int cb_fnbracket(const char* pattern, char c, const char **endptr)
{
    const char* p;
    int isnot = 0, match = 0;

    p = cb_fnbracket_isnot(pattern, &isnot);
    match = cb_fnbracket_cmp(p, c, endptr);
    p = *endptr;
    if (*p != ']')
    {
        if (*pattern == c)
        {
            match = 1;
        }
        *endptr = pattern;
    }
    if (isnot)
    {
        match = !match;
    }
    return match;
}

cb_inline int cb_fncmp(const char* p, char c, const char **end)
{
    int cmp = -1;

    if (*p == '?')
    {
        // Skip a character
        *end = p;
        cmp = 0;
    }
    else if (*p == '[')
    {
        cmp = !cb_fnbracket(p, c, end);
    }
    else
    {
        cmp = *p - c;
        *end = p;
    }
    return cmp;
}

int cb_fnmatch(const char * pattern, const char * string)
{
    const char* p = pattern;
    const char* s = string;
    const char* p_last = cb_null;
    const char* s_last = cb_null;
    int cmp = 0;

    while (cmp == 0 && *s != '\0')
    {
        if (*p == '*')
        {
            // Skip multiple '*'
            do { p++; } while (*p == '*');
            p_last = p;
            s_last = s;
        }
        else
        {
            cmp = cb_fncmp(p, *s, &p);
            if (cmp == 0)
            {
                p++;
                s++;
            }
            else if (p_last != cb_null)
            {
                p = p_last;
                s = ++s_last;
                cmp = 0;
            }
        }
    }
    if (*s != '\0' || *p != '\0')
    {
        cmp = *p - *s;
    }
    return cmp;
}
