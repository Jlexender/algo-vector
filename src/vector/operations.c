#include <malloc.h>
#include <stddef.h>
#include <stdbool.h>
#include "../include/vector/header.h"
#include "../include/vector/operations.h"

static long *get_address(vector_header *header, int offset)
{
    return header->start_address + offset * sizeof(long);
}

static int is_invalid(vector_header *header)
{
    return !header->is_allocated;
}

static void invalidate(vector_header *header)
{
    header->is_allocated = 0;
}

static operation_result grow_vector(vector_header *header)
{
    if (is_invalid(header))
    {
        return ERR_INVALID_HEADER;
    }

    int new_capacity = header->capacity * 2;
    long *new_start_address = realloc(header->start_address, new_capacity * sizeof(long));

    if (new_start_address == NULL)
    {
        return ERR_REALLOC_FAILED;
    }

    header->start_address = new_start_address;
    header->capacity = new_capacity;

    return OK;
}

operation_result free_vector(vector_header *header)
{
    if (is_invalid(header))
    {
        return ERR_INVALID_HEADER;
    }

    free(get_address(header, 0));
    invalidate(header);

    return OK;
}

vector_header init_vector(int capacity)
{
    if (capacity <= 0)
    {
        return (vector_header){
            false,
            NULL,
            0,
            0};
    }

    int actual_capacity = MIN_CAPACITY;
    while (actual_capacity < capacity)
    {
        actual_capacity <<= 1;
    }

    void *start_address = malloc(actual_capacity * sizeof(long));

    if (start_address == NULL)
    {
        return (vector_header){
            false,
            NULL,
            0,
            0};
    }

    return (vector_header){
        true,
        start_address,
        0,
        actual_capacity};
}

long get(vector_header *header, int index)
{
    if (is_invalid(header))
    {
        return ERR_INVALID_HEADER;
    }

    if (index < 0 || index >= header->size)
    {
        return ERR_OUT_OF_BOUNDS;
    }

    return *get_address(header, index);
}

operation_result set(vector_header *header, int index, long value)
{
    if (is_invalid(header))
    {
        return ERR_INVALID_HEADER;
    }

    if (index < 0 || index >= header->size)
    {
        return ERR_OUT_OF_BOUNDS;
    }

    *get_address(header, index) = value;
    return OK;
}

operation_result insert(vector_header *header, int index, long value)
{
    if (is_invalid(header))
    {
        return ERR_INVALID_HEADER;
    }

    if (index < 0 || index > header->size)
    {
        return ERR_OUT_OF_BOUNDS;
    }

    if (header->capacity == header->size)
    {
        operation_result grow_result = grow_vector(header);
        if (grow_result != OK)
        {
            return grow_result;
        }
    }

    header->size++;

    for (int i = header->size - 1; i > index; --i)
    {
        *get_address(header, i) = *get_address(header, i - 1);
    }

    *get_address(header, index) = value;
    return OK;
}

operation_result push_back(vector_header *header, long value)
{
    return insert(header, header->size, value);
}

operation_result erase(vector_header *header, int index)
{
    if (is_invalid(header))
    {
        return ERR_INVALID_HEADER;
    }

    if (index < 0 || index >= header->size)
    {
        return ERR_OUT_OF_BOUNDS;
    }

    for (int i = index; i < header->size - 1; ++i)
    {
        *get_address(header, i) = *get_address(header, i + 1);
    }

    --header->size;
    return OK;
}

operation_result pop_back(vector_header *header)
{
    return erase(header, header->size - 1);
}
