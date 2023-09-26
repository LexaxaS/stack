#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "stackfunc.hpp"

int stackCreate(Stack* stk, size_t capacity)
    {
    assert(stk);
    assert(capacity);
    stk->leftChicken = CHICKEN;
    stk->rightChicken = CHICKEN;
    stk->capacity = 8;
    stk->size = 0;
    stk->dataptr = (elem_t*) (calloc(2 * sizeof(size_t) + capacity * sizeof(elem_t), 1) + 8);
    size_t chicken = CHICKEN;
    memcpy((elem_t*)((char*) stk->dataptr - 8), &chicken, 8);
    memcpy((elem_t*)(stk->dataptr + 8), &chicken, 8);
    for (size_t i = stk->size; i < stk->capacity; i++)
        {
        stk->dataptr[i] = POISON;
        }
    assert(stk->dataptr);
    return 1;
    }

int stackRealloc(Stack* stk, size_t capacity)
    {
    printf("real , capa = <%d>\n", capacity);
    stk->dataptr = (elem_t*) (realloc((void*)stk->dataptr - sizeof(size_t), capacity * sizeof(elem_t) + 2 * sizeof(size_t)) + sizeof(size_t));
    printf("ptr = %p\n", stk->dataptr);
    stk->capacity = capacity;
    for (size_t i = stk->size + 1; i < stk->capacity; i++)
        {
        stk->dataptr[i] = POISON;
        }
    size_t chicken = CHICKEN;
    memcpy((elem_t*)(stk->dataptr + stk->capacity), &chicken, 8);
    return 1;
    }

int stackPush(Stack* stk, elem_t value)
    {
    assert(stk);
    error_t error = verifyStack(stk);
    //if (error == 0)
    //    stackDump(stk, 24, __FILE__, __func__, __LINE__, "stk");
    if (stk->capacity == stk->size)
        {
        printf("realpush \n");
        stackRealloc(stk, stk->capacity * 2);
        }
    stk->dataptr[stk->size++] = value;
    return 1;
    }

int stackPop(Stack* stk, elem_t* value)
    {
    printf("pop \n");
    *value = (stk->dataptr[stk->size - 1]);
    stk->dataptr[stk->size] = POISON;
    stk->size--;
    if (stk->size * 4 <= stk->capacity)
        {
        printf("realpop\n");
        stackRealloc(stk, stk->capacity / 2);
        }
    return 1;
    }

int stackDel(Stack* stk)
    {
    elem_t* dta = stk->dataptr;
    for (size_t i = 0; i < stk->size; i++)
        {
        dta[i] = POISON;
        }
    free(stk->dataptr);
    return 1;
    }

int verifyStack(Stack* stk)
    {
    error_t error = 0;
    if (stk->leftChicken != CHICKEN) {error |= LEFTCHICKENstackDIED;}    
    if (stk->rightChicken != CHICKEN) {error |= RIGHTCHICKENstackDIED;}   
    if (stk->size < 0) {error |= SIZENEGATIVE;}    
    if (stk->size > stk->capacity) {error |= SIZETOOBIG;}    
    if (stk->capacity < 0) {error |= CAPACITYNEGATIVE;}    
    if (ptrverify(stk->dataptr)) {error |= POINTERNOTVALID;}   
    return error;
    }

//called from file: {%s} from function: {%s}

int stackDump(Stack* stk, error_t error, char* vfile, const char* vfunc, int vline, /*char* cfile, char* cfunc, */ char* stackname)
    {
    fprintf(PRINTLOGS, "Stack[%p] \"%s\" from file: [%s] (%d) from function: [%s]\n", stk, stackname, vfile, vline, vfunc);
    fprintf(PRINTLOGS, "Error codes: ");
    size_t i = 128;
    while (error > 0)
        {
        if (error >= i)
            {
            fprintf(PRINTLOGS, "%d ", i);
            error -= i;
            }
        i /= 2;
        }
    fprintf(PRINTLOGS, "\n");
    fprintf(PRINTLOGS, "    {\n");
    fprintf(PRINTLOGS, "    size = %d\n", stk->size);
    fprintf(PRINTLOGS, "    capacity = %d\n", stk->capacity);
    fprintf(PRINTLOGS, "    LeftStackCanary = <%llx> (should be <%llx>)\n", stk->leftChicken, CHICKEN);
    fprintf(PRINTLOGS, "    RightStackCanary = <%llx> (should be <%llx>)\n", stk->rightChicken, CHICKEN);
    fprintf(PRINTLOGS, "    capacity = %d\n", stk->capacity);
    fprintf(PRINTLOGS, "    data[%p]\n", stk->dataptr);
    fprintf(PRINTLOGS, "        {\n");
    fprintf(PRINTLOGS, "        LeftarrayCanary = <%llx> (should be <%llx>)\n", *(size_t*)((char*)stk->dataptr - 8), CHICKEN);
    for (size_t i = 0; i < stk->capacity; i++)
        {
        if (i < stk->size)
            fprintf(PRINTLOGS, "        *[%d] = <%"prELEM_T">\n", i, (stk->dataptr[i]));
        else
            fprintf(PRINTLOGS, "         [%d] = <%"prELEM_T"> (POISON)\n", i, stk->dataptr[i]);
        }
    fprintf(PRINTLOGS, "        RightarrayCanary = <%llx> (should be <%llx>)\n", *(size_t*)((stk->dataptr) + stk->capacity), CHICKEN);
    fprintf(PRINTLOGS, "        }\n");
    fprintf(PRINTLOGS, "    }\n");
    return 1;
    }

int ptrverify(elem_t* ptr)
    {
    if (ptr)
        return 0;
    else
        return 1;
    }

