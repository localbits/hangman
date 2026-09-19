#ifndef WORD_UTILS_H_
#define WORD_UTILS_H_

#include "types.h"
#include "string.h"
#include "file_utils.h"
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

String* get_random_word_from_file(const char* fPath);
void fill_with_underscores(String* word);
void replace_string_char_at_idx(String* str, u32 idx, char c);
u32 hash_char(char c);

#endif // WORD_UTILS_H_
