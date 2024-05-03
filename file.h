#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "log.h"

typedef struct FileData
{
    char *data;
    size_t size;

} FileData;

const FileData *read_file(const char *filepath);