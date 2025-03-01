#include <malloc.h>
#include "../include/deamortized_vector/header.h"
#include "../include/deamortized_vector/operations.h"
#include "../include/vector/operations.h"

static int get_capacity(const int capacity)
{
    return capacity < MIN_CAPACITY ? MIN_CAPACITY : capacity;
}

deamortized_vector_header init_deamortized_vector(const int capacity)
{
    // Made to avoid corner-cases with odd size.
    // Removed: bad decision
    // if (real_capacity % 2 != 0)
    // {
    //     real_capacity++;
    // }

    int real_capacity = get_capacity(capacity);

    vector_header current = init_vector(real_capacity);
    vector_header next = init_vector(real_capacity * 2);

    return (deamortized_vector_header){
        current,
        next,
        0};
}

operation_result free_deamortized_vector(deamortized_vector_header *header)
{
    if (header == NULL)
    {
        return ERR_NULL;
    }
    operation_result result = free_vector(&header->next_vector);
    if (result != OK)
    {
        return result;
    }

    return free_vector(&header->current_vector);
}

long deamortized_get(const deamortized_vector_header *header, int index)
{
    return get(&header->current_vector, index);
}

operation_result deamortized_set(deamortized_vector_header *const header, const int index, const long value)
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

operation_result deamortized_insert(deamortized_vector_header *const header, const int index, const long value)
{
    operation_result result;

    if (index >= header->reallocated_amount)
    {
        result = insert(&header->current_vector, index, value);
        if (result != OK)
        {
            return result;
        }
    }
    else
    {
        result = insert(&header->next_vector, index, value);
        if (result != OK)
        {
            return result;
        }

        header->reallocated_amount++;

        result = insert(&header->current_vector, index, value);
        if (result != OK)
        {
            return result;
        }
    }

    if (header->current_vector.size >= header->current_vector.capacity / 2)
    {
        result = push_back(&header->next_vector, get(&header->current_vector, header->reallocated_amount++));

        if (result != OK)
        {
            return result;
        }

        if (header->current_vector.size != header->reallocated_amount)
        {
            result = push_back(&header->next_vector, get(&header->current_vector, header->reallocated_amount++));

            if (result != OK)
            {
                return result;
            }
        }
    }

    if (header->current_vector.size == header->current_vector.capacity)
    {
        vector_header new_vector = init_vector(header->next_vector.capacity * 2);

        if (new_vector.is_allocated == 0)
        {
            return ERR_INVALID_HEADER;
        }

        free_vector(&header->current_vector);

        header->current_vector = header->next_vector;
        header->next_vector = new_vector;
        header->reallocated_amount = 0;
    }

    return OK;
}

operation_result deamortized_push_back(deamortized_vector_header *const header, const long value)
{
    return deamortized_insert(header, header->current_vector.size, value);
}

operation_result deamortized_erase(deamortized_vector_header *const header, const int index)
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

operation_result deamortized_pop_back(deamortized_vector_header *const header)
{
    return deamortized_erase(header, header->current_vector.size - 1);
}

int get_size(const deamortized_vector_header *const header)
{
    return header->current_vector.size;
}
