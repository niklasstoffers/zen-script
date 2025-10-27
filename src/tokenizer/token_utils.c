#include "tokenizer/token_utils.h"
#include "helpers/string_helper.h"
#include "lang/keywords.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

static bool number_predicate(char c) { return isdigit(c); }
static bool identifier_predicate(char c) { return isalpha(c) || isdigit(c) || c == '_'; }
static bool identifier_prefix_predicate(char c) { return isalpha(c) || c == '_'; }

static bool is_string_delimiter_linebreak_or_escape(char c) { return is_linebreak(c) || is_string_delimiter(c) || is_escape_character(c); }

static const char* find_linebreak_end(const char* s);
static const char* find_string_end(const char* s, bool* valid);

bool is_space(char c) { return c == ' ' || c == '\t'; }
bool is_linebreak(char c) { return c == '\n' || c == '\r'; }
bool is_string_delimiter(char c) { return c == '\"'; }
bool is_escape_character(char c) { return c == '\\'; }

bool is_primitive(char c)
{
    return c == '+'
        || c == '-'
        || c == '*'
        || c == '/'
        || c == '%'; 
}

bool is_token_delimiter(char c) 
{ 
    return is_space(c) 
        || is_linebreak(c) 
        || is_string_delimiter(c)
        || is_primitive(c);
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
    if (*s == '0' && s[1] != '\0') return false;
    return string_match_all(s, number_predicate);
}

bool token_is_string(const char* s)
{
    if (string_null_or_empty(s)) return false;
    if (!is_string_delimiter(*s)) return false;
    bool valid = false;
    (void)find_string_end(s, &valid);
    return valid;
}

bool token_is_linebreak(const char* s)
{
    if (string_null_or_empty(s)) return false;
    else if (strlen(s) == 1) return is_linebreak(*s);
    else if (strlen(s) == 2) return s[0] == '\r' && s[1] == '\n';
    return false;
}

bool token_is_plus(const char* s) { return strcmp(s, "+") == 0; }
bool token_is_minus(const char* s) { return strcmp(s, "-") == 0; }
bool token_is_asterisk(const char* s) { return strcmp(s, "*") == 0; }
bool token_is_slash(const char* s) { return strcmp(s, "/") == 0; }
bool token_is_percent(const char* s) { return strcmp(s, "%") == 0; }

const char* find_next_token(const char* s)
{
    return string_first_not(s, is_space);
}

const char* find_token_end(const char* s)
{
    if (string_null_or_empty(s)) return NULL;
    
    if (is_linebreak(*s)) 
        return find_linebreak_end(s);
    else if (is_string_delimiter(*s))
        return find_string_end(s, NULL);
    else if (is_primitive(*s))
        return s + 1;

    return string_first(s, is_token_delimiter);
}

static const char* find_linebreak_end(const char* s)
{
    if (*s == '\n' || (*s == '\r' && s[1] != '\n')) 
        return s + 1;
    return s + 2;
}

static const char* find_string_end(const char* s, bool* valid)
{
    if (valid) *valid = false;
    const char* end = s;
    while (end)
    {
        end = string_first(end + 1, is_string_delimiter_linebreak_or_escape);
        if (!end) 
            break;
        else if (is_linebreak(*end))
            return end;
        else if (is_escape_character(*end))
            end++;
        else
        {
            if (valid) *valid = true;
            return end + 1;
        }
    }

    return string_end(s);
}