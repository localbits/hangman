#ifndef POWERUPS_H_
#define POWERUPS_H_

#include "types.h"
#include "word_utils.h"
#include "hangman.h"
#include "string.h"
#include <stdbool.h>

typedef struct PlayerState PlayerState;

typedef enum {
    POWERUP_MULTIPLE_GUESSES,
    POWERUP_COUNT
} PowerupType;

typedef struct {
    PowerupType type;
    bool isActive;
    u32 amount;
} Powerup;

typedef struct {
    Powerup powerups[POWERUP_COUNT];
    u32 activeAmount;
} PowerupState;

bool is_powerup_active(PlayerState* p, PowerupType type);
void enable_powerup(PlayerState* p, PowerupType type, u32 amount);
void disable_powerup(PlayerState* p, PowerupType type);
void powerup_multiple_guesses(PlayerState* p, GameState* g);
void process_player_powerups(PlayerState* p, GameState* g);

#endif // POWERUPS_H_
