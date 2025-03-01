#pragma once

#include "header.h"
#include "../operation_result.h"

deamortized_vector_header init_deamortized_vector(const int capacity);
operation_result free_deamortized_vector(deamortized_vector_header *header);
long get(const deamortized_vector_header *header, int index);
operation_result set(deamortized_vector_header *const header, const int index, const long value);
operation_result insert(deamortized_vector_header *const header, const int index, const long value);
operation_result push_back(deamortized_vector_header *const header, const long value);
operation_result erase(deamortized_vector_header *const header, const int index);
operation_result pop_back(deamortized_vector_header *const header);
