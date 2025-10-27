#ifndef DEFINITION_H
#define DEFINITION_H

#include "lib/errors.h"

typedef struct {
    char* variable;
} Definition;

ZencError definition_new(const char* variable, Definition** definition);
void definition_free(Definition* definition);

#endif