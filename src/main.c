#include <stdio.h>
#include "include/vector.h"

int main(void) {
    vector_header header = init_vector(10);

    for (int i = 0; i < 100; ++i) {
        push_back(&header, i);
    }

    for (int i = 0; i < 100; ++i) {
        printf("%ld ", at(&header, i));
    }
}
