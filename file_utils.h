#ifndef FILE_UTILS_H_
#define FILE_UTILS_H_

#include "types.h"
#include "string.h"
#include <stdio.h>

FILE* open_file(const char* fPath, const char* mode);
void close_file(FILE* fStream);
String* load_file_into_buffer(FILE* fStream);

#endif // FILE_UTILS_H_
