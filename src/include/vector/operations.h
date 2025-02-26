#pragma once

#include <header.h>

typedef enum
{
    OK,
    ERROR
} operation_status;

operation_status remove(vector_header header);
vector_header create(int min_capacity);
