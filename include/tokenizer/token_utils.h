#ifndef TOKEN_UTILS_H
#define TOKEN_UTILS_H

#include <stdbool.h>

bool is_space(char c);
bool is_linebreak(char c);
bool is_string_delimiter(char c);
bool is_escape_character(char c);
bool is_primitive(char c);
bool is_token_delimiter(char c);

bool token_is_identifier(const char* s);
bool token_is_keyword(const char* s);
bool token_is_number(const char* s);
bool token_is_string(const char* s);
bool token_is_linebreak(const char* s);
bool token_is_plus(const char* s);
bool token_is_minus(const char* s);
bool token_is_asterisk(const char* s);
bool token_is_slash(const char* s);
bool token_is_percent(const char* s);

const char* find_next_token(const char* s);
const char* find_token_end(const char* s);

#endif