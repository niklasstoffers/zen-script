#ifndef TAC_H
#define TAC_H

#include "lib/errors.h"
#include "parser/ast_defs.h"

typedef enum {
    TAC_OPERATION_ASSIGN,
    TAC_OPERATION_PRINT
} TACOperation;

typedef enum {
    TAC_OPERAND_NUMBER,
    TAC_OPERAND_VARIABLE
} TACOperandType;

typedef struct {
    TACOperandType type;
    union {
        char* variable;
        int number;
    };
} TACOperand;

typedef struct {
    TACOperation op;
    char* result;
    TACOperand* first;
    TACOperand* second;
} TACInstruction;

typedef struct TACNode {
    TACInstruction* instruction;
    struct TACNode* next;
} TACNode;

typedef struct {
    TACNode* head;
} TACList;

ZencError ast_to_tac(Program* program, TACList** list);
void tac_list_free(TACList* list);

#endif