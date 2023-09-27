#ifndef STACKFUNC_HPP
#define STACKFUNC_HPP

#include <stdio.h>

typedef size_t elem_t;
typedef unsigned long long error_t;
typedef unsigned long long hash_t;

#define POISON 123456789
#define MAXCAPACITY (SIZE_MAX / 2)
#define CHICKEN 0xBADDED32
#define PRINTLOGS stdout
#define prELEM_T "d"

#define stackDumpTest_t(stk, err)                                                               \
do                                                                                              \
{                                                                                               \
    stackDump(stk, err, __FILE__, __func__, __LINE__, "stk");                                   \
} while (0)         


#define stackDump_t(stk)                                                                        \
do                                                                                              \
{                                                                                               \
    error_t error = verifyStack(stk);                                                           \
    if (error != 0) {stackDump(stk, 24, __FILE__, __func__, __LINE__, "stk");};                 \
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
     ArrayTooBig = 256};

struct Stack
    {
    size_t leftChicken;
    elem_t* dataptr;
    size_t capacity;
    size_t size;
    size_t rightChicken;
    };

hash_t hashCount(void* source, size_t sizeb);
int stackCreate(Stack* stk);
int stackPush(Stack* stk, elem_t value);
int stackDel(Stack* stk);
int stackPop(Stack* stk, elem_t* value);
int stackDump(Stack* stk, error_t error, char* vfile, const char* vfunc, int vline, /*char* cfile, char* cfunc, */ char* stackname);
int verifyStack(Stack* stk);
int ptrverify(elem_t* ptr);

#endif