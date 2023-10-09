#ifndef F_O_H
#define F_O_H
#include <stdio.h>


FILE *fileopener(char filename[]);
FILE *fileopenerW(char filename[]);

size_t fileLen(const char * file);

#endif