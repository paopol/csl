#include <stdio.h>

// #define CSL_IMPLEMENTATION
#include "csl.h"


int main(int argc, char const *argv[])
{
    csl_unused_parameter(argc);
    csl_unused_parameter(argv);

    csl_log_write(csl_log_level_info, "Hello %s!", "C");

    printf("Hello, World\n");

    return 0;
}
