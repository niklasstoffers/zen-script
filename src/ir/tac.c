// #include "ir/tac.h"
// #include <stdlib.h>
// #include <string.h>

// static ZencError statement_to_tac(Statement* statement, TACInstruction** instruction);
// static ZencError expression_to_operand(Expression* expression, TACOperand** operand);

// static void tac_instruction_free(TACInstruction* instruction);
// static void tac_operand_free(TACOperand* operand);

// ZencError ast_to_tac(Program* program, TACList** list)
// {
//     if (!program || !list)
//         return ZENC_ERROR_INVALID_ARG;

//     TACList* t = (TACList*)malloc(sizeof(TACList));
//     if (!t)
//         return ZENC_ERROR_NOMEM;

//     t->head = NULL;
//     TACNode* current = NULL;
//     StatementNode* statement = program->head;
//     while (statement)
//     {
//         TACInstruction* instruction = NULL;
//         ZencError err = statement_to_tac(statement->statement, &instruction);
//         if (err != ZENC_ERROR_OK)
//         {
//             tac_list_free(t);
//             return err;
//         }

//         TACNode* node = (TACNode*)calloc(1, sizeof(TACNode));
//         if (!node)
//         {
//             tac_list_free(t);
//             tac_instruction_free(instruction);
//             return ZENC_ERROR_NOMEM;
//         }

//         node->instruction = instruction;

//         if (!current) t->head = node;
//         else current->next = node;

//         current = node;
//         statement = statement->next;
//     }

//     *list = t;
//     return ZENC_ERROR_OK;
// }

// static ZencError statement_to_tac(Statement* statement, TACInstruction** instruction)
// {
//     TACInstruction* inst = (TACInstruction*)calloc(1, sizeof(TACInstruction));
//     if (!inst)
//         return ZENC_ERROR_NOMEM;

//     TACOperand* operand = NULL;
//     ZencError err = ZENC_ERROR_OK;

//     switch (statement->type)
//     {
//         case STATEMENT_TYPE_DECLARATION:
//             inst->op = TAC_OPERATION_ASSIGN;
//             inst->result = strdup(statement->declaration->identifier);
//             if (!inst->result)
//             {
//                 free(inst);
//                 return ZENC_ERROR_NOMEM;
//             }

//             err = expression_to_operand(statement->declaration->expression, &operand);
//             if (err != ZENC_ERROR_OK)
//             {
//                 free(inst);
//                 return err;
//             }

//             inst->first = operand;
//             break;
//         case STATEMENT_TYPE_PRINT:
//             inst->op = TAC_OPERATION_PRINT;

//             err = expression_to_operand(statement->print_statement->expression, &operand);
//             if (err != ZENC_ERROR_OK)
//             {
//                 free(inst);
//                 return err;
//             }

//             inst->first = operand;
//             break;
//         default:
//             free(inst);
//             return ZENC_ERROR_INTERNAL;
//     }

//     *instruction = inst;
//     return ZENC_ERROR_OK;
// }

// static ZencError expression_to_operand(Expression* expression, TACOperand** operand)
// {
//     TACOperand* op = (TACOperand*)malloc(sizeof(TACOperand));
//     if (!op)
//         return ZENC_ERROR_NOMEM;
    
//     switch (expression->type)
//     {
//         case EXPRESSION_TYPE_IDENTIFIER:
//             op->type = TAC_OPERAND_VARIABLE;
//             op->variable = strdup(expression->identifier);
//             if (!op->variable)
//             {
//                 free(op);
//                 return ZENC_ERROR_NOMEM;
//             }
//             break;
//         case EXPRESSION_TYPE_NUMBER:
//             op->type = TAC_OPERAND_NUMBER;
//             op->number = expression->number;
//             break;
//         default:
//             free(op);
//             return ZENC_ERROR_INTERNAL;
//     }

//     *operand = op;
//     return ZENC_ERROR_OK;
// }

// void tac_list_free(TACList* list)
// {
//     if (!list)
//         return;
    
//     TACNode* node = list->head;
//     while (node)
//     {
//         TACNode* temp = node->next;
//         tac_instruction_free(node->instruction);
//         free(node);
//         node = temp;
//     }

//     free(list);
// }

// static void tac_instruction_free(TACInstruction* instruction)
// {
//     if (!instruction)
//         return;
//     free(instruction->result);
//     tac_operand_free(instruction->first);
//     tac_operand_free(instruction->second);
//     free(instruction);
// }

// static void tac_operand_free(TACOperand* operand)
// {
//     if (!operand)
//         return;
    
//     if (operand->type == TAC_OPERAND_VARIABLE)
//         free(operand->variable);
//     free(operand);
// }