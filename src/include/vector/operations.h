#pragma once

#include "header.h"
#include "../operation_result.h"

vector_header init_vector(const int capacity);
operation_result free_vector(vector_header* header);
long get(const vector_header* header, int index);
operation_result set(vector_header* const header, const int index, const long value);
operation_result insert(vector_header* const header, const int index, const long value);
operation_result push_back(vector_header* const header, const long value);
operation_result erase(vector_header* const header, const int index);
operation_result pop_back(vector_header* const header);
