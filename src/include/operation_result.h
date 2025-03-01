#pragma once

typedef enum
{
    OK,
    ERR_INVALID_HEADER,
    ERR_INVALID_CAPACITY,
    ERR_MALLOC_FAILED,
    ERR_REALLOC_FAILED,
    ERR_OUT_OF_BOUNDS,
    ERR_NULL
} operation_result;
