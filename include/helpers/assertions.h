#ifndef ASSERTIONS_H
#define ASSERTIONS_H

#include "lib/errors.h"
#include <stdlib.h>

#define RETURN_IF_NULL_VAL(RETVAL, ARG) \
    do { \
        if (!ARG) return RETVAL; \
    } while(0)

#define RETURN_IF_NOT_GREATER_ZERO_VAL(RETVAL, ARG) \
    do { \
        if (ARG <= 0) return RETVAL; \
    } while(0)

#define RETURN_IF_NULL_FREE_VAL(RETVAL, ARG, FREE) \
    do { \
        if (!ARG) { free(FREE); return RETVAL; } \
    } while(0)

#define ASSERT_NOT_NULL(ARG) RETURN_IF_NULL_VAL(ZENC_ERROR_INVALID_ARG, ARG)
#define ASSERT_GREATER_ZERO(ARG) RETURN_IF_NOT_GREATER_ZERO_VAL(ZENC_ERROR_INVALID_ARG, ARG)
#define ASSERT_ALLOC(ARG) RETURN_IF_NULL_VAL(ZENC_ERROR_NOMEM, ARG)
#define ASSERT_ALLOC_FREE(ARG, FREE) RETURN_IF_NULL_FREE_VAL(ZENC_ERROR_NOMEM, ARG, FREE)
#define RETURN_IF_NULL(ARG) RETURN_IF_NULL_VAL(, ARG)

#define IS_ERROR(ARG) ARG != ZENC_ERROR_OK
#define IS_NO_ERROR(ARG) !IS_ERROR(ARG)

#define ASSERT_NO_ERROR(ARG) \
    do { \
        if (IS_ERROR(ARG)) return ARG; \
    } while(0)

#define ASSERT_NO_ERROR_FREE(ARG, FREE) \
    do { \
        if (IS_ERROR(ARG)) { free(FREE); return ARG; } \
    } while(0)

#endif