#pragma once

#define MIN_CAPACITY 32

typedef struct {
    int is_allocated;
    void *start_address;
    int size;
    int capacity;
} vector_header;

