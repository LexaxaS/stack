#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "stackfunc.hpp"

int _stackRealloc(Stack* stk, size_t capacity);
int hashStack(Stack* stk);
hash_t hashCount(void* source, size_t sizeb);
const char* errorStr(int code);
int ptrverify(elem_t* ptr);



int hashStack(Stack* stk)
    {
    hash_t hashArr = hashCount((size_t*)((char*)stk->dataptr - 8), stk->capacity * sizeof(elem_t) + 2 * sizeof(size_t));
    stk->arrayhash = hashArr;
    stk->stackhash = 0;
    hash_t hashStk = hashCount(stk, sizeof(*stk));
    stk->stackhash = hashStk;
    return 0;
    }

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
    *(elem_t*)((char*) stk->dataptr - 8) = CHICKEN;
    *(elem_t*)(stk->dataptr + sizeof(size_t)) = CHICKEN;
    for (size_t i = stk->size; i < stk->capacity; i++)
        stk->dataptr[i] = POISON;
    stk->stackhash = hashCount((size_t*)((char*)stk->dataptr - 8), stk->capacity * sizeof(elem_t) + 2 * sizeof(size_t));
    hashStack(stk);
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
    *(elem_t*)(stk->dataptr + stk->capacity) = CHICKEN;
    hashStack(stk);
    stackDump_t(stk);
    return 1;
    }

int stackPush(Stack* stk, elem_t value)
    {
    stackDump_t(stk);
    if (stk->capacity == stk->size)
        _stackRealloc(stk, stk->capacity * 2);
    stk->dataptr[stk->size++] = value;
    hashStack(stk);
    stackDump_t(stk);
    return 1;
    }

int stackPop(Stack* stk, elem_t* value)
    {
    stackDump_t(stk);
    assert(value);
    if (stk->size * 4 <= stk->capacity)
        _stackRealloc(stk, stk->capacity / 2);
    *value = (stk->dataptr[stk->size - 1]);
    stk->dataptr[stk->size] = POISON;
    stk->size--;
    hashStack(stk);
    stackDump_t(stk);
    return 1;
    }

int stackDel(Stack* stk)
    {
    stackDump_t(stk);
    elem_t* dta = stk->dataptr;
    for (size_t i = 0; i < stk->size; i++)
        dta[i] = POISON;
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
    hash_t sthash = stk->stackhash;
    stk->stackhash = 0;
    if (hashCount(stk, sizeof(*stk)) != sthash) {error |= StackHashWrong;}
    stk->stackhash = sthash;
    if (hashCount((size_t*)((char*)stk->dataptr - 8), stk->capacity * sizeof(elem_t) + 2 * sizeof(size_t)) != stk->arrayhash) {error |= ArrayHashWrong;}
    return error;
    }

//called from file: {%s} from function: {%s}

// const char* varName(void* var)
//     {

//     return 1;
//     }

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
        CASE_ (StackHashWrong)
        CASE_ (ArrayHashWrong)
        }

    return "**UNKNOWN**";

    #undef CASE_
    }

int stackDump(Stack* stk, error_t error, char* vfile, const char* vfunc, int vline, /*char* cfile, char* cfunc, */ char* stackname)
    {
    fprintf(PRINTLOGS, "\n\nStack[%p] \"%s\" from file: [%s] (%d) from function: [%s]\n", stk, stackname, vfile, vline, vfunc);
    fprintf(PRINTLOGS, "Error codes: ");
    size_t i = 1024;
    while (error > 0)
        {
        if (error >= i)
            {
            fprintf(PRINTLOGS, "#%d (%s) ", i, errorStr(i));
            error -= i;
            }
        i /= 2;
        }
    fprintf(PRINTLOGS, "\n");
    fprintf(PRINTLOGS, "stackHash = \"%llu\"\n", stk->stackhash);
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
    exit(0);
    return 1;
    }

int ptrverify(elem_t* ptr)
    {
    if (ptr)
        return 0;
    else
        return 1;
    }

