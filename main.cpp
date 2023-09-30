#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "stackfunc.hpp"

int main()
    {
    char h1[] = "Hello";
    char h2[] = "Hello!";
    //printf("hash(Hello) = [%llu], hash(Hello!) = [%llu]\n", hashCount(h1, sizeof(h1) - 1), hashCount(h2, sizeof(h2) - 1));
    Stack tststack = {};
    stackCreate(&tststack);

    for (size_t i = 0; i < 100; i++)
        stackPush(&tststack, i);

    for (size_t i = 0; i < tststack.size; i++)
        printf("<%d> %d ", i, tststack.dataptr[i]);
    printf("\n");
    // stackDumpTest_t(&tststack, 24);
    elem_t val = 0;
    for (size_t i = 0; i < 100; i++)
        stackPop(&tststack, &val);
    // stackDumpTest_t(&tststack, 24);
    for (size_t i = 0; i < tststack.size; i++)
        printf("<%d> %d ", i, tststack.dataptr[i]);
    printf("zaebis\n");
    return 0;
    }
