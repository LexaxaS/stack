#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "stackfunc.hpp"

int main()
    {
    printf("Stack by Alexey3XL\n"
           "\n\n");

    Stack tststack = {};
    stackCreate(&tststack);

    for (size_t i = 0; i < 300; i++)
        stackPush(&tststack, i);

    for (size_t i = 0; i < tststack.size; i++)
        printf("<%d> %d ", i, tststack.dataptr[i]);
    // stackDumpTest_t(&tststack, 24);
    printf("\n");
    // *(tststack.dataptr + 128) = 5;
    elem_t val = 0;
    for (size_t i = 0; i < 100; i++)
        stackPop(&tststack, &val);
    stackDumpTest_t(&tststack, 24);
    for (size_t i = 0; i < tststack.size; i++)
        printf("<%d> %d ", i, tststack.dataptr[i]);
    printf("zaebis\n");
    stackDel(&tststack);
    return 0;
    }
