#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "include/vector.h"

#define TEST_CAPACITY 64
#define TEST_VALUE 42L
#define STRESS_TEST_SIZE 1000

void test_initialization(void)
{
    printf("Testing initialization...\n");

    // Test minimum capacity handling
    vector_header h = init_vector(10);
    assert(h.capacity == MIN_CAPACITY);
    assert(h.size == 0);
    assert(h.is_allocated);
    free_vector(&h);

    // Test valid capacity
    h = init_vector(TEST_CAPACITY);
    assert(h.capacity == TEST_CAPACITY);
    assert(h.size == 0);
    assert(h.is_allocated);
    free_vector(&h);

    // Test invalid capacity
    h = init_vector(-5);
    assert(h.is_allocated == 0);
    printf("Passed!\n\n");
}

void test_basic_operations(void)
{
    printf("Testing basic operations...\n");
    vector_header h = init_vector(MIN_CAPACITY);

    // Test push_back
    for (int i = 0; i < 5; i++)
    {
        assert(push_back(&h, i * 10) == OK);
        assert(h.size == i + 1);
    }
    assert(h.capacity == MIN_CAPACITY);

    // Test get
    for (int i = 0; i < 5; i++)
    {
        assert(get(&h, i) == i * 10);
    }

    // Test set
    assert(set(&h, 2, TEST_VALUE) == OK);
    assert(get(&h, 2) == TEST_VALUE);

    // Test out of bounds access
    assert(get(&h, -1) == ERR_OUT_OF_BOUNDS && h.is_allocated);
    assert(get(&h, 100) == ERR_OUT_OF_BOUNDS && h.is_allocated);
    assert(set(&h, -1, 5) == ERR_OUT_OF_BOUNDS);

    free_vector(&h);
    printf("Passed!\n\n");
}

void test_insert_delete(void)
{
    printf("Testing insert/delete...\n");
    vector_header h = init_vector(MIN_CAPACITY);

    // Insert at beginning
    assert(insert(&h, 0, TEST_VALUE) == OK);
    assert(h.size == 1);
    assert(get(&h, 0) == TEST_VALUE);

    // Insert at middle
    assert(insert(&h, 1, 100) == OK);
    assert(insert(&h, 1, 200) == OK);
    assert(get(&h, 1) == 200);
    assert(h.size == 3);

    // Test erase
    assert(erase(&h, 1) == OK);
    assert(h.size == 2);
    assert(get(&h, 1) == 100);

    // Test pop_back
    assert(pop_back(&h) == OK);
    assert(h.size == 1);

    // Test invalid operations
    assert(insert(&h, 5, 10) == ERR_OUT_OF_BOUNDS);
    assert(erase(&h, 5) == ERR_OUT_OF_BOUNDS);
    assert(pop_back(&h) == OK);
    assert(pop_back(&h) == ERR_OUT_OF_BOUNDS);

    free_vector(&h);
    printf("Passed!\n\n");
}

void test_capacity_management(void)
{
    printf("Testing capacity management...\n");
    vector_header h = init_vector(MIN_CAPACITY);

    // Test capacity expansion
    for (int i = 0; i < MIN_CAPACITY + 1; i++)
    {
        assert(push_back(&h, i) == OK);
    }
    assert(h.capacity > MIN_CAPACITY);

    // Test capacity reduction
    while (h.size > MIN_CAPACITY / 2)
    {
        assert(pop_back(&h) == OK);
    }
    assert(h.capacity == MIN_CAPACITY * 2);

    free_vector(&h);
    printf("Passed!\n\n");
}

void test_edge_cases(void)
{
    printf("Testing edge cases...\n");
    vector_header h = {0};

    // Test operations with invalid header
    assert(free_vector(&h) == ERR_INVALID_HEADER);
    assert(get(&h, 0) == ERR_INVALID_HEADER);
    assert(set(&h, 0, 5) == ERR_INVALID_HEADER);

    // Test null pointer handling
    assert(push_back(NULL, 5) == ERR_NULL);
    assert(insert(NULL, 0, 5) == ERR_NULL);

    printf("Passed!\n\n");
}

int main(void)
{
    test_initialization();
    test_basic_operations();
    test_insert_delete();
    test_capacity_management();
    test_edge_cases();

    printf("All tests passed successfully!\n");
    return 0;
}
