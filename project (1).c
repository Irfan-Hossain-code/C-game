#include "project.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>



Character *characters = NULL; // Dynamic allocation
int numCharacters = 0;
int maxCharacters = 10;

void freeCharacter(Character *character) {
    if (character != NULL && !character->is_freed) {
        if (character->name != NULL){
            free(character->name);
        }
        if (character->weapon_name != NULL){
            free(character->weapon_name);
        }
        character->is_freed = true; // Set the flag to true after freeing memory
    }
}
void Quit() {
    // Free memory with function
    for (int i = 0; i < numCharacters; i++) {
        freeCharacter(&characters[i]);
    }
    // Free memory
    free(characters);
    numCharacters = 0;
    maxCharacters = 10;
}
void AddCommand(const char *name, int hp, const char *weapon_name, int weapon_damage) {
    // Check
    for (int i = 0; i < numCharacters; i++) {
        if (characters[i].name != NULL && strcmp(characters[i].name, name) == 0) {
            printf("Fighter '%s' already exists.\n", name);
            return;
        }
    }
    if (name == NULL || weapon_name == NULL || strlen(name) == 0 || strlen(weapon_name) == 0) {
        printf("Invalid command\n");
        return;
    }
    
    // Check
    if (weapon_damage < 0 || hp < 0) {
        printf("Invalid command\n");
        return;
    }
    // Dynamic memory
    Character *new_character = malloc(sizeof(Character));
    if (new_character == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }
    new_character->name = malloc(strlen(name) + 1); // +1 for the null terminator
    if (new_character->name == NULL) {
        printf("Memory allocation failed.\n");
        free(new_character);
        return;
    }
    new_character->weapon_name = malloc(strlen(weapon_name) + 1); // +1 for the null terminator
    if (new_character->weapon_name == NULL) {
        printf("Memory allocation failed.\n");
        freeCharacter(new_character); // Free memory using function
        free(new_character);
        return;
    }

    strcpy(new_character->name, name);
    strcpy(new_character->weapon_name, weapon_name);
    new_character->hp = hp;
    new_character->weapon_damage = weapon_damage;
    new_character->exp = 0;
    new_character->is_freed = false; // Flag to false

    // Check if more memory needs to be allocated for characters
    if (numCharacters == maxCharacters) {
        maxCharacters += 10; 
        characters = realloc(characters, maxCharacters * sizeof(Character));
        if (characters == NULL) {
            printf("Memory reallocation failed.\n");
            freeCharacter(new_character); // Free memory using function
            return;
        }
    }

    // Add the new character
    characters[numCharacters] = *new_character;
    numCharacters++;
    free(new_character);

    printf("SUCCESS\n");
}

void AttackCommand(Character *attacker, Character *attacked) {
    // Check
    if (attacker != NULL && attacked != NULL) {
        // Checking if both attacker and attacked are alive
        if (attacker->hp > 0 && attacked->hp > 0) {
            // Modifying attacked
            if (attacker->weapon_damage >= attacked->hp) {
                attacked->hp = 0;
            } else {
                attacked->hp -= attacker->weapon_damage;
            }

            printf("%s attacked %s with %s by %d damage.\n", attacker->name, attacked->name,
                   attacker->weapon_name, attacker->weapon_damage);
            printf("%s has %d hit points remaining.\n", attacked->name, attacked->hp);

            // Modifying attacker
            attacker->exp += attacker->weapon_damage;
            printf("%s gained %d experience points.\n", attacker->name, attacker->weapon_damage);

            printf("SUCCESS\n");
        } else {
            // Check
            if (attacker->hp <= 0) {
                printf("%s is dead.\n", attacker->name);
            } else {
                printf("%s is dead.\n", attacked->name);
            }
        }
    } else {
        printf("Invalid command.\n");
    }
}


void ListCommand() {
    // Sort the characters
    qsort(characters, numCharacters, sizeof(Character), sorting);
    
    // Print characters
    for (int i = 0; i < numCharacters; i++) {
        printf("%s %d %d %s %d\n", characters[i].name, characters[i].hp,
               characters[i].exp, characters[i].weapon_name, characters[i].weapon_damage);
    }

    printf("SUCCESS\n");
}

void WriteCommand(const char *filename) {
    FILE *file = fopen(filename, "w");
    
    // Check
    if (file == NULL) {
        printf("Unable to open file %s for writing.\n", filename);
        return;
    }

    // Write data to file
    for (int i = 0; i < numCharacters; i++) {
        fprintf(file, "%s %d %d %s %d\n", characters[i].name, characters[i].hp,
                characters[i].exp, characters[i].weapon_name, characters[i].weapon_damage);
    }

    fclose(file);
    printf("SUCCESS\n");
}


void LoadCommand(const char *filename) {
    FILE *file = fopen(filename, "r");
    
    // Checking
    if (file == NULL) {
        printf("Unable to load file %s.\n", filename);
        return;
    }
    
    // Free memory
    for (int i = 0; i < numCharacters; i++) {
        freeCharacter(&characters[i]); 
    }
    free(characters); 

    // Reset numCharacters
    numCharacters = 0;

    // Allocate memory
    characters = malloc(maxCharacters * sizeof(Character));
    if (characters == NULL) {
        printf("Memory allocation failed.\n");
        fclose(file);
        return;
    }

    char line[1000];
    while (fgets(line, sizeof(line), file)) {
        char name[1000];
        char weapon_name[1000];
        int hp, exp, weapon_damage;
        if (sscanf(line, "%s %d %d %s %d", name, &hp, &exp, weapon_name, &weapon_damage) == 5){
            Character *newcharacter = malloc(sizeof(Character));
            if (newcharacter == NULL){
                printf("Memory allocation failed\n");
                fclose(file);
                return;
            }
            newcharacter->name = malloc(strlen(name) + 1);
            if (newcharacter->name == NULL){
                printf("Memory allocation failed\n");
                freeCharacter(newcharacter); // Free memory using function
                fclose(file);
                return;
            }
            newcharacter->weapon_name = malloc(strlen(weapon_name) + 1);
            if (newcharacter->weapon_name == NULL){
                printf("Memory allocation failed\n");
                freeCharacter(newcharacter); // Free memory using function
                free(newcharacter);
                fclose(file);
                return;
            }
            strcpy(newcharacter->name, name);
            strcpy(newcharacter->weapon_name, weapon_name);
            newcharacter->hp = hp;
            newcharacter->exp = exp;
            newcharacter->weapon_damage = weapon_damage;
            newcharacter->is_freed = false; // Flag to false

            // Add new character
            characters[numCharacters] = *newcharacter;
            numCharacters++;
            free(newcharacter);
        } else {
            printf("Invalid command\n");
            break;
        }
    }
    
    fclose(file);
    printf("SUCCESS\n");
}




int main(void) {
    char input[1000]; // Task allowed us to assume the max character limit is 1000
    char letter;
    char name1[1000], name2[1000]; // If max limit is 1000, names can be at most 1000
    int number1, number2;
    char filename[1000]; // If max limit is 1000, filename can be at most 1000
    
    characters = malloc(maxCharacters * sizeof(Character));
    if (characters == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }    

    while (1) {
        fgets(input, sizeof(input), stdin);

        if (input[0] == 'A') {
            if (sscanf(input, "%c %s %d %s %d", &letter, name1, &number1, name2, &number2) == 5) {
                AddCommand(name1, number1, name2, number2);
            } else {
                printf("Invalid command\n"); // Dealing with invalid inputs
            } 
        } else if (input[0] == 'H') {
            if (sscanf(input, "%c %s %s", &letter, name1, name2) == 3) {
                
                Character *attacker = NULL;
                Character *attacked = NULL;
                
                // Setting the characters to their roles
                for (int i = 0; i < numCharacters; i++) {
                    if (strcmp(characters[i].name, name1) == 0) {
                        attacker = &characters[i];
                    }
                    if (strcmp(characters[i].name, name2) == 0) {
                        attacked = &characters[i];
                    }
                }
                
                // Setting the conditions for attacking
                if (attacker == attacked) {
                    printf("Invalid attack.\n");
                } else if (attacker != NULL && attacked != NULL) {
                    AttackCommand(attacker, attacked);
                }
            } else {
                printf("Invalid command\n"); // Dealing with invalid inputs
            }
        } else if (input[0] == 'L') {
            if (sscanf(input,"%c", &letter) == 1){
                ListCommand();
            } else {
                printf("Invalid command\n"); // Dealing with invalid inputs
            }
        } else if (input[0] == 'W') {
            if (sscanf(input, "%c %s", &letter, filename) == 2){
                WriteCommand(filename);
            } else {
                printf("Invalid command\n"); // Dealing with invalid inputs
            }            
        } else if (input[0] == 'O') {
            if (sscanf(input, "%c %s", &letter, filename) == 2){
                LoadCommand(filename);
            } else {
                printf("Invalid command\n"); // Dealing with invalid inputs
            }
        } else if (input[0] == 'Q') {
            Quit(); // Free allocated memory before quitting
            printf("SUCCESS\n");
            return 0;
        } else {
            printf("Invalid command\n"); // Dealing with invalid inputs
        }
    }
    return 0;
}