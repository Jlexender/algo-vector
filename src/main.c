#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "include/vector.h"
#include "include/deamortized_vector.h"

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

void test_stress(void)
{
    printf("Testing stress...\n");
    vector_header h = init_vector(MIN_CAPACITY);

    // Stress test push_back
    for (int i = 0; i < STRESS_TEST_SIZE; i++)
    {
        assert(push_back(&h, i) == OK);
    }
    assert(h.size == STRESS_TEST_SIZE);

    // Stress test get
    for (int i = 0; i < STRESS_TEST_SIZE; i++)
    {
        assert(get(&h, i) == i);
    }

    // Stress test pop_back
    for (int i = 0; i < STRESS_TEST_SIZE; i++)
    {
        assert(pop_back(&h) == OK);
    }
    assert(h.size == 0);

    free_vector(&h);
    printf("Passed!\n\n");
}

void test_deamortized_vector_basic(void)
{
    printf("Testing deamortized vector...\n");

    // Test initialization
    deamortized_vector_header dh = init_deamortized_vector(TEST_CAPACITY);
    assert(dh.current_vector.capacity == TEST_CAPACITY);
    assert(dh.current_vector.size == 0);
    assert(dh.current_vector.is_allocated);
    assert(dh.next_vector.capacity == TEST_CAPACITY * 2);
    assert(dh.next_vector.size == 0);
    assert(dh.next_vector.is_allocated);
    assert(dh.reallocated_amount == 0);

    // Test push_back
    for (int i = 0; i < 5; i++)
    {
        assert(deamortized_push_back(&dh, i * 10) == OK);
        assert(dh.current_vector.size == i + 1);
    }

    // Test get
    for (int i = 0; i < 5; i++)
    {
        assert(deamortized_get(&dh, i) == i * 10);
    }

    // Test set
    assert(deamortized_set(&dh, 2, TEST_VALUE) == OK);
    assert(deamortized_get(&dh, 2) == TEST_VALUE);

    // Test insert
    assert(deamortized_insert(&dh, 2, 100) == OK);
    assert(deamortized_get(&dh, 2) == 100);
    assert(deamortized_get(&dh, 3) == TEST_VALUE);

    // Test erase
    assert(deamortized_erase(&dh, 2) == OK);
    assert(deamortized_get(&dh, 2) == TEST_VALUE);

    // Test pop_back
    assert(deamortized_pop_back(&dh) == OK);
    assert(dh.current_vector.size == 4);

    // Test invalid operations
    assert(deamortized_insert(&dh, 10, 10) == ERR_OUT_OF_BOUNDS);
    assert(deamortized_erase(&dh, 10) == ERR_OUT_OF_BOUNDS);
    assert(deamortized_pop_back(&dh) == OK);
    assert(deamortized_pop_back(&dh) == OK);
    assert(deamortized_pop_back(&dh) == OK);
    assert(deamortized_pop_back(&dh) == OK);
    assert(deamortized_pop_back(&dh) == ERR_OUT_OF_BOUNDS);

    free_deamortized_vector(&dh);
    printf("Passed!\n\n");
}



void vector_tests(void) {
    test_initialization();
    test_basic_operations();
    test_insert_delete();
    test_capacity_management();
    test_edge_cases();
    test_stress();
    printf("All vector tests passed!\n");
}

void test_deamortized_reallocation_process(void) {
    printf("Testing deamortized reallocation process...\n");
    
    // Initialize with small capacity to observe reallocations
    deamortized_vector_header dh = init_deamortized_vector(4);

    // MIN_CAPACITY thing
    assert(dh.current_vector.capacity == 32);
    assert(dh.next_vector.capacity == 64);
    
    // Fill current vector
    for(int i = 0; i < 4; i++) {
        assert(deamortized_push_back(&dh, i) == OK);
        assert(dh.reallocated_amount == 0); // No reallocation yet
    }
    
    // Trigger reallocation process
    assert(deamortized_push_back(&dh, 4) == OK);

    // not the time to reallocate
    assert(dh.reallocated_amount == 0);
    
    // Verify elements exist in both vectors during migration
    for(int i = 0; i <= dh.reallocated_amount; i++) {
        assert(deamortized_get(&dh, i) == i);
    }
    
    // Complete reallocation
    while(dh.reallocated_amount < dh.current_vector.size) {
        int prev_realloc = dh.reallocated_amount;
        assert(deamortized_push_back(&dh, dh.current_vector.size) == OK);
        assert(dh.reallocated_amount >= prev_realloc);
    }
    
    // Verify final state after full reallocation
    assert(dh.current_vector.capacity == 32);
    assert(dh.next_vector.capacity == 64);
    
    // Verify all elements
    for(int i = 0; i < 8; i++) {
        assert(deamortized_get(&dh, i) == i);
    }
    
    free_deamortized_vector(&dh);
    printf("Passed!\n\n");
}

void test_deamortized_complex_operations(void) {
    printf("Testing complex deamortized operations...\n");
    deamortized_vector_header dh = init_deamortized_vector(8);
    
    // Fill initial vector
    for(int i = 0; i < 10; i++) {
        assert(deamortized_push_back(&dh, i) == OK);
    }
    
    // Interleave insertions during reallocation
    assert(deamortized_insert(&dh, 5, 100) == OK);
    assert(deamortized_get(&dh, 5) == 100);
    assert(deamortized_get(&dh, 6) == 5); // Subsequent elements shifted
    
    // Modify elements during reallocation
    for(int i = 0; i < 5; i++) {
        assert(deamortized_set(&dh, i, i * 2) == OK);
    }
    
    // Verify mixed access
    for(int i = 0; i < 5; i++) {
        assert(deamortized_get(&dh, i) == i * 2);
    }
    for(int i = 5; i < get_size(&dh); i++) {
        assert(deamortized_get(&dh, i) == (i < 6 ? 100 : i - 1));
    }
    
    // Force multiple reallocations
    for(int i = 0; i < 1000; i++) {
        assert(deamortized_push_back(&dh, i) == OK);
    }
    assert(get_size(&dh) == 1011);
    
    // Verify capacity growth pattern
    assert(dh.current_vector.capacity >= 1024);
    
    // Test front/middle/back deletions during reallocation
    assert(deamortized_erase(&dh, 0) == OK);
    assert(deamortized_erase(&dh, 500) == OK);
    assert(deamortized_erase(&dh, get_size(&dh)-1) == OK);
    
    free_deamortized_vector(&dh);
    printf("Passed!\n\n");
}

void test_deamortized_edge_cases(void) {
    printf("Testing deamortized edge cases...\n");
    
    // Test empty vector operations
    deamortized_vector_header dh = init_deamortized_vector(4);
    assert(deamortized_pop_back(&dh) == ERR_OUT_OF_BOUNDS);
    assert(deamortized_erase(&dh, 0) == ERR_OUT_OF_BOUNDS);
    
    // Test partial reallocation access
    for(int i = 0; i < 5; i++) {
        deamortized_push_back(&dh, i);
    }
    assert(deamortized_get(&dh, 4) == 4);
    assert(deamortized_get(&dh, 5) == ERR_OUT_OF_BOUNDS);
    
    // Test invalid header operations
    deamortized_vector_header invalid = {0};
    assert(deamortized_push_back(&invalid, 5) == ERR_INVALID_HEADER);
    assert(free_deamortized_vector(&invalid) == ERR_INVALID_HEADER);
    
    // Test null pointer handling
    assert(deamortized_push_back(NULL, 5) == ERR_NULL);
    assert(deamortized_get(NULL, 0) == ERR_NULL);
    
    free_deamortized_vector(&dh);
    printf("Passed!\n\n");
}

void test_deamortized_capacity_management(void) {
    printf("Testing deamortized capacity management...\n");
    deamortized_vector_header dh = init_deamortized_vector(2);
    
    // Verify initial capacity progression
    assert(dh.current_vector.capacity == 32);
    assert(dh.next_vector.capacity == 64);
    
    // Trigger first reallocation
    deamortized_push_back(&dh, 1);
    deamortized_push_back(&dh, 2);
    deamortized_push_back(&dh, 3); // Trigger realloc
    assert(dh.next_vector.capacity == 64);
    
    // Complete reallocation
    while(dh.reallocated_amount > 0) {
        deamortized_push_back(&dh, dh.current_vector.size);
    }
    assert(dh.current_vector.capacity == 32);
    assert(dh.next_vector.capacity == 64);
    
    // Verify size/capacity ratio after multiple reallocations
    for(int i = 0; i < 100; i++) {
        deamortized_push_back(&dh, i);
    }
    assert(dh.current_vector.capacity == 128);
    assert(dh.next_vector.capacity == 256);
    
    free_deamortized_vector(&dh);
    printf("Passed!\n\n");
}

void deamortized_vector_tests(void) {
    test_deamortized_vector_basic();
    test_deamortized_reallocation_process();
    test_deamortized_complex_operations();
    test_deamortized_edge_cases();
    test_deamortized_capacity_management();
    printf("All deamortized vector tests passed!\n");
}

int main(void)
{
    vector_tests();
    deamortized_vector_tests();

    printf("All tests passed successfully!\n");
    return 0;
}
