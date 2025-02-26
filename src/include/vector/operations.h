#pragma once

#include "header.h"

typedef enum {
    OK,
    ERR_INVALID_HEADER,
    ERR_INVALID_CAPACITY,
    ERR_MALLOC_FAILED,
    ERR_REALLOC_FAILED,
    ERR_OUT_OF_BOUNDS
} operation_result;

vector_header init_vector(int capacity);
operation_result free_vector(vector_header* header);
long get(vector_header* header, int index);
operation_result set(vector_header* header, int index, long value);
operation_result insert(vector_header *header, int index, long value);
operation_result push_back(vector_header* header, long value);
operation_result erase(vector_header *header, int index);
operation_result pop_back(vector_header* header);

