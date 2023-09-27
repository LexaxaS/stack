#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "stackfunc.hpp"

int _stackRealloc(Stack* stk, size_t capacity);



hash_t hashCount(void* source, size_t sizeb)
    {
    hash_t hash = 5381;
    size_t c = 0;

    while (c < sizeb)
        {
        hash = ((hash << 5) + hash) + *(char*)(source + c);
        c++;
        }
    return hash;
    }

int stackCreate(Stack* stk)
    {
    assert(stk);
    stk->leftChicken = CHICKEN;
    stk->rightChicken = CHICKEN;
    stk->capacity = 8;
    stk->size = 0;
    stk->dataptr = (elem_t*) (calloc(2 * sizeof(size_t) + stk->capacity * sizeof(elem_t), 1) + 8);
    size_t chicken = CHICKEN;
    memcpy((elem_t*)((char*) stk->dataptr - 8), &chicken, 8);
    memcpy((elem_t*)(stk->dataptr + sizeof(size_t)), &chicken, 8);
    for (size_t i = stk->size; i < stk->capacity; i++)
        {
        stk->dataptr[i] = POISON;
        }
    assert(stk->dataptr);
    stackDump_t(stk);
    return 1;
    }

int _stackRealloc(Stack* stk, size_t capacity)
    {
    stackDump_t(stk);
    stk->dataptr = (elem_t*) (realloc((void*)stk->dataptr - sizeof(size_t), capacity * sizeof(elem_t) + 2 * sizeof(size_t)) + sizeof(size_t));
    stk->capacity = capacity;
    for (size_t i = stk->size + 1; i < stk->capacity; i++)
        {
        stk->dataptr[i] = POISON;
        }
    size_t chicken = CHICKEN;
    memcpy((elem_t*)(stk->dataptr + stk->capacity), &chicken, 8);
    stackDump_t(stk);
    return 1;
    }

int stackPush(Stack* stk, elem_t value)
    {
    stackDump_t(stk);
    if (stk->capacity == stk->size)
        {
        _stackRealloc(stk, stk->capacity * 2);
        }
    stk->dataptr[stk->size++] = value;
    return 1;
    stackDump_t(stk);
    }

int stackPop(Stack* stk, elem_t* value)
    {
    stackDump_t(stk);
    assert(value);
    *value = (stk->dataptr[stk->size - 1]);
    stk->dataptr[stk->size] = POISON;
    stk->size--;
    if (stk->size * 4 <= stk->capacity)
        {
        _stackRealloc(stk, stk->capacity / 2);
        }
    stackDump_t(stk);
    return 1;
    }

int stackDel(Stack* stk)
    {
    stackDump_t(stk);
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
    if (stk->leftChicken != CHICKEN) {error |= LeftChickenStackDied;}    
    if (stk->rightChicken != CHICKEN) {error |= RightChickenStackDied;}   
    if (stk->size < 0) {error |= SizeNegative;}    
    if (stk->size > stk->capacity) {error |= SizeTooBig;}    
    if (stk->capacity < 0) {error |= CapacityNegative;}    
    if (ptrverify(stk->dataptr)) {error |= PointerNoValid;}   
    if (*(size_t*)((char*)stk->dataptr - 8) != CHICKEN) {error |= LeftChickenArrayDied;}    
    if (*(size_t*)((stk->dataptr) + stk->capacity) != CHICKEN) {error |= RightChickenArrayDied;} 
    if (stk->capacity >= MAXCAPACITY) {error |= ArrayTooBig;}
    return error;
    }

//called from file: {%s} from function: {%s}

const char* errorStr(int code)
    {
    #define CASE_(code) case code: return #code;

    switch (code)
        {
        CASE_ (LeftChickenStackDied)
        CASE_ (RightChickenStackDied)
        CASE_ (SizeNegative)
        CASE_ (SizeTooBig)
        CASE_ (CapacityNegative)
        CASE_ (PointerNoValid)
        CASE_ (LeftChickenArrayDied)
        CASE_ (RightChickenArrayDied)
        }

    return "**UNKNOWN**";

    #undef CASE_
    }

int stackDump(Stack* stk, error_t error, char* vfile, const char* vfunc, int vline, /*char* cfile, char* cfunc, */ char* stackname)
    {
    fprintf(PRINTLOGS, "\n\nStack[%p] \"%s\" from file: [%s] (%d) from function: [%s]\n", stk, stackname, vfile, vline, vfunc);
    fprintf(PRINTLOGS, "Error codes: ");
    size_t i = 128;
    while (error > 0)
        {
        if (error >= i)
            {
            fprintf(PRINTLOGS, "#%d ", i);
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
