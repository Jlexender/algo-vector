#include <malloc.h>
#include <stddef.h>
#include "include/vector/header.h"
#include "include/vector/operations.h"

static void* get_address(vector_header header, int offset) {
    return header.start_address + offset * header.element_size;
}

static int is_invalid(vector_header header) {
    return header.is_allocated == 0;
}

static vector_header get_invalid_header() {
    return (vector_header) {
        0,
        NULL,
        0,
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

operation_result init_vector(int capacity, int element_size) {
    vector_header invalid_header = get_invalid_header();

    if (element_size <= 0) {
        return (operation_result) {
            ERR_INVALID_SIZE,
            invalid_header
        };
    }

    void *start_address = malloc(capacity * element_size);
    if (start_address == NULL) {
        return (operation_result) {
            ERR_MALLOC_FAILED,
            invalid_header
        };
    }
    
    vector_header header = {
        1,
        start_address,
        element_size,
        0,
        capacity
    };

    return (operation_result) {
        OK,
        header
    };
}


