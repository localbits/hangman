#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "word_utils.h"
#include "io_utils.h"
#include "entities.h"
#include "types.h"

// temporary
#define MULTIPLE_GUESSES 2

bool game_won(PlayerState* p, u32 underscores)
{
    return p->lettersFound == underscores;
}

bool game_lost(PlayerState* p)
{
    return p->hp == 0;
}

void default_parse_guess(PlayerState* p, GameState* g)
{
    bool matchFound = false;
    bool* usedLetters = p->usedLetters;
    char guess = p->guesses->data[0];

    u32 guessIdx = hash_char(guess);
    if (usedLetters[guessIdx]) {
        p->hp--;
        printf("Letter already previously guessed!\n");
        return;
    }

    usedLetters[guessIdx] = true;

    for (u32 i = 0; i < g->secretWord->length; ++i) {
        if (g->secretWord->data[i] == guess) {
            g->playerWord->data[i] = guess;
            p->lettersFound++;
            matchFound = true;
        }
    }

    if (!matchFound) {
        p->hp--;
    }
}

void parse_player_guess(PlayerState* p, GameState* g)
{
    if (p->powerupState.activeAmount == 0) {
        default_parse_guess(p, g);
        return;
    }

    process_player_powerups(p, g);
}

void display_game_info(PlayerState* p, GameState* g)
{
    printf("\n-- YOUR TURN --\n");
    printf("Your HP: %d\n", p->hp);
    printf("Your current word: %s\n", g->playerWord->data);
}

void game_turn(PlayerState* p, GameState* g)
{
    if (p->powerupState.activeAmount == 0 || !p->powerupState.powerups[POWERUP_MULTIPLE_GUESSES].isActive) {
        char letter;
        display_game_info(p, g);
        printf("Type your guess: ");
        scanf(" %c", &letter);
        p->guesses->data[0] = letter;
        flush_stdin();
        clear_console();
        return;
    }

    for (u32 i = 0; i < MULTIPLE_GUESSES; ++i) {
        display_game_info(p, g);
        printf("You have the right to guess multiple letters!\n");
        char letter;
        printf("Type your guess: ");
        scanf(" %c", &letter);
        p->guesses->data[i] = letter;
        flush_stdin();
        clear_console();
    }
}

bool hangman_game(PlayerState* p, GameState* g)
{
    printf("How many lives would you like? ");
    scanf("%d", &p->hp);
    clear_console();
    
    g->playerWord = create_string(g->secretWord->length);
    p->guesses = create_string(MULTIPLE_GUESSES);

    fill_string_with_char(g->playerWord, '_');

    u32 underscores = g->playerWord->length;

    while (!game_won(p, underscores)) {
        game_turn(p, g);
        parse_player_guess(p, g);

        if (game_lost(p)) {
            printf("Your current word: %s\n", g->playerWord->data);
            printf("The original word was: %s\n", g->secretWord->data);
            free_string(g->playerWord);
            free_string(p->guesses);
            return false;
        }
    }
    
    printf("Your current word: %s\n", g->playerWord->data);
    
    free_string(g->playerWord);
    free_string(g->secretWord);
    free_string(p->guesses);

    return true;
}

void display_outcome(bool gameResult)
{
    printf(gameResult ? "You won!\n" : "You lost!\n");
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Provide a word list file as an argument\n");
        exit(EXIT_FAILURE);
    }

    PlayerState p = {0};
    p.powerupState.activeAmount = 1; 
    p.powerupState.powerups[POWERUP_MULTIPLE_GUESSES].isActive = true;
    enable_powerup(&p, POWERUP_MULTIPLE_GUESSES, 1);
    
    GameState g = {0};

    g.secretWord = get_random_word_from_file(argv[1]);

    bool gameResult = hangman_game(&p, &g);
    display_outcome(gameResult);

    return 0;
}
