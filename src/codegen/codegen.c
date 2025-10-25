// #include "codegen/codegen.h"
// #include <stdio.h>
// #include <stdbool.h>

// static bool emit_data_section(TACList* list, FILE* fp);
// static bool emit_main_section(FILE* fp);
// static ZencError emit_program_code(TACList* list, FILE* fp);
// static ZencError emit_assignment(TACInstruction* instruction, FILE* fp);
// static ZencError emit_print(TACInstruction* instruction, FILE* fp);
// static ZencError emit_expression(TACOperand* operand, FILE* fp);

// ZencError emit_assembler(TACList* list, const char* file_path)
// {
//     if (!list || !file_path)
//         return ZENC_ERROR_INVALID_ARG;

//     FILE* fp = fopen(file_path, "w");
//     if (!fp)
//         return ZENC_ERROR_SYSTEM;

//     if (!emit_data_section(list, fp)) goto fail;
//     if (!emit_main_section(fp)) goto fail;

//     ZencError err = emit_program_code(list, fp);
//     if (err != ZENC_ERROR_OK)
//     {
//         fclose(fp);
//         return err;
//     }

//     return ZENC_ERROR_OK;

// fail:
//     fclose(fp);
//     return ZENC_ERROR_SYSTEM;
// }

// static bool emit_data_section(TACList* list, FILE* fp)
// {
//     if (fprintf(fp, "section .data\n") < 0) return false;
//     if (fprintf(fp, "_fmt db \"%%d\", 10, 0\n") < 0) return false;

//     TACNode* node = list->head;
//     while (node) 
//     {
//         if (node->instruction->op == TAC_OPERATION_ASSIGN)
//         {
//             int res = fprintf(fp, "%s dd 0\n", node->instruction->result);
//             if (res < 0) return false;
//         }
//         node = node->next;
//     }

//     return true;
// }

// static bool emit_main_section(FILE* fp)
// {
//     if (fprintf(fp, "\nsection .text\n") < 0) return false;
//     if (fprintf(fp, "global main\n") < 0) return false;
//     if (fprintf(fp, "extern printf\n") < 0) return false;
//     if (fprintf(fp, "\nmain:\n") < 0) return false;
//     return true;
// }

// static ZencError emit_program_code(TACList* list, FILE* fp)
// {
//     ZencError err = ZENC_ERROR_OK;
//     TACNode* node = list->head;

//     while (node)
//     {
//         switch (node->instruction->op)
//         {
//             case TAC_OPERATION_ASSIGN:
//                 err = emit_assignment(node->instruction, fp);
//                 break;
//             case TAC_OPERATION_PRINT:
//                 err = emit_print(node->instruction, fp);
//                 break;
//             default:
//                 return ZENC_ERROR_INTERNAL;
//         }

//         if (err != ZENC_ERROR_OK)
//             return err;
        
//         if (fprintf(fp, "\n") < 0)
//             return ZENC_ERROR_SYSTEM;

//         node = node->next;
//     }

//     return ZENC_ERROR_OK;
// }

// static ZencError emit_assignment(TACInstruction* instruction, FILE* fp)
// {
//     ZencError err = ZENC_ERROR_OK;
//     err = emit_expression(instruction->first, fp); 
//     if (err != ZENC_ERROR_OK)
//         return err;

//     if (fprintf(fp, "\tmov dword [rel %s], eax", instruction->result) < 0) 
//         return ZENC_ERROR_SYSTEM;

//     if (fprintf(fp, "\n") < 0) 
//         return ZENC_ERROR_SYSTEM;
//     return ZENC_ERROR_OK;
// }

// static ZencError emit_print(TACInstruction* instruction, FILE* fp)
// {
//     if (fprintf(fp, "\tlea rdi, [rel _fmt]\n") < 0) return ZENC_ERROR_SYSTEM;
    
//     ZencError err = emit_expression(instruction->first, fp);
//     if (err != ZENC_ERROR_OK)
//         return err;

//     if (fprintf(fp, "\tmov esi, eax") < 0) return ZENC_ERROR_SYSTEM;
//     if (fprintf(fp, "\n\txor eax, eax\n") < 0) return ZENC_ERROR_SYSTEM;
//     if (fprintf(fp, "\tcall printf\n") < 0) return ZENC_ERROR_SYSTEM;
//     if (fprintf(fp, "\tmov rax, 60\n") < 0) return ZENC_ERROR_SYSTEM;
//     if (fprintf(fp, "\txor rdi, rdi\n") < 0) return ZENC_ERROR_SYSTEM;
//     if (fprintf(fp, "\tsyscall\n") < 0) return ZENC_ERROR_SYSTEM;

//     return ZENC_ERROR_OK;
// }

// static ZencError emit_expression(TACOperand* operand, FILE* fp)
// {
//     int res = 0;
//     switch (operand->type)
//     {
//         case TAC_OPERAND_NUMBER:
//             res = fprintf(fp, "\tmov eax, %d\n", operand->number);
//             break;
//         case TAC_OPERAND_VARIABLE:
//             res = fprintf(fp, "\tmov eax, dword [rel %s]\n", operand->variable);
//             break;
//         default:
//             return ZENC_ERROR_INTERNAL;
//     }

//     if (res < 0)
//         return ZENC_ERROR_SYSTEM;
//     return ZENC_ERROR_OK;
// }