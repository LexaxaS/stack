#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "stackfunc.hpp"



int main()
    {
    Stack tststack = {};
    stackCreate(&tststack, 5);
    assert(tststack.dataptr);
    for (size_t i = 0; i < 100; i++)
        stackPush(&tststack, i);
    for (size_t i = 0; i < tststack.size; i++)
        printf("<%d> %d ", i, tststack.dataptr[i]);
    printf("\n");
    stackDump(&tststack, 24, __FILE__, __func__, __LINE__, "stk");
    elem_t val = 0;
    stackPop(&tststack, &val);
    printf("val = %d\n", val);
    for (size_t i = 0; i < tststack.size; i++)
        printf("<%d> %d ", i, tststack.dataptr[i]);
    return 0;
    }

