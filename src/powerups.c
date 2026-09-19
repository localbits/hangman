#include "powerups.h"
#include "entities.h"

bool is_powerup_active(PlayerState *p, PowerupType type)
{
    return p->powerupState.powerups[type].isActive;
}

void enable_powerup(PlayerState* p, PowerupType type, u32 amount)
{
    p->powerupState.powerups[type].isActive = true;
    p->powerupState.powerups[type].amount = amount;
    p->powerupState.activeAmount++;
}

void disable_powerup(PlayerState* p, PowerupType type)
{
    p->powerupState.powerups[type].isActive = false;
    p->powerupState.powerups[type].amount = 0;
    p->powerupState.activeAmount--;
}

PowerupType* get_active_powerups(PlayerState* p, u32* activeAmount)
{
    static PowerupType activePowerups[POWERUP_COUNT];
    for (u32 i = 0; i < POWERUP_COUNT; ++i) {
        if (p->powerupState.powerups[i].isActive) {
            activePowerups[i] = p->powerupState.powerups[i].type;
            (*activeAmount)++;
        }
    }

    return activePowerups;
}

void powerup_multiple_guesses(PlayerState* p, GameState* g)
{
    bool matchFound = false;
    bool* usedLetters = p->usedLetters;

    char* secretStr = g->secretWord->data;
    char* guessesStr = p->guesses->data;
    
    for (u32 i = 0; i < g->secretWord->length; ++i) {
        for (u32 j = 0; j < p->guesses->length; ++j) {
            char c = guessesStr[j];
            u32 guessIdx = hash_char(c);
            if (usedLetters[guessIdx]) {
                continue;
            }

            usedLetters[guessIdx] = true;
            if (secretStr[i] == c) {
                matchFound = true;
                replace_string_char_at_idx(g->playerWord, i, c);
                break;
            }
        }
    }

    if (!matchFound) {
        p->hp--;
    }
}

void process_player_powerups(PlayerState* p, GameState* g)
{
    for (u32 i = 0; i < POWERUP_COUNT; ++i) {
        Powerup powerup = p->powerupState.powerups[i];
        if (powerup.isActive) {
            switch (powerup.type) {
                case POWERUP_MULTIPLE_GUESSES:
                    powerup_multiple_guesses(p, g);
                    break;
                case POWERUP_COUNT:
                    break;
            }
        }
    }
}
