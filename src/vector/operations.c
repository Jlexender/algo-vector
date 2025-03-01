#include <malloc.h>
#include <stddef.h>
#include <stdbool.h>
#include "../include/vector/header.h"
#include "../include/vector/operations.h"

static int get_capacity(const int capacity)
{
    return capacity < MIN_CAPACITY ? MIN_CAPACITY : capacity;
}

static long *get_address(const vector_header *const header, const int offset)
{
    return header->start_address + offset;
}

static int is_invalid(const vector_header *const header)
{
    return !header->is_allocated;
}

static void invalidate(vector_header *const header)
{
    header->is_allocated = 0;
}

static operation_result grow_vector(vector_header *const header) {
    if (header == NULL) {
        return ERR_NULL;
    }

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

operation_result free_vector(vector_header *const header)
{
    if (header == NULL) {
        return ERR_NULL;
    }

    if (is_invalid(header))
    {
        return ERR_INVALID_HEADER;
    }

    free(get_address(header, 0));
    invalidate(header);

    return OK;
}

vector_header init_vector(const int capacity)
{
    if (capacity <= 0)
    {
        return (vector_header){
            false,
            NULL,
            0,
            0};
    }

    // FIXME: CHANGED 01.03
    // Seems like it doesn't really affect the current impl, but I'm not sure...
    int actual_capacity = get_capacity(capacity);

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

long get(const vector_header *const header, const int index)
{
    if (header == NULL) {
        return ERR_NULL;
    }

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

operation_result set(vector_header *const header, const int index, const long value)
{
    if (header == NULL) {
        return ERR_NULL;
    }

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

operation_result insert(vector_header *const header, const int index, const long value)
{
    if (header == NULL) {
        return ERR_NULL;
    }

    if (is_invalid(header))
    {
        return ERR_INVALID_HEADER;
    }

    // note: in this implementation, insert on vector size
    // is considered as pushing the element back
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

operation_result push_back(vector_header *const header, const long value)
{
    if (header == NULL) {
        return ERR_NULL;
    }

    return insert(header, header->size, value);
}

operation_result erase(vector_header *const header, const int index)
{
    if (header == NULL) {
        return ERR_NULL;
    }

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

operation_result pop_back(vector_header *const header)
{
    if (header == NULL) {
        return ERR_NULL;
    }

    return erase(header, header->size - 1);
}
