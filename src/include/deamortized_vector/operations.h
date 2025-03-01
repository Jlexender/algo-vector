#pragma once

#include "header.h"
#include "../operation_result.h"

deamortized_vector_header init_deamortized_vector(const int capacity);
operation_result free_deamortized_vector(deamortized_vector_header *header);
long deamortized_get(const deamortized_vector_header *header, int index);
operation_result deamortized_set(deamortized_vector_header *const header, const int index, const long value);
operation_result deamortized_insert(deamortized_vector_header *const header, const int index, const long value);
operation_result deamortized_push_back(deamortized_vector_header *const header, const long value);
operation_result deamortized_erase(deamortized_vector_header *const header, const int index);
operation_result deamortized_pop_back(deamortized_vector_header *const header);
int get_size(const deamortized_vector_header *const header);
