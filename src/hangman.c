#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "word_utils.h"
#include "io_utils.h"
#include "types.h"

bool game_won(u32 lettersFound, u32 underscores)
{
    return lettersFound == underscores;
}

bool game_lost(u32 playerHp)
{
    return playerHp == 0;
}

static u32 hash_char(char c)
{
    return tolower(c) - '0' - 49;
}

void parse_player_guess(String* word, String* playerWord, bool* usedLetters, char guess, u32* hp, u32* lettersFound)
{
    bool matchFound = false;
    u32 guessIdx = hash_char(guess);

    if (usedLetters[guessIdx]) {
        (*hp)--;
        printf("Letter already previously guessed!\n");
        return;
    }

    usedLetters[guessIdx] = true;

    for (u32 i = 0; i < word->length; ++i) {
        if (word->data[i] == guess) {
            playerWord->data[i] = guess;
            (*lettersFound)++;
            matchFound = true;
        }
    }

    if (!matchFound) {
        (*hp)--;
    }
}

void display_game_info(String* playerWord, u32 playerHp)
{
    printf("\n-- YOUR TURN --\n");
    printf("Your HP: %d\n", playerHp);
    printf("Your current word: %s\n", playerWord->data);
}

char game_turn(String* playerWord, u32 playerHp)
{
    char letter;
    display_game_info(playerWord, playerHp);
    printf("Type your guess: ");
    scanf(" %c", &letter);
    flush_stdin();
    clear_console();

    while (!isalpha(letter)) {
        printf("Provide a valid alphabet character\n");
        display_game_info(playerWord, playerHp);
        printf("Type your guess: ");
        scanf(" %c", &letter);
        flush_stdin();
        clear_console();
    }
    
    return letter;
}

bool hangman_game(String* word)
{
    bool usedLetters[26] = {false};
    printf("How many lives would you like? ");
    u32 playerHp;
    scanf("%d", &playerHp);
    clear_console();
    
    String* underscoreString = create_string(word->length);
    fill_string_with_char(underscoreString, '_');

    u32 underscores = underscoreString->length;
    u32 lettersFound = 0;

    while (!game_won(lettersFound, underscores)) {
        char c = game_turn(underscoreString, playerHp);
        parse_player_guess(word, underscoreString, usedLetters, c, &playerHp, &lettersFound);

        if (game_lost(playerHp)) {
            printf("Your current word: %s\n", underscoreString->data);
            printf("The original word was: %s\n", word->data);
            free_string(underscoreString);
            return false;
        }
    }
    
    printf("Your current word: %s\n", underscoreString->data);
    free_string(underscoreString);
    
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

    bool gameResult = hangman_game(word);
    display_outcome(gameResult);
    free_string(word);

    return 0;
}
