#include "tokenizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static char* get_token_type_string(TokenType type);

int main(int argc, char** argv)
{
    if (argc != 2) return 1;
    char *input_file_path = argv[1];
    FILE *fp = fopen(input_file_path, "rb");
    if (!fp) return 1;
    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *content = malloc(file_size + 1);
    fread(content, 1, file_size, fp);
    content[file_size] = '\0';
    fclose(fp);

    Tokenizer* tokenizer = tokenizer_new(content);
    free(content);

    Token* token = NULL;
    while(true) {
        token = tokenizer_next(tokenizer);
        if (token == NULL)
            break;

        printf("%s(%s)\n", get_token_type_string(token->type), token->value);
        token_free(token);
    }

    tokenizer_free(tokenizer);
    return 0;
}

static char* get_token_type_string(TokenType type)
{
    if (type == TOKEN_TYPE_KEYWORD)
        return "KEYWORD";
    else if (type == TOKEN_TYPE_IDENTIFIER)
        return "IDENTIFIER";
    else if (type == TOKEN_TYPE_NUMBER)
        return "NUMBER";
    return NULL;
}