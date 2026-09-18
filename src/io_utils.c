#include "io_utils.h"

void flush_stdin(void)
{
    s32 c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void clear_console(void)
{
    #ifdef PLATFORM_WINDOWS
        system("cls");
    #else
        system("clear");
    #endif
}
