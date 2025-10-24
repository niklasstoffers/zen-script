#include "helpers/conversion_helper.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

bool string_to_int(const char* s, int* result)
{
    if (!s || !result) return false;
    char* endptr;
    long val = strtol(s, &endptr, 10);
    if (endptr == s || *endptr != '\0') return false;
    if ((val == LONG_MAX || val == LONG_MIN) && errno == ERANGE) return false;
    if (val > INT_MAX || val < INT_MIN) return false;
    *result = (int)val;
    return true; 
}