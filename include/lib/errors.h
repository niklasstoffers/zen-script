#ifndef ERRORS_H
#define ERRORS_H

typedef enum {
    ZENC_ERROR_OK,
    ZENC_ERROR_INVALID_ARG,
    ZENC_ERROR_NOMEM,
    ZENC_ERROR_SYSTEM,
    ZENC_ERROR_INTERNAL
} ZencError;

const char* zenc_strerror(ZencError error);

#endif