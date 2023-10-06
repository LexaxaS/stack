#ifndef STACKFUNC_HPP
#define STACKFUNC_HPP

#include <stdio.h>

typedef unsigned long long error_t;
typedef unsigned long long hash_t;



#define POISON 123456789
#define MAXCAPACITY (SIZE_MAX / 2)
#define CHICKEN 0xBADDED32
#define PRINTLOGS stdout

#define INT 0
#define SIZE_T 1
#define DOUBLE 2
#define STR 3
#define CHAR 4

#define TYPE SIZE_T

#if (TYPE == INT)
    typedef int elem_t;
    #define prELEM_T "d"
#endif

#if (TYPE == SIZE_T)
    typedef size_t elem_t;
    #define prELEM_T "llu"
#endif

#if (TYPE == DOUBLE)
    typedef double elem_t;
    #define prELEM_T "lg"
#endif

#if (TYPE == STR)
    typedef char* elem_t;
    #define prELEM_T "s"
#endif


#if (TYPE == CHAR)
    typedef char elem_t;
    #define prELEM_T "c"
#endif

#ifndef TYPE
    typedef int elem_t;
    #define prELEM_T "d"
#endif

#define stackDumpTest_t(stk, err)                                                               \
do                                                                                              \
{                                                                                               \
    printf("\nTEST DUMP!!");                                                                    \
    stackDump(stk, err, __FILE__, __func__, __LINE__, #stk, 0);                                 \
} while (0)         


#define stackDump_t(stk)                                                                        \
do                                                                                              \
{                                                                                               \
    error_t error = verifyStack(stk);                                                           \
    if (error != 0) {stackDump(stk, error, __FILE__, __func__, __LINE__, #stk, 1);};            \ 
} while (0);    

enum ERRORS 
    {LeftChickenStackDied = 1,
     RightChickenStackDied = 2,
     SizeNegative = 4,
     SizeTooBig = 8, 
     CapacityNegative = 16,
     PointerNoValid = 32,
     LeftChickenArrayDied = 64,
     RightChickenArrayDied = 128,
     ArrayTooBig = 256,
     StackHashWrong = 512,
     ArrayHashWrong = 1024};

struct Stack
    {
    size_t leftChicken;
    elem_t* dataptr;
    size_t capacity;
    size_t size;
    hash_t arrayhash;
    hash_t stackhash;
    size_t rightChicken;
    };



hash_t hashCount(void* source, size_t sizeb);
int stackCreate(Stack* stk);
int stackPush(Stack* stk, elem_t value);
int stackDel(Stack* stk);
int stackPop(Stack* stk, elem_t* value);
int stackDump(Stack* stk, error_t error, char* vfile, const char* vfunc, int vline, /*char* cfile, char* cfunc, */ char* stackname, int extneeded);
int verifyStack(Stack* stk);

#endif