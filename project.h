#ifndef _PROJECT__H_
#define _PROJECT__H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//Requirements of character creation
typedef struct {
    char *name;
    int hp;
    char *weapon_name;
    int weapon_damage;
    int exp;
    bool is_freed;
} Character;

//In order to sort characters based on their exp
int sorting(const void *a, const void *b) {
    const Character *charA = (const Character *)a;
    const Character *charB = (const Character *)b;
    return charB->exp - charA->exp;
}


void AddCommand(const char *name, int hp, const char *weapon_name, int weapon_damage); //Dealing with adding characters
void AttackCommand(Character *attacker, Character *attacked); //Dealing with attacks
void ListCommand(); //Creating a list based on exp
void WriteCommand(const char *filename); //Saving the game
void LoadCommand(const char *filename); //Loading a previously saved game
#endif //! _PROJECT__H_