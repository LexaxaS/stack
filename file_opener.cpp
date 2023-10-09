#include <stdio.h>
#include <stdlib.h>
#include <sys\stat.h>
#include <assert.h>
#include "file_opener.hpp"

FILE *fileopener(char filename[])
    {
    assert(filename);

    FILE *fileEO = fopen(filename, "rb");
    if (fileEO == NULL)
        {
        perror("FILE_ERROR\n");
        abort();
        }
    return fileEO;
    }

FILE *fileopenerW(char filename[])
    {
    assert(filename);

    FILE *fileEO = fopen(filename, "w");
    if (fileEO == NULL)
        {
        printf("FILE_ERROR\n");
        abort();
        }
    return fileEO;
    }

size_t fileLen(const char * file)
    {
    assert(file);
    
    struct stat st = {};
    if (stat(file, &st) == 0)
        return (size_t) st.st_size;
    return 0;
    }
