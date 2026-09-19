#ifndef HANGMAN_H_
#define HANGMAN_H_

#include "types.h"
#include "string.h"

typedef struct {
    String* secretWord;
    String* playerWord;
} GameState;

#endif // HANGMAN_H_
