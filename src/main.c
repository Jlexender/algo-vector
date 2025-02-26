#include <stdio.h>
#include "include/vector.h"

int main(void) {
    vector_header header = init_vector(30);

    for (int i = 0; i < 100; ++i) {
        push_back(&header, i+1);
    }

    for (int i = 0; i < header.size; ++i) {
        printf("%ld ", get(&header, i));
    }
    puts("\n");

    
    return 0;
}
