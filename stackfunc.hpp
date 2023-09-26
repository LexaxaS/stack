#ifndef STACKFUNC_HPP
#define STACKFUNC_HPP

#include <stdio.h>

typedef size_t elem_t;
typedef unsigned long long error_t;

#define POISON 123456789
#define CHICKEN 0xBADDED32
#define PRINTLOGS stdout
#define prELEM_T "d"

enum ERRORS 
    {LEFTCHICKENstackDIED = 1,
     RIGHTCHICKENstackDIED = 2,
     SIZENEGATIVE = 4,
     SIZETOOBIG = 8, 
     CAPACITYNEGATIVE = 16,
     POINTERNOTVALID = 32,
     LEFTCHICKENarrayDIED = 64,
     RIGHTCHICKENarrayDIED = 128};

struct Stack
    {
    size_t leftChicken;
    elem_t* dataptr;
    size_t capacity;
    size_t size;
    size_t rightChicken;
    };

int stackCreate(Stack* stk, size_t capacity);
int stackPush(Stack* stk, elem_t value);
int stackDel(Stack* stk);
int stackPop(Stack* stk, elem_t* value);
int stackDump(Stack* stk, error_t error, char* vfile, const char* vfunc, int vline, /*char* cfile, char* cfunc, */ char* stackname);
int verifyStack(Stack* stk);
int ptrverify(elem_t* ptr);

#endif