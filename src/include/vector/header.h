#pragma once

typedef struct {
    int is_allocated;
    void *start_address;
    int element_size;
    int size;
    int capacity;
} vector_header;

