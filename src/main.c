#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "include/deamortized_vector.h"
#include "include/vector.h"

void test_vector(void) {
    puts("vector.h test");
    vector_header header = init_vector(30);

    operation_result error;
    int total_errors = 0;
    for (int i = 0; i < 100; ++i)
    {
        error = push_back(&header, i + 1);
        if (error != OK) {
            printf("ERROR: %u\n", error);
            total_errors++;
        }
    }

    printf("Total errors: %d\n", total_errors);

    for (int i = 0; i < header.size; ++i)
    {
        printf("%ld ", get(&header, i));
    }
    puts("\n");
}

void test_push_back_and_get(void) {
    puts("\n=== Testing push_back and get operations ===");
    deamortized_vector_header header = init_deamortized_vector(5);
    
    printf("Testing push_back with small initial capacity... ");
    operation_result error;
    int total_errors = 0;
    for (int i = 0; i < 20; ++i) {
        error = deamortized_push_back(&header, i * 10);
        if (error != OK) {
            printf("ERROR at index %d: %u\n", i, error);
            total_errors++;
        }
    }
    printf("Done. Total errors: %d\n", total_errors);
    
    // Test size functionality
    printf("Size after insertions: %d\n", get_size(&header));
    
    // Test get functionality
    printf("Testing get functionality... ");
    int get_errors = 0;
    for (int i = 0; i < get_size(&header); ++i) {
        if (deamortized_get(&header, i) != i * 10) {
            printf("FAIL: Index %d has value %ld but expected %d\n", 
                   i, deamortized_get(&header, i), i * 10);
            get_errors++;
        }
    }
    if (get_errors == 0) {
        printf("PASS: All values correct\n");
    } else {
        printf("Total get errors: %d\n", get_errors);
    }
    
    // Free with error check
    error = free_deamortized_vector(&header);
    if (error != OK) {
        printf("ERROR freeing vector: %u\n", error);
    }
}

void test_set_operation(void) {
    puts("\n=== Testing set operation ===");
    deamortized_vector_header header = init_deamortized_vector(10);
    
    // First add some elements
    operation_result error;
    for (int i = 0; i < 10; ++i) {
        error = deamortized_push_back(&header, i);
        if (error != OK) {
            printf("ERROR adding element %d: %u\n", i, error);
        }
    }
    
    // Test set operation
    printf("Testing set operation... ");
    int set_errors = 0;
    for (int i = 0; i < get_size(&header); i += 2) {
        error = deamortized_set(&header, i, i * 100);
        if (error != OK) {
            printf("ERROR setting index %d: %u\n", i, error);
            set_errors++;
        }
    }
    
    // Verify values were set correctly
    int verify_errors = 0;
    for (int i = 0; i < get_size(&header); ++i) {
        long expected = (i % 2 == 0) ? i * 100 : i;
        if (deamortized_get(&header, i) != expected) {
            printf("FAIL: Index %d has value %ld but expected %ld\n", 
                   i, deamortized_get(&header, i), expected);
            verify_errors++;
        }
    }
    
    if (set_errors == 0 && verify_errors == 0) {
        printf("PASS: All set operations successful\n");
    } else {
        printf("Set errors: %d, Verification errors: %d\n", set_errors, verify_errors);
    }
    
    // Test out of bounds set
    error = deamortized_set(&header, 20, 999);
    printf("Setting out of bounds (index 20): %s\n", 
           error == OK ? "Unexpectedly succeeded" : "Failed as expected");
    
    // Free with error check
    error = free_deamortized_vector(&header);
    if (error != OK) {
        printf("ERROR freeing vector: %u\n", error);
    }
}

void test_insert_operation(void) {
    puts("\n=== Testing insert operation ===");
    deamortized_vector_header header = init_deamortized_vector(5);
    
    // First add some elements
    operation_result error;
    for (int i = 0; i < 5; ++i) {
        error = deamortized_push_back(&header, i * 10);
        if (error != OK) {
            printf("ERROR adding element %d: %u\n", i, error);
        }
    }
    
    printf("Initial vector: ");
    for (int i = 0; i < get_size(&header); ++i) {
        printf("%ld ", deamortized_get(&header, i));
    }
    printf("\n");
    
    // Test insert at beginning
    printf("Inserting at beginning... ");
    error = deamortized_insert(&header, 0, 999);
    if (error == OK && deamortized_get(&header, 0) == 999) {
        printf("PASS\n");
    } else {
        printf("FAIL: Error code %u\n", error);
    }
    
    // Test insert in middle
    printf("Inserting in middle... ");
    error = deamortized_insert(&header, 3, 888);
    if (error == OK && deamortized_get(&header, 3) == 888) {
        printf("PASS\n");
    } else {
        printf("FAIL: Error code %u\n", error);
    }
    
    // Test insert at end
    printf("Inserting at end... ");
    int end_idx = get_size(&header);
    error = deamortized_insert(&header, end_idx, 777);
    if (error == OK && deamortized_get(&header, end_idx) == 777) {
        printf("PASS\n");
    } else {
        printf("FAIL: Error code %u\n", error);
    }
    
    // Test insert out of bounds
    printf("Inserting out of bounds... ");
    error = deamortized_insert(&header, get_size(&header) + 5, 666);
    printf("%s\n", error == OK ? "Unexpectedly succeeded" : "Failed as expected");
    
    printf("Vector after insertions: ");
    for (int i = 0; i < get_size(&header); ++i) {
        printf("%ld ", deamortized_get(&header, i));
    }
    printf("\n");
    
    // Free with error check
    error = free_deamortized_vector(&header);
    if (error != OK) {
        printf("ERROR freeing vector: %u\n", error);
    }
}

void test_erase_operation(void) {
    puts("\n=== Testing erase operation ===");
    deamortized_vector_header header = init_deamortized_vector(10);
    
    // Add elements
    operation_result error;
    for (int i = 0; i < 10; ++i) {
        error = deamortized_push_back(&header, i * 10);
        if (error != OK) {
            printf("ERROR adding element %d: %u\n", i, error);
        }
    }
    
    printf("Initial vector: ");
    for (int i = 0; i < get_size(&header); ++i) {
        printf("%ld ", deamortized_get(&header, i));
    }
    printf("\n");
    
    // Test erase from beginning
    printf("Erasing from beginning... ");
    error = deamortized_erase(&header, 0);
    if (error == OK && get_size(&header) == 9 && deamortized_get(&header, 0) == 10) {
        printf("PASS\n");
    } else {
        printf("FAIL: Error code %u\n", error);
    }
    
    // Test erase from middle
    printf("Erasing from middle... ");
    error = deamortized_erase(&header, 4);
    if (error == OK && get_size(&header) == 8 && deamortized_get(&header, 4) == 60) {
        printf("PASS\n");
    } else {
        printf("FAIL: Error code %u\n", error);
    }
    
    // Test erase from end
    printf("Erasing from end... ");
    error = deamortized_erase(&header, get_size(&header) - 1);
    if (error == OK && get_size(&header) == 7) {
        printf("PASS\n");
    } else {
        printf("FAIL: Error code %u\n", error);
    }
    
    // Test erase out of bounds
    printf("Erasing out of bounds... ");
    error = deamortized_erase(&header, get_size(&header) + 5);
    printf("%s\n", error == OK ? "Unexpectedly succeeded" : "Failed as expected");
    
    printf("Vector after erasures: ");
    for (int i = 0; i < get_size(&header); ++i) {
        printf("%ld ", deamortized_get(&header, i));
    }
    printf("\n");
    
    // Free with error check
    error = free_deamortized_vector(&header);
    if (error != OK) {
        printf("ERROR freeing vector: %u\n", error);
    }
}

void test_pop_back_operation(void) {
    puts("\n=== Testing pop_back operation ===");
    deamortized_vector_header header = init_deamortized_vector(5);
    
    // Add elements
    operation_result error;
    for (int i = 0; i < 5; ++i) {
        error = deamortized_push_back(&header, i * 10);
        if (error != OK) {
            printf("ERROR adding element %d: %u\n", i, error);
        }
    }
    
    printf("Initial size: %d\n", get_size(&header));
    
    // Test pop_back
    printf("Testing pop_back... ");
    for (int i = 0; i < 3; ++i) {
        error = deamortized_pop_back(&header);
        if (error != OK) {
            printf("FAIL: Error code %u at iteration %d\n", error, i);
            break;
        }
    }
    
    if (error == OK && get_size(&header) == 2) {
        printf("PASS: Successfully removed 3 elements\n");
    }
    
    // Test pop_back on empty vector
    deamortized_vector_header empty_header = init_deamortized_vector(5);
    printf("Testing pop_back on empty vector... ");
    error = deamortized_pop_back(&empty_header);
    printf("%s\n", error == OK ? "Unexpectedly succeeded" : "Failed as expected");
    
    // Free with error check
    error = free_deamortized_vector(&header);
    if (error != OK) {
        printf("ERROR freeing header vector: %u\n", error);
    }
    
    error = free_deamortized_vector(&empty_header);
    if (error != OK) {
        printf("ERROR freeing empty vector: %u\n", error);
    }
}

void test_large_operations(void) {
    puts("\n=== Testing large-scale operations ===");
    deamortized_vector_header header = init_deamortized_vector(2);

    printf("Size: %d, CAP: %d\n", header.current_vector.size, header.current_vector.capacity);
    
    // Push a lot of elements
    printf("Pushing 10000 elements... ");
    operation_result error;
    int push_errors = 0;
    for (int i = 0; i < 10000; ++i) {
        error = deamortized_push_back(&header, i);
        if (error != OK) {
            push_errors++;
            if (push_errors < 5) {  // Only show first few errors
                printf("ERROR pushing element %d: %u\n", i, error);
            }
        }
    }
    printf("Done. Size: %d, Errors: %d\n", get_size(&header), push_errors);
    
    // Random insertions
    printf("Performing 1000 random insertions... ");
    int insert_errors = 0;
    for (int i = 0; i < 1000; ++i) {
        int pos = rand() % (get_size(&header) + 1);
        error = deamortized_insert(&header, pos, 10000 + i);
        if (error != OK) {
            insert_errors++;
            if (insert_errors < 5) {
                printf("ERROR inserting at position %d: %u\n", pos, error);
            }
        }
    }
    printf("Done. Size: %d, Errors: %d\n", get_size(&header), insert_errors);
    
    // Random erasures
    printf("Performing 100 random erasures... ");
    int erase_errors = 0;
    for (int i = 0; i < 100; ++i) {
        int pos = rand() % get_size(&header);
        error = deamortized_erase(&header, pos);
        if (error != OK) {
            erase_errors++;
            if (erase_errors < 5) {
                printf("ERROR erasing at position %d: %u\n", pos, error);
            }
        }
    }
    printf("Done. Size: %d, Errors: %d\n", get_size(&header), erase_errors);
    
    // Free with error check
    error = free_deamortized_vector(&header);
    if (error != OK) {
        printf("ERROR freeing vector: %u\n", error);
    }
}

void test_mixed_operations(void) {
    puts("\n=== Testing mixed operations ===");
    deamortized_vector_header header = init_deamortized_vector(10);
    
    // Reference array to validate correctness
    long* reference = malloc(1000 * sizeof(long));
    if (reference == NULL) {
        printf("ERROR: Failed to allocate reference array\n");
        return;
    }
    int ref_size = 0;
    
    operation_result error;
    int operation_errors = 0;
    
    // Perform 1000 random operations
    for (int i = 0; i < 1000; ++i) {
        int op = rand() % 5;  // 5 possible operations
        
        switch (op) {
            case 0:  // push_back
                {
                    long value = rand() % 1000;
                    error = deamortized_push_back(&header, value);
                    if (error == OK) {
                        reference[ref_size++] = value;
                    } else {
                        operation_errors++;
                        if (operation_errors < 5) {
                            printf("ERROR in push_back, iteration %d: %u\n", i, error);
                        }
                    }
                }
                break;
                
            case 1:  // pop_back
                if (ref_size > 0) {
                    error = deamortized_pop_back(&header);
                    if (error == OK) {
                        ref_size--;
                    } else {
                        operation_errors++;
                        if (operation_errors < 5) {
                            printf("ERROR in pop_back, iteration %d: %u\n", i, error);
                        }
                    }
                }
                break;
                
            case 2:  // insert
                if (ref_size < 999) {
                    int pos = rand() % (ref_size + 1);
                    long value = rand() % 1000;
                    error = deamortized_insert(&header, pos, value);
                    if (error == OK) {
                        // Shift reference array
                        for (int j = ref_size; j > pos; j--) {
                            reference[j] = reference[j-1];
                        }
                        reference[pos] = value;
                        ref_size++;
                    } else {
                        operation_errors++;
                        if (operation_errors < 5) {
                            printf("ERROR in insert at pos %d, iteration %d: %u\n", pos, i, error);
                        }
                    }
                }
                break;
                
            case 3:  // erase
                if (ref_size > 0) {
                    int pos = rand() % ref_size;
                    error = deamortized_erase(&header, pos);
                    if (error == OK) {
                        // Shift reference array
                        for (int j = pos; j < ref_size - 1; j++) {
                            reference[j] = reference[j+1];
                        }
                        ref_size--;
                    } else {
                        operation_errors++;
                        if (operation_errors < 5) {
                            printf("ERROR in erase at pos %d, iteration %d: %u\n", pos, i, error);
                        }
                    }
                }
                break;
                
            case 4:  // set
                if (ref_size > 0) {
                    int pos = rand() % ref_size;
                    long value = rand() % 1000;
                    error = deamortized_set(&header, pos, value);
                    if (error == OK) {
                        reference[pos] = value;
                    } else {
                        operation_errors++;
                        if (operation_errors < 5) {
                            printf("ERROR in set at pos %d, iteration %d: %u\n", pos, i, error);
                        }
                    }
                }
                break;
        }
    }
    
    printf("Performed 1000 random operations with %d errors\n", operation_errors);
    printf("Final size: %d (expected: %d)\n", get_size(&header), ref_size);
    
    // Verify correctness
    printf("Verifying vector state... ");
    int verify_errors = 0;
    if (get_size(&header) != ref_size) {
        verify_errors++;
    } else {
        for (int i = 0; i < ref_size; ++i) {
            if (deamortized_get(&header, i) != reference[i]) {
                verify_errors++;
                if (verify_errors < 5) {  // Show first few errors
                    printf("\nMismatch at index %d: expected %ld, got %ld", 
                           i, reference[i], deamortized_get(&header, i));
                }
            }
        }
    }
    
    if (verify_errors == 0) {
        printf("PASS: All elements match expected values\n");
    } else {
        printf("FAIL: %d verification errors\n", verify_errors);
    }
    
    free(reference);
    
    // Free with error check
    error = free_deamortized_vector(&header);
    if (error != OK) {
        printf("ERROR freeing vector: %u\n", error);
    }
}

void test_init_free(void) {
    puts("\n=== Testing initialization and freeing ===");
    
    // Test with valid capacity
    printf("Testing initialization with capacity 10... ");
    deamortized_vector_header header1 = init_deamortized_vector(10);
    if (get_size(&header1) == 0) {
        printf("PASS\n");
    } else {
        printf("FAIL: Initial size should be 0 but got %d\n", get_size(&header1));
    }
    
    // Free with error check
    operation_result error = free_deamortized_vector(&header1);
    if (error == OK) {
        printf("Free operation: PASS\n");
    } else {
        printf("Free operation: FAIL with error %u\n", error);
    }
    
    // Test with zero capacity
    printf("Testing initialization with capacity 0... ");
    deamortized_vector_header header2 = init_deamortized_vector(0);
    if (get_size(&header2) == 0) {
        printf("PASS\n");
    } else {
        printf("FAIL: Initial size should be 0 but got %d\n", get_size(&header2));
    }
    
    // Free with error check
    error = free_deamortized_vector(&header2);
    if (error == OK) {
        printf("Free operation: PASS\n");
    } else {
        printf("Free operation: FAIL with error %u\n", error);
    }
    
    // Test freeing NULL
    printf("Testing freeing NULL... ");
    deamortized_vector_header* null_header = NULL;
    error = free_deamortized_vector(null_header);
    printf("%s\n", error == OK ? "Unexpectedly succeeded" : "Failed as expected");
}

void test_deamortized_vector(void) {
    puts("deamortized_vector.h test");
    deamortized_vector_header header = init_deamortized_vector(30);

    operation_result error;
    int total_errors = 0;
    for (int i = 0; i < 100; ++i)
    {
        error = deamortized_push_back(&header, i + 1);
        if (error != OK) {
            printf("ERROR: %u\n", error);
            total_errors++;
        }
    }

    printf("Total errors: %d\n", total_errors);

    for (int i = 0; i < get_size(&header); ++i)
    {
        printf("%ld ", deamortized_get(&header, i));
    }
    puts("\n");
    
    // Free with error check
    error = free_deamortized_vector(&header);
    if (error != OK) {
        printf("ERROR freeing vector: %u\n", error);
    }
}

void test_vector_push_back_and_get(void) {
    puts("\n=== Testing vector push_back and get operations ===");
    vector_header header = init_vector(5);
    
    printf("Testing push_back with small initial capacity... ");
    operation_result error;
    int total_errors = 0;
    for (int i = 0; i < 20; ++i) {
        error = push_back(&header, i * 10);
        if (error != OK) {
            printf("ERROR at index %d: %u\n", i, error);
            total_errors++;
        }
    }
    printf("Done. Total errors: %d\n", total_errors);
    
    // Test size functionality
    printf("Size after insertions: %d\n", header.size);
    
    // Test get functionality
    printf("Testing get functionality... ");
    int get_errors = 0;
    for (int i = 0; i < header.size; ++i) {
        if (get(&header, i) != i * 10) {
            printf("FAIL: Index %d has value %ld but expected %d\n", 
                   i, get(&header, i), i * 10);
            get_errors++;
        }
    }
    if (get_errors == 0) {
        printf("PASS: All values correct\n");
    } else {
        printf("Total get errors: %d\n", get_errors);
    }
    
    // Free with error check
    error = free_vector(&header);
    if (error != OK) {
        printf("ERROR freeing vector: %u\n", error);
    }
}

void test_vector_set_operation(void) {
    puts("\n=== Testing vector set operation ===");
    vector_header header = init_vector(10);
    
    // First add some elements
    operation_result error;
    for (int i = 0; i < 10; ++i) {
        error = push_back(&header, i);
        if (error != OK) {
            printf("ERROR adding element %d: %u\n", i, error);
        }
    }
    
    // Test set operation
    printf("Testing set operation... ");
    int set_errors = 0;
    for (int i = 0; i < header.size; i += 2) {
        error = set(&header, i, i * 100);
        if (error != OK) {
            printf("ERROR setting index %d: %u\n", i, error);
            set_errors++;
        }
    }
    
    // Verify values were set correctly
    int verify_errors = 0;
    for (int i = 0; i < header.size; ++i) {
        long expected = (i % 2 == 0) ? i * 100 : i;
        if (get(&header, i) != expected) {
            printf("FAIL: Index %d has value %ld but expected %ld\n", 
                   i, get(&header, i), expected);
            verify_errors++;
        }
    }
    
    if (set_errors == 0 && verify_errors == 0) {
        printf("PASS: All set operations successful\n");
    } else {
        printf("Set errors: %d, Verification errors: %d\n", set_errors, verify_errors);
    }
    
    // Test out of bounds set
    error = set(&header, 20, 999);
    printf("Setting out of bounds (index 20): %s\n", 
           error == OK ? "Unexpectedly succeeded" : "Failed as expected");
    
    // Free with error check
    error = free_vector(&header);
    if (error != OK) {
        printf("ERROR freeing vector: %u\n", error);
    }
}

void test_vector_insert_operation(void) {
    puts("\n=== Testing vector insert operation ===");
    vector_header header = init_vector(5);
    
    // First add some elements
    operation_result error;
    for (int i = 0; i < 5; ++i) {
        error = push_back(&header, i * 10);
        if (error != OK) {
            printf("ERROR adding element %d: %u\n", i, error);
        }
    }
    
    printf("Initial vector: ");
    for (int i = 0; i < header.size; ++i) {
        printf("%ld ", get(&header, i));
    }
    printf("\n");
    
    // Test insert at beginning
    printf("Inserting at beginning... ");
    error = insert(&header, 0, 999);
    if (error == OK && get(&header, 0) == 999) {
        printf("PASS\n");
    } else {
        printf("FAIL: Error code %u\n", error);
    }
    
    // Test insert in middle
    printf("Inserting in middle... ");
    error = insert(&header, 3, 888);
    if (error == OK && get(&header, 3) == 888) {
        printf("PASS\n");
    } else {
        printf("FAIL: Error code %u\n", error);
    }
    
    // Test insert at end
    printf("Inserting at end... ");
    int end_idx = header.size;
    error = insert(&header, end_idx, 777);
    if (error == OK && get(&header, end_idx) == 777) {
        printf("PASS\n");
    } else {
        printf("FAIL: Error code %u\n", error);
    }
    
    // Test insert out of bounds
    printf("Inserting out of bounds... ");
    error = insert(&header, header.size + 5, 666);
    printf("%s\n", error == OK ? "Unexpectedly succeeded" : "Failed as expected");
    
    printf("Vector after insertions: ");
    for (int i = 0; i < header.size; ++i) {
        printf("%ld ", get(&header, i));
    }
    printf("\n");
    
    // Free with error check
    error = free_vector(&header);
    if (error != OK) {
        printf("ERROR freeing vector: %u\n", error);
    }
}

void test_vector_erase_operation(void) {
    puts("\n=== Testing vector erase operation ===");
    vector_header header = init_vector(10);
    
    // Add elements
    operation_result error;
    for (int i = 0; i < 10; ++i) {
        error = push_back(&header, i * 10);
        if (error != OK) {
            printf("ERROR adding element %d: %u\n", i, error);
        }
    }
    
    printf("Initial vector: ");
    for (int i = 0; i < header.size; ++i) {
        printf("%ld ", get(&header, i));
    }
    printf("\n");
    
    // Test erase from beginning
    printf("Erasing from beginning... ");
    error = erase(&header, 0);
    if (error == OK && header.size == 9 && get(&header, 0) == 10) {
        printf("PASS\n");
    } else {
        printf("FAIL: Error code %u\n", error);
    }
    
    // Test erase from middle
    printf("Erasing from middle... ");
    error = erase(&header, 4);
    if (error == OK && header.size == 8 && get(&header, 4) == 60) {
        printf("PASS\n");
    } else {
        printf("FAIL: Error code %u\n", error);
    }
    
    // Test erase from end
    printf("Erasing from end... ");
    error = erase(&header, header.size - 1);
    if (error == OK && header.size == 7) {
        printf("PASS\n");
    } else {
        printf("FAIL: Error code %u\n", error);
    }
    
    // Test erase out of bounds
    printf("Erasing out of bounds... ");
    error = erase(&header, header.size + 5);
    printf("%s\n", error == OK ? "Unexpectedly succeeded" : "Failed as expected");
    
    printf("Vector after erasures: ");
    for (int i = 0; i < header.size; ++i) {
        printf("%ld ", get(&header, i));
    }
    printf("\n");
    
    // Free with error check
    error = free_vector(&header);
    if (error != OK) {
        printf("ERROR freeing vector: %u\n", error);
    }
}

void test_vector_pop_back_operation(void) {
    puts("\n=== Testing vector pop_back operation ===");
    vector_header header = init_vector(5);
    
    // Add elements
    operation_result error;
    for (int i = 0; i < 5; ++i) {
        error = push_back(&header, i * 10);
        if (error != OK) {
            printf("ERROR adding element %d: %u\n", i, error);
        }
    }
    
    printf("Initial size: %d\n", header.size);
    
    // Test pop_back
    printf("Testing pop_back... ");
    for (int i = 0; i < 3; ++i) {
        error = pop_back(&header);
        if (error != OK) {
            printf("FAIL: Error code %u at iteration %d\n", error, i);
            break;
        }
    }
    
    if (error == OK && header.size == 2) {
        printf("PASS: Successfully removed 3 elements\n");
    }
    
    // Test pop_back on empty vector
    vector_header empty_header = init_vector(5);
    printf("Testing pop_back on empty vector... ");
    error = pop_back(&empty_header);
    printf("%s\n", error == OK ? "Unexpectedly succeeded" : "Failed as expected");
    
    // Free with error check
    error = free_vector(&header);
    if (error != OK) {
        printf("ERROR freeing header vector: %u\n", error);
    }
    
    error = free_vector(&empty_header);
    if (error != OK) {
        printf("ERROR freeing empty vector: %u\n", error);
    }
}

void test_vector_large_operations(void) {
    puts("\n=== Testing vector large-scale operations ===");
    vector_header header = init_vector(2);

    printf("Size: %d, Capacity: %d\n", header.size, header.capacity);
    
    // Push a lot of elements
    printf("Pushing 1000 elements... ");
    operation_result error;
    int push_errors = 0;
    for (int i = 0; i < 1000; ++i) {
        error = push_back(&header, i);
        if (error != OK) {
            push_errors++;
            if (push_errors < 5) {  // Only show first few errors
                printf("ERROR pushing element %d: %u\n", i, error);
            }
        }
    }
    printf("Done. Size: %d, Errors: %d\n", header.size, push_errors);
    
    // Random insertions
    printf("Performing 100 random insertions... ");
    int insert_errors = 0;
    for (int i = 0; i < 100; ++i) {
        int pos = rand() % (header.size + 1);
        error = insert(&header, pos, 10000 + i);
        if (error != OK) {
            insert_errors++;
            if (insert_errors < 5) {
                printf("ERROR inserting at position %d: %u\n", pos, error);
            }
        }
    }
    printf("Done. Size: %d, Errors: %d\n", header.size, insert_errors);
    
    // Random erasures
    printf("Performing 50 random erasures... ");
    int erase_errors = 0;
    for (int i = 0; i < 50; ++i) {
        int pos = rand() % header.size;
        error = erase(&header, pos);
        if (error != OK) {
            erase_errors++;
            if (erase_errors < 5) {
                printf("ERROR erasing at position %d: %u\n", pos, error);
            }
        }
    }
    printf("Done. Size: %d, Errors: %d\n", header.size, erase_errors);
    
    // Free with error check
    error = free_vector(&header);
    if (error != OK) {
        printf("ERROR freeing vector: %u\n", error);
    }
}

void test_vector_mixed_operations(void) {
    puts("\n=== Testing vector mixed operations ===");
    vector_header header = init_vector(10);
    
    // Reference array to validate correctness
    long* reference = malloc(500 * sizeof(long));
    if (reference == NULL) {
        printf("ERROR: Failed to allocate reference array\n");
        return;
    }
    int ref_size = 0;
    
    operation_result error;
    int operation_errors = 0;
    
    // Perform 500 random operations
    for (int i = 0; i < 500; ++i) {
        int op = rand() % 5;  // 5 possible operations
        
        switch (op) {
            case 0:  // push_back
                {
                    long value = rand() % 1000;
                    error = push_back(&header, value);
                    if (error == OK) {
                        reference[ref_size++] = value;
                    } else {
                        operation_errors++;
                        if (operation_errors < 5) {
                            printf("ERROR in push_back, iteration %d: %u\n", i, error);
                        }
                    }
                }
                break;
                
            case 1:  // pop_back
                if (ref_size > 0) {
                    error = pop_back(&header);
                    if (error == OK) {
                        ref_size--;
                    } else {
                        operation_errors++;
                        if (operation_errors < 5) {
                            printf("ERROR in pop_back, iteration %d: %u\n", i, error);
                        }
                    }
                }
                break;
                
            case 2:  // insert
                if (ref_size < 499) {
                    int pos = rand() % (ref_size + 1);
                    long value = rand() % 1000;
                    error = insert(&header, pos, value);
                    if (error == OK) {
                        // Shift reference array
                        for (int j = ref_size; j > pos; j--) {
                            reference[j] = reference[j-1];
                        }
                        reference[pos] = value;
                        ref_size++;
                    } else {
                        operation_errors++;
                        if (operation_errors < 5) {
                            printf("ERROR in insert at pos %d, iteration %d: %u\n", pos, i, error);
                        }
                    }
                }
                break;
                
            case 3:  // erase
                if (ref_size > 0) {
                    int pos = rand() % ref_size;
                    error = erase(&header, pos);
                    if (error == OK) {
                        // Shift reference array
                        for (int j = pos; j < ref_size - 1; j++) {
                            reference[j] = reference[j+1];
                        }
                        ref_size--;
                    } else {
                        operation_errors++;
                        if (operation_errors < 5) {
                            printf("ERROR in erase at pos %d, iteration %d: %u\n", pos, i, error);
                        }
                    }
                }
                break;
                
            case 4:  // set
                if (ref_size > 0) {
                    int pos = rand() % ref_size;
                    long value = rand() % 1000;
                    error = set(&header, pos, value);
                    if (error == OK) {
                        reference[pos] = value;
                    } else {
                        operation_errors++;
                        if (operation_errors < 5) {
                            printf("ERROR in set at pos %d, iteration %d: %u\n", pos, i, error);
                        }
                    }
                }
                break;
        }
    }
    
    printf("Performed 500 random operations with %d errors\n", operation_errors);
    printf("Final size: %d (expected: %d)\n", header.size, ref_size);
    
    // Verify correctness
    printf("Verifying vector state... ");
    int verify_errors = 0;
    if (header.size != ref_size) {
        verify_errors++;
    } else {
        for (int i = 0; i < ref_size; ++i) {
            if (get(&header, i) != reference[i]) {
                verify_errors++;
                if (verify_errors < 5) {  // Show first few errors
                    printf("\nMismatch at index %d: expected %ld, got %ld", 
                           i, reference[i], get(&header, i));
                }
            }
        }
    }
    
    if (verify_errors == 0) {
        printf("PASS: All elements match expected values\n");
    } else {
        printf("FAIL: %d verification errors\n", verify_errors);
    }
    
    free(reference);
    
    // Free with error check
    error = free_vector(&header);
    if (error != OK) {
        printf("ERROR freeing vector: %u\n", error);
    }
}

void test_vector_init_free(void) {
    puts("\n=== Testing vector initialization and freeing ===");
    
    // Test with valid capacity
    printf("Testing initialization with capacity 10... ");
    vector_header header1 = init_vector(10);
    if (header1.size == 0) {
        printf("PASS\n");
    } else {
        printf("FAIL: Initial size should be 0 but got %d\n", header1.size);
    }
    
    // Check capacity
    printf("Checking capacity... ");
    if (header1.capacity == 32) {
        printf("PASS\n");
    } else {
        printf("FAIL: Capacity should be 32 but got %d\n", header1.capacity);
    }
    
    // Free with error check
    operation_result error = free_vector(&header1);
    if (error == OK) {
        printf("Free operation: PASS\n");
    } else {
        printf("Free operation: FAIL with error %u\n", error);
    }
    
    // Test with zero capacity
    printf("Testing initialization with capacity 0... ");
    vector_header header2 = init_vector(0);
    if (header2.size == 0) {
        printf("PASS\n");
    } else {
        printf("FAIL: Initial size should be 0 but got %d\n", header2.size);
    }
    
    // Free with error check
    error = free_vector(&header2);
    if (error == OK) {
        printf("Free operation: PASS\n");
    } else {
        printf("Free operation: FAIL with error %u\n", error);
    }
    
    // Test freeing NULL
    printf("Testing freeing NULL... ");
    vector_header* null_header = NULL;
    error = free_vector(null_header);
    printf("%s\n", error == OK ? "Unexpectedly succeeded" : "Failed as expected");
}

void run_vector_tests(void) {
    puts("\n====================");
    puts("RUNNING VECTOR TESTS");
    puts("====================\n");
    
    // Initialize and free test
    test_vector_init_free();
    
    // Basic operations tests
    test_vector_push_back_and_get();
    test_vector_set_operation();
    test_vector_insert_operation();
    test_vector_erase_operation();
    test_vector_pop_back_operation();
    
    // Stress tests
    test_vector_large_operations();
    test_vector_mixed_operations();
}

int main(void)
{
    // // Seed random number generator for consistent test results
    srand(42);

    run_vector_tests();
    
    // Original tests
    test_vector();
    test_deamortized_vector();
    
    // New initialization and free test
    test_init_free();
    
    // Tests for each operation with error checking
    test_push_back_and_get();
    test_set_operation();
    test_insert_operation();
    test_erase_operation();
    test_pop_back_operation();
    
    // Integration tests
    test_large_operations();
    test_mixed_operations();

    
    return 0;
}
