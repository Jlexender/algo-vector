#pragma once

#include "../vector/header.h"

typedef struct {
    vector_header current_vector;
    vector_header next_vector;
} deamortized_vector_header;
