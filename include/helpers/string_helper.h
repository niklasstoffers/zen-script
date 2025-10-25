#ifndef STRING_HELPER_H
#define STRING_HELPER_H

#include <stdbool.h>

typedef bool (*CharPredicate)(char);

bool string_null_or_empty(const char* s);
bool string_not_null_or_empty(const char* s);

bool string_match_all(const char* s, CharPredicate predicate);
bool string_match_none(const char* s, CharPredicate predicate);
const char* string_first(const char* s, CharPredicate predicate);
const char* string_first_not(const char* s, CharPredicate predicate);
const char* string_end(const char* s);

#endif