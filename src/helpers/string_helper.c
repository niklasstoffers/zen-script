#include "helpers/string_helper.h"
#include "string.h"
#include <stdlib.h>

static const char* string_first_generic(const char* s, CharPredicate predicate, bool inverse);

bool string_null_or_empty(const char* s)
{
    if (!s) return true;
    return !*s;
}

bool string_not_null_or_empty(const char* s)
{
    return !string_null_or_empty(s);
}

bool string_match_all(const char* s, CharPredicate predicate)
{
    return string_first_generic(s, predicate, true) == NULL;
}

bool string_match_none(const char* s, CharPredicate predicate)
{
    return string_first_generic(s, predicate, false) == NULL;
}

const char* string_first(const char* s, CharPredicate predicate)
{
    return string_first_generic(s, predicate, false);
}

const char* string_first_not(const char* s, CharPredicate predicate)
{
    return string_first_generic(s, predicate, true);
}

const char* string_end(const char* s)
{
    if (!s) return NULL;
    size_t length = strlen(s);
    return &(s[length]);
}

static const char* string_first_generic(const char* s, CharPredicate predicate, bool inverse)
{
    if (!s) return NULL;
    while(*s)
    {
        if (predicate((unsigned char)*s) != inverse) return s;
        s++;
    }
    return NULL;
}