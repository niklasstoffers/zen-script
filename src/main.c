#include "tokenizer/tokenizer.h"
#include "parser/parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static char* get_token_type_string(TokenType type);
static char* expression_to_string(Expression* expression);

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

    printf("\n\n\n");

    ParserError parser_error = PARSER_ERROR_OK;
    Program* program = NULL;
    err = parse_program(stream, &program, &parser_error);
    token_stream_free(stream);

    if (err != ZENC_ERROR_OK)
    {
        printf("Parsing failed: %s\n", zenc_strerror(err));
        return 1;
    }
    else if (parser_error != PARSER_ERROR_OK)
    {
        printf("Invalid program format!\n");
        return 1;
    }

    StatementNode* current_statement = program->head;
    while (current_statement) {
        if (current_statement->statement->type == STATEMENT_TYPE_DECLARATION)
        {
            char* exp = expression_to_string(current_statement->statement->declaration->expression);
            printf("Declaration: { Identifier: %s, Value: %s }\n", current_statement->statement->declaration->identifier, exp);
            free(exp);
        }
        else if (current_statement->statement->type == STATEMENT_TYPE_PRINT)
        {
            char* exp = expression_to_string(current_statement->statement->print_statement->expression);
            printf("Print Statement: { Value: %s }\n", exp);
            free(exp);
        }

        current_statement = current_statement->next;
    }

    free_program(program);
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

// // Berechne Länge des benötigten Strings
//     size_t len = snprintf(NULL, 0, "Expression: %s", expression->value);
    
//     // +1 für Null-Terminator
//     char* str = (char*)malloc(len + 1);
//     if (!str) return NULL;

//     // String formatieren
//     snprintf(str, len + 1, "Expression: %s", expression->value);
//     return str; // Caller muss free() aufrufen!

static char* expression_to_string(Expression* expression)
{
    if (expression->type == EXPRESSION_TYPE_IDENTIFIER) 
    {
        size_t len = snprintf(NULL, 0, "Expression: { Type: %s, Value: %s }", "Identifier", expression->identifier);
        char* str = (char*)malloc(len + 1);
        snprintf(str, len + 1, "Expression: { Type: %s, Value: %s }", "Identifier", expression->identifier);
        return str;
    }
    else if (expression->type == EXPRESSION_TYPE_NUMBER)
    {
        size_t len = snprintf(NULL, 0, "Expression: { Type: %s, Value: %d }", "Number", expression->number);
        char* str = (char*)malloc(len + 1);
        snprintf(str, len + 1, "Expression: { Type: %s, Value: %d }", "Number", expression->number);
        return str;
    }

    return NULL;
}