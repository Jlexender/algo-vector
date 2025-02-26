#include <malloc.h>
#include <stddef.h>
#include <stdbool.h>
#include "include/vector/header.h"
#include "include/vector/operations.h"

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

static operation_result grow_vector(vector_header *header) {
    int new_capacity = header->capacity * 2;
    long* new_start_address = realloc(header->start_address, new_capacity * sizeof(long));

    if (new_start_address == NULL) {
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


operation_result pop_back(vector_header* header)
{
    if (is_invalid(header))
    {
        return ERR_INVALID_HEADER;
    }

    if (header->size == 0)
    {
        return (operation_result){
            OK,
            header};
    }

    header->size--;

    return (operation_result){
        OK,
        header};
}

operation_result push_back(vector_header* header, long value) {
    if (is_invalid(header)) {
        return ERR_INVALID_HEADER;
    }

    if (header->size == header->capacity) {
        operation_result result = grow_vector(header);
        if (result != OK) {
            return result;
        }
    }

    *get_address(header, header->size) = value;
    header->size++;
    return OK;
}
