#include "word_utils.h"

String* get_random_word_from_file(const char* fPath)
{
    FILE* file = open_file(fPath, "rb");
    if (file == NULL) {
        return NULL;
    }

    String* buffer = load_file_into_buffer(file);

    srand(time(NULL));
    u32 wordCount = 0;

    for (u32 i = 0; i < buffer->length; ++i) {
        if (buffer->data[i] == '\n') {
            wordCount++;
        }
    }

    u32 randomWordIdx = rand() % wordCount;
    u32 wordsParsed = 0;
    u32 randomWordByte = 0;

    for (u32 i = 0; i < buffer->length; ++i) {
        if (wordsParsed == randomWordIdx - 1) {
            randomWordByte = i;
            break;
        }

        if (buffer->data[i] == '\n') {
            wordsParsed++;
        }
    }

    u32 wordLength = 0;

    for (u32 i = randomWordByte; buffer->data[i] != '\n'; ++i) {
        wordLength++;
    }

    String* word = create_string(wordLength);

    for (u32 i = randomWordByte, j = 0; i < randomWordByte + wordLength; ++i, ++j) {
        word->data[j] = buffer->data[i];
    }

    close_file(file);
    free_string(buffer);
    
    return word;
}

void fill_with_underscores(String* word)
{
    fill_string_with_char(word, '_');
}
