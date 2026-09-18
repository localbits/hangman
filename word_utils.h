#ifndef WORD_UTILS_H_
#define WORD_UTILS_H_

#include "types.h"
#include "string.h"
#include "file_utils.h"

String* get_random_word_from_file(const char* fPath);
void fill_with_underscores(String* word);

#endif // WORD_UTILS_H_
