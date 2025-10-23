#include "tokenizer/token_utils.h"
#include "helpers/string_helper.h"
#include "lang/keywords.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

static bool number_predicate(char c) { return isdigit(c); }
static bool identifier_predicate(char c) { return isalpha(c) || isdigit(c) || c == '_'; }
static bool identifier_prefix_predicate(char c) { return isalpha(c) || c == '_'; }

static bool is_space_or_linebreak(char c) { return is_space(c) || is_linebreak(c); }

bool is_space(char c)
{
    return c == ' ' || c == '\t';
}

bool is_linebreak(char c)
{
    return c == '\n' || c == '\r';
}

bool token_is_identifier(const char* s)
{
    if (string_null_or_empty(s)) return false;
    return identifier_prefix_predicate(*s) && string_match_all(s + 1, identifier_predicate);
}

bool token_is_keyword(const char* s)
{
    if (string_null_or_empty(s)) return false;
    for (int i = 0; i < num_keywords; i++)
    {
        if (strcmp(s, keywords[i]) == 0)
            return true;
    }
    return false;
}

bool token_is_number(const char* s)
{
    if (string_null_or_empty(s)) return false;
    return string_match_all(s, number_predicate);
}

bool token_is_linebreak(const char* s)
{
    if (string_null_or_empty(s)) return false;
    else if (strlen(s) == 1) return is_linebreak(*s);
    else if (strlen(s) == 2) return s[0] == '\r' && s[1] == '\n';
    return false;
}

const char* find_next_token(const char* s)
{
    return string_first_not(s, is_space);
}

const char* find_token_end(const char* s)
{
    if (string_null_or_empty(s)) return NULL;
    
    if (is_linebreak(*s)) 
    {
        if (*s == '\n' || (*s == '\r' && s[1] != '\n')) 
            return s + 1;
        return s + 2;
    }

    return string_first(s, is_space_or_linebreak);
}