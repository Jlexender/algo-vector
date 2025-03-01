#include <malloc.h>
#include "../include/deamortized_vector/header.h"
#include "../include/deamortized_vector/operations.h"
#include "../include/vector/operations.h"

deamortized_vector_header init_deamortized_vector(const int capacity)
{
    // Made to avoid cases where capacity < MIN_CAPACITY
    // and therefore results in a technical error.
    int real_capacity = capacity < MIN_CAPACITY ? MIN_CAPACITY : real_capacity;

    // Made to avoid corner-cases with odd size.
    // UPDATED: bad decision
    // if (real_capacity % 2 != 0)
    // {
    //     real_capacity++;
    // }

    vector_header current = init_vector(real_capacity);
    vector_header next = init_vector(real_capacity * 2);

    return (deamortized_vector_header){
        current,
        next};
}

operation_result free_deamortized_vector(deamortized_vector_header *header)
{
    operation_result result = free_vector(&header->next_vector);
    if (result != OK)
    {
        return result;
    }

    return free_vector(&header->current_vector);
}

long get(const deamortized_vector_header *header, int index)
{
    return get(&header->current_vector, index);
}

operation_result set(deamortized_vector_header *const header, const int index, const long value)
{
    if (index >= header->reallocated_amount)
    {
        return set(&header->current_vector, index, value);
    }

    operation_result result = set(&header->next_vector, index, value);
    if (result != OK)
    {
        return result;
    }

    return set(&header->current_vector, index, value);
}

operation_result insert(deamortized_vector_header *const header, const int index, const long value)
{
    if (header->current_vector.size >= header->current_vector.capacity / 2)
    {
        operation_result result = push_back(&header->next_vector, get(&header->current_vector, header->reallocated_amount++));
        if (result != OK)
        {
            return result;
        }

        result = push_back(&header->next_vector, get(&header->current_vector, header->reallocated_amount++));

        if (result != OK) {
            return result;
        }
    }

    if (index >= header->reallocated_amount) {
        return insert(&header->current_vector, index, value);
    }

    operation_result result = insert(&header->next_vector)
}

operation_result push_back(deamortized_vector_header *const header, const long value)
{
    return insert(header, header->current_vector.size, value);
}

operation_result erase(deamortized_vector_header *const header, const int index)
{
    if (index >= header->reallocated_amount)
    {
        return erase(&header->current_vector, index);
    }

    operation_result result = erase(&header->next_vector, index);
    if (result != OK)
    {
        return result;
    }

    header->reallocated_amount--;

    return erase(&header->current_vector, index);
}

operation_result pop_back(deamortized_vector_header *const header)
{
    return erase(header, header->current_vector.size - 1);
}
