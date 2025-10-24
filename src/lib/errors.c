#include "lib/errors.h"

const char* zenc_strerror(ZencError error)
{
    switch (error)
    {
        case ZENC_ERROR_OK: return "OK";
        case ZENC_ERROR_INVALID_ARG: return "Invalid Argument";
        case ZENC_ERROR_NOMEM: return "Out of memory";
        case ZENC_ERROR_INTERNAL: return "Internal error";
        default: return "Unknown error";
    }
}