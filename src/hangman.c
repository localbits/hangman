#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "word_utils.h"
#include "io_utils.h"
#include "hangman.h"
#include "types.h"

bool game_won(PlayerState* p, u32 underscores)
{
    return p->lettersFound == underscores;
}

bool game_lost(PlayerState* p)
{
    return p->hp == 0;
}

static u32 hash_char(char c)
{
    return tolower(c) - '0' - 49;
}

void parse_player_guess(PlayerState* p, String* word, String* playerWord, char guess)
{
    bool matchFound = false;
    bool* usedLetters = p->usedLetters;

    u32 guessIdx = hash_char(guess);
    if (usedLetters[guessIdx]) {
        p->hp--;
        printf("Letter already previously guessed!\n");
        return;
    }

    usedLetters[guessIdx] = true;

    for (u32 i = 0; i < word->length; ++i) {
        if (word->data[i] == guess) {
            playerWord->data[i] = guess;
            p->lettersFound++;
            matchFound = true;
        }
    }

    if (!matchFound) {
        p->hp--;
    }
}

void display_game_info(String* playerWord, PlayerState* p)
{
    printf("\n-- YOUR TURN --\n");
    printf("Your HP: %d\n", p->hp);
    printf("Your current word: %s\n", playerWord->data);
}

char game_turn(String* playerWord, PlayerState* p)
{
    char letter;
    display_game_info(playerWord, p);
    printf("Type your guess: ");
    scanf(" %c", &letter);
    flush_stdin();
    clear_console();

    while (!isalpha(letter)) {
        printf("Provide a valid alphabet character\n");
        display_game_info(playerWord, p);
        printf("Type your guess: ");
        scanf(" %c", &letter);
        flush_stdin();
        clear_console();
    }
    
    return letter;
}

bool hangman_game(PlayerState* p, String* word)
{
    printf("How many lives would you like? ");
    scanf("%d", &p->hp);
    clear_console();
    
    String* playerWord = create_string(word->length);
    fill_string_with_char(playerWord, '_');

    u32 underscores = playerWord->length;

    while (!game_won(p, underscores)) {
        char c = game_turn(playerWord, p);
        parse_player_guess(p, word, playerWord, c);

        if (game_lost(p)) {
            printf("Your current word: %s\n", playerWord->data);
            printf("The original word was: %s\n", word->data);
            free_string(playerWord);
            return false;
        }
    }
    
    printf("Your current word: %s\n", playerWord->data);
    free_string(playerWord);
    
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

    String* word = get_random_word_from_file(argv[1]);
    PlayerState p = {0};
    
    bool gameResult = hangman_game(&p, word);
    display_outcome(gameResult);
    free_string(word);

    return 0;
}
