#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "word_utils.h"
#include "types.h"

bool game_won(u32 lettersFound, u32 underscores)
{
    return lettersFound == underscores;
}

bool game_lost(u32 playerHp)
{
    return playerHp == 0;
}

void clear_screen(void)
{
    #ifdef PLATFORM_WINDOWS
        system("cls");
    #else
        system("clear");
    #endif
}

char game_turn(String* str, u32 playerHp)
{
    char letter;
    printf("\n-- YOUR TURN --\n");
    printf("Your HP: %d\n", playerHp);
    printf("Your current word: %s\n", str->data);
    printf("Type your guess: ");
    scanf(" %c", &letter);
    clear_screen();
    
    return letter;
}

bool find_letter_matches(String* word, String* str, char letter, u32* lettersFound)
{
    bool matchFound = false;
    char* wordString = word->data;

    for (u32 i = 0; i < word->length; ++i) {
        if (wordString[i] == letter && str->data[i] == '_') {
            matchFound = true;
            str->data[i] = letter;
            (*lettersFound)++;
            continue;
        }

        if (wordString[i] == letter && str->data[i] == letter) {
            printf("Letter already previously guessed!\n");
            return false;
        }
    }

    return matchFound;
}

bool hangman_game(String* word)
{
    printf("How many lives would you like? ");
    u32 playerHp;
    scanf("%d", &playerHp);
    clear_screen();
    
    String* underscoreString = create_string(word->length);
    fill_string_with_char(underscoreString, '_');

    u32 underscores = underscoreString->length;
    u32 lettersFound = 0;

    while (!game_won(lettersFound, underscores)) {
        char c = game_turn(underscoreString, playerHp);
        bool matchFound = find_letter_matches(word, underscoreString, c, &lettersFound);

        if (!matchFound) {
            playerHp--;
        }

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
