#include "tokenizer/tokenizer.h"
#include "parser/parser.h"
#include "semantic/semantic_analyzer.h"
#include "ir/tac.h"
#include "codegen/codegen.h"
#include "helpers/assertions.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static char* get_token_type_string(TokenType type);
// static void print_tac_instruction(TACInstruction* instruction);
// static char* expression_to_string(Expression* expression);

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
    if (IS_ERROR(err))
    {
        printf("Could not allocate tokenizer: %s\n", zenc_strerror(err));
        return 1;
    }

    free(content);
    err = tokenizer_tokenize(tokenizer);
    if (IS_ERROR(err))
    {
        printf("Error during tokenization: %s\n", zenc_strerror(err));
        return 1;
    }

    if (tokenizer_had_error(tokenizer))
    {
        const TokenizerErrorList* errors = tokenizer_get_errors(tokenizer);
        TokenizerErrorListIterator iterator;
        (void)tokenizer_error_list_iterator_init(&iterator, errors);
        while (tokenizer_error_list_iterator_has_next(&iterator))
        {
            const TokenizerError* error = tokenizer_error_list_iterator_next(&iterator);
            printf("Unexpected token: %s\n", error->token);
        }
    }
    else
    {
        const TokenList* tokens = tokenizer_get_token_list(tokenizer);
        TokenListIterator iterator;
        (void)token_list_iterator_init(&iterator, tokens);
        while (token_list_iterator_has_next(&iterator))
        {
            const Token* token = token_list_iterator_next(&iterator);
            if (token->type == TOKEN_TYPE_LINEBREAK)
                printf("%s\n", get_token_type_string(token->type));
            else
                printf("%s(%s), ", get_token_type_string(token->type), token->value);
        }
    }

    tokenizer_free(tokenizer);
    return 0;

    // Tokenizer* tokenizer = NULL;
    // ZencError err = tokenizer_new(content, &tokenizer);
    // free(content);

    // TokenizerError* tokenizer_error = NULL;
    // TokenStream* stream = NULL;
    // err = tokenizer_tokenize(tokenizer, &stream, &tokenizer_error);

    // tokenizer_free(tokenizer);

    // if (err != ZENC_ERROR_OK)
    // {
    //     printf("Tokenization failed: %s\n", zenc_strerror(err));
    //     return 1;
    // }
    
    // if (tokenizer_error)
    // {
    //     if (tokenizer_error->type == TOKENIZER_ERROR_INVALID_TOKEN)
    //     {
    //         printf("Invalid token detected: %s\n", tokenizer_error->invalid_token_error->token);
    //         tokenizer_error_free(tokenizer_error);
    //         return 1;
    //     }
    // }

    // TokenNode* current = stream->head;
    // while(current) {
    //     if (current->token->type == TOKEN_TYPE_LINEBREAK)
    //         printf("%s\n", get_token_type_string(current->token->type));
    //     else
    //         printf("%s(%s), ", get_token_type_string(current->token->type), current->token->value);
    //     current = current->next;
    // }

    // printf("\n\n\n");

    // ParserError parser_error = PARSER_ERROR_OK;
    // Program* program = NULL;
    // err = parse_program(stream, &program, &parser_error);
    // token_stream_free(stream);

    // if (err != ZENC_ERROR_OK)
    // {
    //     printf("Parsing failed: %s\n", zenc_strerror(err));
    //     return 1;
    // }
    // else if (parser_error != PARSER_ERROR_OK)
    // {
    //     printf("Invalid program format!\n");
    //     return 1;
    // }

    // StatementNode* current_statement = program->head;
    // while (current_statement) {
    //     if (current_statement->statement->type == STATEMENT_TYPE_DECLARATION)
    //     {
    //         char* exp = expression_to_string(current_statement->statement->declaration->expression);
    //         printf("Declaration: { Identifier: %s, Value: %s }\n", current_statement->statement->declaration->identifier, exp);
    //         free(exp);
    //     }
    //     else if (current_statement->statement->type == STATEMENT_TYPE_PRINT)
    //     {
    //         char* exp = expression_to_string(current_statement->statement->print_statement->expression);
    //         printf("Print Statement: { Value: %s }\n", exp);
    //         free(exp);
    //     }

    //     current_statement = current_statement->next;
    // }

    // SemanticErrorList* semantic_errors = NULL;
    // err = validate_semantic(program, &semantic_errors);
    // if (err != ZENC_ERROR_OK)
    // {
    //     printf("Semantic validation failed: %s\n", zenc_strerror(err));
    //     return 1;
    // }

    // printf("\n\n\n");
    // SemanticErrorNode* semantic_error = semantic_errors->head;
    // bool had_semantic_error = semantic_error != NULL;
    // while(semantic_error) 
    // {
    //     switch (semantic_error->error->type)
    //     {
    //         case SEMANTIC_ERROR_REDECLARATION: printf("Redeclaration Error of symbol: %s\n", semantic_error->error->redeclaration->identifier); break;
    //         case SEMANTIC_ERROR_UNDECLARED_IDENTIFIER: printf("Undeclared identifier: %s\n", semantic_error->error->undeclared_identifier->identifier); break;
    //     }
    //     semantic_error = semantic_error->next;
    // }

    // free_semantic_error_list(semantic_errors);
    // if (had_semantic_error)
    //     return 1;

    // TACList* tac_list = NULL;
    // err = ast_to_tac(program, &tac_list);
    // if (err != ZENC_ERROR_OK)
    // {
    //     printf("Could not convert AST to TAC: %s\n", zenc_strerror(err));
    //     return 1;
    // }

    // printf("\n\n\n");
    // TACNode* tac_node = tac_list->head;
    // while (tac_node)
    // {
    //     print_tac_instruction(tac_node->instruction);
    //     tac_node = tac_node->next;
    // }

    // printf("\n\n\n");
    // err = emit_assembler(tac_list, "out.asm");
    // if (err != ZENC_ERROR_OK)
    // {
    //     printf("Code generation failed: %s\n", zenc_strerror(err));
    //     return 1;
    // }
        
    // free_program(program);
    // return 0;
}

static char* get_token_type_string(TokenType type)
{
    if (type == TOKEN_TYPE_KEYWORD)
        return "KEYWORD";
    else if (type == TOKEN_TYPE_IDENTIFIER)
        return "IDENTIFIER";
    else if (type == TOKEN_TYPE_NUMBER)
        return "NUMBER";
    else if (type == TOKEN_TYPE_STRING)
        return "STRING";
    else if (type == TOKEN_TYPE_LINEBREAK)
        return "LINEBREAK";
    else if (type == TOKEN_TYPE_INVALID)
        return "INVALID";
    return NULL;
}

// static void print_tac_instruction(TACInstruction* instruction)
// {
//     if (instruction->op == TAC_OPERATION_ASSIGN)
//     {
//         if (instruction->first->type == TAC_OPERAND_NUMBER)
//             printf("%s = %d\n", instruction->result, instruction->first->number);
//         else if (instruction->first->type == TAC_OPERAND_VARIABLE)
//             printf("%s = %s\n", instruction->result, instruction->first->variable);
//     }
//     else if (instruction->op == TAC_OPERATION_PRINT)
//     {
//         if (instruction->first->type == TAC_OPERAND_NUMBER)
//             printf("print %d\n", instruction->first->number);
//         else if (instruction->first->type == TAC_OPERAND_VARIABLE)
//             printf("print %s\n", instruction->first->variable);
//     }
// }

// static char* expression_to_string(Expression* expression)
// {
//     if (expression->type == EXPRESSION_TYPE_IDENTIFIER) 
//     {
//         size_t len = snprintf(NULL, 0, "Expression: { Type: %s, Value: %s }", "Identifier", expression->identifier);
//         char* str = (char*)malloc(len + 1);
//         snprintf(str, len + 1, "Expression: { Type: %s, Value: %s }", "Identifier", expression->identifier);
//         return str;
//     }
//     else if (expression->type == EXPRESSION_TYPE_NUMBER)
//     {
//         size_t len = snprintf(NULL, 0, "Expression: { Type: %s, Value: %d }", "Number", expression->number);
//         char* str = (char*)malloc(len + 1);
//         snprintf(str, len + 1, "Expression: { Type: %s, Value: %d }", "Number", expression->number);
//         return str;
//     }

//     return NULL;
// }