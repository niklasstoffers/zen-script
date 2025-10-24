#include "tokenizer/tokenizer.h"
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

    Tokenizer* tokenizer = NULL;
    ZencError err = tokenizer_new(content, &tokenizer);
    free(content);

    TokenizerError* tokenizer_error = NULL;
    TokenStream* stream = NULL;
    err = tokenizer_tokenize(tokenizer, &stream, &tokenizer_error);

    tokenizer_free(tokenizer);

    if (err != ZENC_ERROR_OK)
    {
        printf("Tokenization failed: %s\n", zenc_strerror(err));
        return 1;
    }
    
    if (tokenizer_error)
    {
        if (tokenizer_error->type == TOKENIZER_ERROR_INVALID_TOKEN)
        {
            printf("Invalid token detected: %s\n", tokenizer_error->invalid_token_error->token);
            tokenizer_error_free(tokenizer_error);
            return 1;
        }
    }

    TokenNode* current = stream->head;
    while(current) {
        if (current->token->type == TOKEN_TYPE_LINEBREAK)
            printf("%s\n", get_token_type_string(current->token->type));
        else
            printf("%s(%s), ", get_token_type_string(current->token->type), current->token->value);
        current = current->next;
    }

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
    else if (type == TOKEN_TYPE_LINEBREAK)
        return "LINEBREAK";
    else if (type == TOKEN_TYPE_INVALID)
        return "INVALID";
    return NULL;
}