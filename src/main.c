#include <stdio.h>
#include "include/deamortized_vector.h"

int main(void)
{
    deamortized_vector_header header = init_deamortized_vector(30);

    for (int i = 0; i < 100; ++i)
    {
        deamortized_push_back(&header, i + 1);
    }

    for (int i = 0; i < get_size(&header); ++i)
    {
        printf("%ld ", deamortized_get(&header, i));
    }
    puts("\n");

    return 0;
}
