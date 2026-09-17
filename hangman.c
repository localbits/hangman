#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
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

String* get_random_word_from_file(const char* fPath)
{
    FILE* file = fopen(fPath, "r");
    if (file == NULL) {
        printf("Failed to open file %s", fPath);
        return NULL;
    }

    u32 wordCount = 0;
    while (true) {
        char c = fgetc(file);
        if (c == '\n') {
            wordCount++;
            continue;
        }
        if (c == EOF) {
            break;
        }
    }

    rewind(file);

    srand(time(NULL));
    u32 randomWordIndex = rand() % wordCount;

    u32 wordsRead = 0;
    while (wordsRead != randomWordIndex - 1) {
        char c = fgetc(file);
        if (c == '\n') {
            wordsRead++;
        }
    }
    
    FILE* mark = file;

    u32 wordSize = 0;
    while (true) {
        char c = fgetc(file);
        if (c == EOF || c == '\n') {
            break;
        }
        wordSize++;
    }

    String* string = (String*)malloc(sizeof(String));
    char* word = (char*)malloc(sizeof(char) * wordSize + 1);
    word[wordSize] = '\0';

    u32 i = 0;
    while (true) {
        char c = fgetc(mark);
        if (c == EOF || c == '\n') {
            break;
        }
        word[i] = c;
        i++;
    }

    string->data = word;
    string->length = wordSize;

    fclose(file);

    return string;
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
        printf("Provide a word list file as an argument\n");
        exit(EXIT_FAILURE);
    }

    String* word = get_random_word_from_file(argv[1]);

    u32 hp = 5;
    bool gameResult = hangman_game(word->data, hp);
    display_outcome(gameResult);
    free_string(word);

    return 0;
}
