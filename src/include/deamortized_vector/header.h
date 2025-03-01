#pragma once

#include "../vector/header.h"

typedef struct
{
    vector_header current_vector;
    vector_header next_vector;
    int reallocated_amount;
} deamortized_vector_header;
