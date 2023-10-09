#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "stackfunc_internal.hpp"
#include "file_opener.hpp"
#include "stackfunc.hpp"


int hashingStack(Stack* stk)
    {
    hash_t hashArr = hashCount((size_t*)((char*)stk->dataptr - sizeof(chicken_t)), stk->capacity * sizeof(elem_t) + 2 * sizeof(chicken_t));
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

int _stackRealloc(Stack* stk, size_t capacity)
    {
    stackDump_t(stk);

    stk->dataptr = (elem_t*) (realloc((void*)stk->dataptr - sizeof(chicken_t), capacity * sizeof(elem_t) + 2 * sizeof(chicken_t)) + sizeof(chicken_t));
    
    stk->capacity = capacity;
    for (size_t i = stk->size + 1; i < stk->capacity; i++)
        {
        stk->dataptr[i] = POISON;
        }
    *(elem_t*)((void*)stk->dataptr + sizeof(elem_t) * stk->capacity) = CHICKEN;

    hashingStack(stk);
    stackDump_t(stk);
    return 1;
    }

int ptrverify(elem_t* ptr)
    {
    if (ptr)
        return 0;
    else
        return 1;
    }

int stackCreate(Stack* stk)
    {
    assert(stk);

    stk->leftChicken = CHICKEN;
    stk->rightChicken = CHICKEN;
    stk->capacity = 8;
    stk->size = 0;
    stk->dataptr = (elem_t*) (calloc(2 * sizeof(chicken_t) + stk->capacity * sizeof(elem_t), 1) + sizeof(chicken_t));
    *(elem_t*)((char*) stk->dataptr - sizeof(chicken_t)) = CHICKEN;
    *(elem_t*)(stk->dataptr + sizeof(chicken_t)) = CHICKEN;
    for (size_t i = stk->size; i < stk->capacity; i++)
        stk->dataptr[i] = POISON;
    hashingStack(stk);
    stackDump_t(stk);
    return 1;
    }


int stackPush(Stack* stk, elem_t value)
    {
    stackDump_t(stk);

    if (stk->capacity == stk->size)
        _stackRealloc(stk, stk->capacity * 2);
    stk->dataptr[stk->size++] = value;

    hashingStack(stk);
    stackDump_t(stk);
    return 1;
    }

int stackPop(Stack* stk, elem_t* value)
    {
    stackDump_t(stk);
    assert(value);

    if (stk->size * 4 <= stk->capacity & stk->capacity > 8)
        _stackRealloc(stk, stk->capacity / 2);
    *value = *(elem_t*)((void*)stk->dataptr + sizeof(elem_t) * (stk->size - 1));
    *(elem_t*)((void*)stk->dataptr + sizeof(elem_t) * (stk->size - 1)) = POISON;
    stk->size--;

    hashingStack(stk);
    stackDump_t(stk);
    return 1;
    }

int stackDel(Stack* stk)
    {
    stackDump_t(stk);

    elem_t* dta = stk->dataptr;
    for (size_t i = 0; i < stk->size; i++)
        dta[i] = POISON;
    free((void*)((char*) stk->dataptr - 8));
    return 1;
    }

int verifyStack(Stack* stk)
    {
    error_t error = 0;

    if (stk->leftChicken != CHICKEN)                                        {error |= LeftChickenStackDied;}    
    if (stk->rightChicken != CHICKEN)                                       {error |= RightChickenStackDied;}   
    if (*(elem_t*)((void*)stk->dataptr - sizeof(size_t)) != CHICKEN)        {error |= LeftChickenArrayDied;}    
    if (*(elem_t*)((stk->dataptr) + stk->capacity) != CHICKEN)              {error |= RightChickenArrayDied;} 

    if (stk->size < 0)                                                      {error |= SizeNegative;}    
    if (stk->size > stk->capacity)                                          {error |= SizeTooBig;}    
    if (stk->capacity < 0)                                                  {error |= CapacityNegative;}    
    if (ptrverify(stk->dataptr))                                            {error |= PointerNoValid;}   
    if (stk->capacity >= MAXCAPACITY)                                       {error |= ArrayTooBig;}

    hash_t sthash = stk->stackhash;
    stk->stackhash = 0;
    if (hashCount(stk, sizeof(*stk)) != sthash)                             {error |= StackHashWrong;}
    stk->stackhash = sthash;
    if (hashCount((size_t*)((char*)stk->dataptr - sizeof(chicken_t)), stk->capacity * sizeof(elem_t) + 2 * sizeof(chicken_t)) != stk->arrayhash)
                                                                            {error |= ArrayHashWrong;}

    return error;
    }


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



int stackDump(Stack* stk, error_t error, char* vfile, const char* vfunc, int vline, char* stackname, int abortneeded)
    {
    FILE* logspath = stderr;
    if (PRINTLOGS != "stderr")
        logspath = fileopenerW(PRINTLOGS);

    fprintf(logspath, "\n\nStack[%p] \"%s\" from file: [%s] (%d) from function: [%s]\n", stk, stackname, vfile, vline, vfunc);
    fprintf(logspath, "Error codes: ");
    size_t i = 1024;
    bool structerr = false;
    if ((1 << 10) & error > 0)
        {
        structerr = true;
        }
    while (error > 0)
        {
        if (error >= i)
            {
            fprintf(logspath, "#%d (%s) ", i, errorStr(i));
            error -= i;
            }
        i /= 2;
        }
    fprintf(logspath, "\n");
    fprintf(logspath, "stackHash = \"%llu\"\n", stk->stackhash);
    fprintf(logspath, "    {\n");
    fprintf(logspath, "    size = %d\n", stk->size);
    fprintf(logspath, "    capacity = %d\n", stk->capacity);
    fprintf(logspath, "    LeftStackCanary = <%llx> (should be <%llx>)\n", stk->leftChicken, CHICKEN);
    fprintf(logspath, "    RightStackCanary = <%llx> (should be <%llx>)\n", stk->rightChicken, CHICKEN);
    fprintf(logspath, "    capacity = %d\n", stk->capacity);
    fprintf(logspath, "    data[%p]\n", stk->dataptr);

    if (structerr == false)
        {
        fprintf(logspath, "        {\n");
        fprintf(logspath, "        LeftArrayCanary = <%llx> (should be <%llx>)\n", *(elem_t*)((char*)stk->dataptr - sizeof(chicken_t)), CHICKEN);

        if (stk->capacity >= 256)
            {
            for (size_t i = 0; i < 100; i++)
                {
                if (i < stk->size)
                    fprintf(logspath, "        *[%d] = <%"prELEM_T">\n", i, (stk->dataptr[i]));
                else
                    fprintf(logspath, "         [%d] = <%"prELEM_T"> (POISON)\n", i, stk->dataptr[i]);
                } 
            fprintf(logspath, "         ...\n");
            fprintf(logspath, "         %d more elements\n", stk->capacity - 128);
            fprintf(logspath, "         ...\n");
            for (size_t i = stk->capacity - 28; i < stk->capacity; i++)
                {
                if (i < stk->size)
                    fprintf(logspath, "        *[%d] = <%"prELEM_T">\n", i, (stk->dataptr[i]));
                else
                    fprintf(logspath, "         [%d] = <%"prELEM_T"> (POISON)\n", i, stk->dataptr[i]);
                }        
            }
        else
            for (size_t i = 0; i < stk->capacity; i++)
                {
                if (i < stk->size)
                    fprintf(logspath, "        *[%d] = <%"prELEM_T">\n", i, (stk->dataptr[i]));
                else
                    fprintf(logspath, "         [%d] = <%"prELEM_T"> (POISON)\n", i, stk->dataptr[i]);
                }
        fprintf(logspath, "        RightArrayCanary = <%llx> (should be <%llx>)\n", *(elem_t*)(stk->dataptr + stk->capacity), CHICKEN);
        fprintf(logspath, "        }\n");
        }
    else
        {
        fprintf(logspath, "    {Error in the structure, its dangerous ro print the array}");
        }
    fprintf(logspath, "    }\n");
    if (abortneeded)
        abort();
    return 1;
    }


