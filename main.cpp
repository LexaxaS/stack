#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "stackfunc.hpp"

int main()
    {

    printf("Stack by Alexey3XL\n",
           "Bring me some beer pls <3\n"
           "\n\n\n");

    Stack tststack = {};
    stackCreate(&tststack);

    for (size_t i = 0; i < 100; i++)
        stackPush(&tststack, i);

    for (size_t i = 0; i < tststack.size; i++)
        printf("<%d> %d ", i, tststack.dataptr[i]);
    // stackDumpTest_t(&tststack, 24);
    printf("\n");
    elem_t val = 0;

    tststack.dataptr[127] = 1000;
    stackDumpTest_t(&tststack, 24);

    for (size_t i = 0; i < 100; i++)
        stackPop(&tststack, &val);
    for (size_t i = 0; i < tststack.size; i++)
        printf("<%d> %d ", i, tststack.dataptr[i]);
    printf("zaebis\n");
    stackDel(&tststack);
    return 0;
    }
