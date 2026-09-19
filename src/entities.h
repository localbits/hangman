#ifndef entities_h_
#define entities_h_

#include "types.h"
#include "powerups.h"

typedef struct PlayerState {
    u32 hp;
    u32 lettersFound;
    bool usedLetters[26];
    PowerupState powerupState;
    String* guesses;
} PlayerState;

#endif // entities_h_
