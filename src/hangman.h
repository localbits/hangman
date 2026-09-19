#ifndef HANGMAN_H_
#define HANGMAN_H_

#include "types.h"

typedef struct {
    u32 hp;
    u32 lettersFound;
    bool usedLetters[26];
} PlayerState;

#endif // HANGMAN_H_
