#include "file_utils.h"

FILE* open_file(const char* fPath, const char* mode)
{
    FILE* fStream = fopen(fPath, mode);
    return fStream;
}

void close_file(FILE* fStream)
{
    fclose(fStream);
}

String* load_file_into_buffer(FILE* fStream)
{
    fseek(fStream, 0, SEEK_END);
    u32 bufferSize = ftell(fStream);
    rewind(fStream);

    String* buffer = create_string(bufferSize + 1);
    fread(buffer->data, sizeof(char), bufferSize, fStream);

    buffer->length = bufferSize;
    buffer->capacity = bufferSize + 1;

    return buffer;
}
