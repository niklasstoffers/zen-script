#ifndef TOKEN_UTILS_H
#define TOKEN_UTILS_H

#include <stdbool.h>

bool is_space(char c);
bool is_linebreak(char c);

bool token_is_identifier(const char* s);
bool token_is_keyword(const char* s);
bool token_is_number(const char* s);
bool token_is_linebreak(const char* s);

const char* find_next_token(const char* s);
const char* find_token_end(const char* s);

#endif