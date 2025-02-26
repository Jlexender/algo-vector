#include <malloc.h>
#include <stddef.h>
#include "include/vector/header.h"
#include "include/vector/operations.h"

static void* get_address(vector_header header, int offset) {
    return header.start_address + offset * sizeof(long);
}

static int is_invalid(vector_header header) {
    return header.is_allocated == 0;
}

static vector_header get_invalid_header() {
    return (vector_header) {
        0,
        NULL,
        0,
        0
    };
}

operation_result free_vector(vector_header header) {
    vector_header invalid_header = get_invalid_header();

    if (is_invalid(header)) {
        return (operation_result) {
            ERR_NOT_ALLOCATED,
            invalid_header
        };
    }

    free(get_address(header, 0));
    set_free(header);

    return (operation_result) {
        OK,
        invalid_header
    };
}

operation_result init_vector(int capacity) {
    vector_header invalid_header = get_invalid_header();

    if (capacity <= 0) {
        return (operation_result) {
            ERR_INVALID_CAPACITY,
            invalid_header
        };
    }

    int actual_capacity = MIN_CAPACITY;
    while (actual_capacity < capacity) {
        actual_capacity <<= 1;
    }

    void *start_address = malloc(actual_capacity * sizeof(long));
    if (start_address == NULL) {
        return (operation_result) {
            ERR_MALLOC_FAILED,
            invalid_header
        };
    }
    
    vector_header header = {
        1,
        start_address,
        0,
        actual_capacity
    };

    return (operation_result) {
        OK,
        header
    };
}

operation_result pop_back(vector_header header) {
    vector_header invalid_header = get_invalid_header();

    if (is_invalid(header)) {
        return (operation_result) {
            ERR_NOT_ALLOCATED,
            invalid_header
        };
    }

    if (header.size == 0) {
        return (operation_result) {
            OK,
            header
        };
    }

    header.size--;

    return (operation_result) {
        OK,
        header
    };
}




