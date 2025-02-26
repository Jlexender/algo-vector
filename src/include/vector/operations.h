#pragma once

#include "header.h"

typedef enum {
    OK,
    ERR_NOT_ALLOCATED,
    ERR_INVALID_CAPACITY,
    ERR_MALLOC_FAILED
} status_code;

typedef struct {
    status_code status;
    vector_header header;
} operation_result;

operation_result init_vector(int capacity);
operation_result free_vector(vector_header header);
operation_result push_back(vector_header header, long value);
operation_result pop_back(vector_header header);


