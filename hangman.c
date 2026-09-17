#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "types.h"

typedef struct {
    char* data;
    u32 length;
} String;

String* dashed_string(const char* word)
{
    String* string = (String*)malloc(sizeof(String));
    u32 wordLen = strlen(word);
    char* dashedString = (char*)malloc(sizeof(char) * wordLen + 1);

    for (u32 i = 0; i < wordLen; ++i) {
        dashedString[i] = '-';
    }

    dashedString[wordLen] = '\0';

    string->length = wordLen;
    string->data = dashedString;

    return string;
}

void free_string(String* str)
{
    free(str->data);
    free(str);
    str = NULL;
}

bool game_won(u32 lettersFound, u32 dashes)
{
    return lettersFound == dashes;
}

bool game_lost(u32 playerHp)
{
    return playerHp == 0;
}

char game_turn(String* str, u32 playerHp)
{
    char letter;

    printf("\n-- YOUR TURN --\n");
    printf("Your HP: %d\n", playerHp);
    printf("Your current word: %s\n", str->data);
    printf("Type your guess: ");
    scanf(" %c", &letter);
    printf("\n");

    return letter;
}

bool find_letter_matches(const char* word, String* str, char letter, u32* lettersFound)
{
    bool matchFound = false;
    for (u32 i = 0; i < strlen(word); ++i) {
        if (word[i] == letter && str->data[i] == '-') {
            matchFound = true;
            str->data[i] = letter;
            (*lettersFound)++;
            continue;
        }

        if (word[i] == letter && str->data[i] == letter) {
            printf("Letter already previously guessed!\n");
            return false;
        }
    }

    return matchFound;
}

bool hangman_game(const char* word, u32 playerHp)
{
    String* dashedString = dashed_string(word);
    u32 dashes = dashedString->length;
    u32 lettersFound = 0;

    while (!game_won(lettersFound, dashes)) {
        char c = game_turn(dashedString, playerHp);
        bool matchFound = find_letter_matches(word, dashedString, c, &lettersFound);

        if (!matchFound) {
            playerHp--;
        }

        if (game_lost(playerHp)) {
            printf("Your current word: %s\n", dashedString->data);
            printf("The original word was: %s\n", word);
            free_string(dashedString);
            return false;
        }
    }
    
    printf("Your current word: %s\n", dashedString->data);
    free_string(dashedString);
    
    return true;
}

void display_outcome(bool gameResult)
{
    printf(gameResult ? "You won!\n" : "You lost!\n");
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("Provide a word to be guessed as an argument\n");
        exit(EXIT_FAILURE);
    }

    const char* word = argv[1];

    u32 hp = 5;
    bool gameResult = hangman_game(word, hp);
    display_outcome(gameResult);

    return 0;
}

