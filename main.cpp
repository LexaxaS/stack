#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef size_t elem_t;
typedef unsigned long long error_t;

struct Stack
    {
    size_t leftChicken;
    elem_t* dataptr;
    size_t capacity;
    size_t size;
    size_t rightChicken;
    };

#define POISON 123456789
#define CHICKEN 101112
#define PRINTLOGS stdout
#define prELEM_T "d"

#define StackDump_t(stk)                \
    {                                   \
                                        \
    }


enum ERRORS 
    {LEFTCHICKENstackDIED = 1,
     RIGHTCHICKENstackDIED = 2,
     SIZENEGATIVE = 4,
     SIZETOOBIG = 8, 
     CAPACITYNEGATIVE = 16,
     POINTERNOTVALID = 32,
     LEFTCHICKENarrayDIED = 64,
     RIGHTCHICKENarrayDIED = 128};

int stackDump(Stack* stk, error_t error, char* vfile, const char* vfunc, int vline, /*char* cfile, char* cfunc, */ char* stackname);
int stackCreate(Stack* stk, size_t capacity);
int stackRealloc(Stack* stk, size_t capacity);
int stackPush(Stack* stk, elem_t value);
int stackDel(Stack* stk);
int stackPop(Stack* stk, elem_t* value);
int verifyStack(Stack* stk);
int ptrverify(elem_t* ptr);

int main()
    {
    Stack tststack = {};
    stackCreate(&tststack, 5);
    assert(tststack.dataptr);
    for (size_t i = 0; i < 5; i++)
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

int stackCreate(Stack* stk, size_t capacity)
    {
    assert(stk);
    assert(capacity);
    const size_t chicken = CHICKEN;
    stk->leftChicken = CHICKEN;
    stk->rightChicken = CHICKEN;
    stk->capacity = 8;
    stk->size = 0;
    stk->dataptr = (elem_t*) (calloc(24, 1) + 8);
    memcpy(stk->dataptr, &chicken, 8);
    memcpy(stk->dataptr + 8, &chicken, 8);
    for (size_t i = stk->size; i < stk->capacity; i++)
        {
        stk->dataptr[i] = POISON;
        }
    assert(stk->dataptr);
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
    fprintf(PRINTLOGS, "    size = %d;\n", stk->size);
    fprintf(PRINTLOGS, "    capacity = %d\n", stk->capacity);
    fprintf(PRINTLOGS, "    data[%p]\n", stk->dataptr);
    fprintf(PRINTLOGS, "        {\n");
    fprintf(PRINTLOGS, "        LeftCanary = <%"prELEM_T"> (should be "")", stk->dataptr);
    for (size_t i = 0; i < stk->capacity; i++)
        {
        if (i < stk->size)
            fprintf(PRINTLOGS, "       *[%d] = <%"prELEM_T">\n", i, (stk->dataptr[i]));
        else
            fprintf(PRINTLOGS, "        [%d] = <%"prELEM_T"> (POISON)\n", i, stk->dataptr[i]);
        }
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


int stackRealloc(Stack* stk, size_t capacity)
    {
    printf("real ");
    stk->dataptr = (elem_t*) realloc(stk->dataptr, capacity * sizeof(elem_t));
    printf("ptr = %p\n", stk->dataptr);
    stk->capacity = capacity;
    for (size_t i = stk->size + 1; i < stk->capacity; i++)
        {
        stk->dataptr[i] = POISON;
        }
    return 1;
    }

int stackPush(Stack* stk, elem_t value)
    {
    assert(stk);
    error_t error = verifyStack(stk);
    //if (error == 0)
    //    stackDump(stk, 24, __FILE__, __func__, __LINE__, "stk");
    if (stk->capacity == stk->size + 1)
        {
        printf("realpush \n");
        stackRealloc(stk, stk->capacity * 2);
        }
    stk->dataptr[stk->size++] = value;
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


