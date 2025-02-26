#pragma once

#include "header.h"

typedef enum {
    OK,
    ERR_INVALID_HEADER,
    ERR_INVALID_CAPACITY,
    ERR_MALLOC_FAILED,
    ERR_REALLOC_FAILED
} operation_result;

vector_header init_vector(int capacity);
operation_result free_vector(vector_header* header);
operation_result push_back(vector_header* header, long value);
operation_result pop_back(vector_header* header);


