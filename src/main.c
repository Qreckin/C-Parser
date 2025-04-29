#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashmap.h"
#include "actions.h"
#include "structures.h"
#include "queries.h"
#include "helper_methods.h"

HashMap *ingredients = NULL; // Hashmap to store ingredients
HashMap *trophies = NULL; // Hashmap to store trophies
HashMap *potions = NULL; // Hashmap to store potions
HashMap *monsters = NULL; // Hashmap to store monsters

const int INITIAL_CAPACITY = 13; // Initial hashmap capacity

#define MIN_BREW 3 // Minimum number of words required in BREW action
#define MIN_LOOT 4 // Minimum number of words required in LOOT action
#define MIN_TRADE 8 // Minimum number of words required in TRADE action
#define ENCOUNTER 4 // Minimum number of words required in ENCOUNTER action
#define MIN_LEARN 8 // Minimum number of words required in LEARN action
#define MIN_TOTAL 2 // Minimum number of words required in TOTAL query
#define MIN_WHAT 4 // Minimum number of words required in WHAT IS IN query
#define MIN_EFFECTIVE 5 // Minimum number of words required in WHAT IS EFFECTIVE query
#define MIN_TOTAL_POTION 3 // Minimum number of words required in TOTAL POTION query
#define MIN_TOTAL_TROPHY 3 // Minimum number of words required in TOTAL TROPHY query





/**
 * @brief Execute the line of input. The function checks the command and calls the appropriate function.
 * @param arr The array of tokens
 * @param size The size of the array
 * @param input The original input line
 */
void execute_line(char **arr, int size, char *input){
    // Check if the first word is "Geralt", "Total" or "What" or INVALID

    // If the first word is "Geralt"
    if (strcmp(arr[0], "Geralt") == 0){
        // Check if the second word is "loots", "trades", "brews", "learns" or "encounters"

         // If the second word is "loots"
        if (strcmp(arr[1], "loots") == 0){
            if (size < MIN_LOOT){ // If command is not given correctly
                printf("INVALID\n");
                return;
            }
            
            int prefixCount = 2; // "Geralt loots" prefix size
            char **ingredientStart = &arr[prefixCount]; // Ingredients start from index 2

            if (!checkPairs(ingredientStart, size-prefixCount)){ // Check if the pairs are valid
                printf("INVALID\n");
                return;
            }

            PairArray *ingredientArray = constructPairArray(ingredientStart, size-prefixCount); // Construct the PairArray
            loot(ingredients, ingredientArray); // Execute LOOT action
            freePairArray(ingredientArray); // Free the PairArray
        }

        // If the second word is "trades"
        else if (strcmp(arr[1], "trades") == 0){
            if (size < MIN_TRADE){ // If command is not given correctly
                printf("INVALID\n");
                return;
            }
            
            int trophyIndex = -1;
            // Finds the index of the string "trophy"
            for (int i = 0; i < size; i++){
                if (strcmp(arr[i], "trophy") == 0){
                    trophyIndex = i;
                    break;
                }
            }

            // "trophy" not found or "for" not found
            if (trophyIndex == -1 || trophyIndex+1 >= size || strcmp(arr[trophyIndex+1], "for") != 0){
                printf("INVALID\n");
                return;
            }

            int prefix = 2;  // "Geralt trades" prefix size

            char **trophyStart = &arr[prefix];  // Trophy array starts from index 2
            int trophySize = trophyIndex - prefix;

            char **ingredientStart = &arr[trophyIndex+2];  // Ingredient array starts from trophyIndex + 2
            int ingredientSize = size - (trophyIndex + 2);  // +1 is to skip "for"

            if (!checkPairs(trophyStart, trophySize) || !checkPairs(ingredientStart, ingredientSize)){ // Check if the pairs are valid
                printf("INVALID\n");
                return;
            }

            PairArray *trophyArray = constructPairArray(trophyStart, trophySize);
            PairArray *ingredientArray = constructPairArray(ingredientStart, ingredientSize);

            trade(ingredients, trophies, ingredientArray, trophyArray); // Execute TRADE action

            // Free pair arrays
            freePairArray(trophyArray); 
            freePairArray(ingredientArray);

        }

        // If the second word is "brews"
        else if (strcmp(arr[1], "brews") == 0){
            if (size < MIN_BREW){ // If command is not given correctly
                printf("INVALID\n");
                return;
            }

            char *potionName;
            potionName = strstr(input, "brews") + strlen("brews"); // Points after "brews"

            while (*potionName == ' '){ // Skip whitespaces
                potionName++;
            }
            
            if (!isNameValid(potionName)){ // If potion name is not valid
                printf("INVALID\n");
                return;
            }

            brew(potions, ingredients, potionName); // Execute BREWS action
        }

        // If the second word is "learns"
        else if (strcmp(arr[1], "learns") == 0){
            if (size < MIN_LEARN){ // If command is not given correctly
                printf("INVALID\n");
                return;
            }
            
            char *prefix_consist = "consists";
            char *prefix_potion = "potion";


            int consistsIndex = findIndex(arr, size, prefix_consist); // Index of "consists" in arr
            int potionIndex = findIndex(arr, size, prefix_potion); // Index of "potion" in arr

            // "consists" token is available
            if (consistsIndex != -1){
                if (consistsIndex < 4 || strcmp(arr[consistsIndex-1], "potion") != 0){  // Consists is too early or "potion" does not come before "consists"
                    printf("INVALID\n");
                    return;
                }

                char *potionName = strstr(input, "learns") + strlen("learns"); // Points after "learns"

                while (*potionName == ' '){ // Skip whitespaces
                    potionName++;
                }

                char *potionEnd = strstr(input, "potion"); // Points to "potion"

                extractPotionName(potionName, potionEnd); // Extracts potion name

                if (!isNameValid(potionName)){ // Invalid name
                    printf("INVALID\n");
                    return;
                }

                if (size-consistsIndex < 3 || strcmp(arr[consistsIndex+1], "of") != 0){  // After consists at least 3 words must show up and "of" must come after it
                    printf("INVALID\n");
                    return;
                }

                int prefixCount = consistsIndex+2;  // Prefix before ingredient pairs
                char **ingredientStart = &arr[prefixCount];  // Ingredients start from index prefixCount

                if (!checkPairs(ingredientStart, size-prefixCount)){ // If pairs are invalid
                    printf("INVALID\n");
                    return;
                }

                PairArray *ingredientArray = constructPairArray(ingredientStart, size-prefixCount); // Create ingredient array

                learnPotionRecipe(potions, potionName, ingredientArray); // Execute LEARN RECIPE action

            }

            // "consists" token is not available -> learn sign or potion
            else{
                char *monsterName;
                // Learn sign
                if (strcmp(arr[3], "sign") == 0 && strcmp(arr[4], "is") == 0 && strcmp(arr[5], "effective") == 0 && strcmp(arr[6], "against") == 0 && size == MIN_LEARN){ // size must be exactly MIN_LEARN after "sign if effective against" 
                    char *signName = arr[2]; // Name of the sign
                    monsterName = arr[7]; // Name of the monster

                    if (!isNameValid(signName) || !isNameValid(monsterName)){ // Invalid names
                        printf("INVALID\n");
                        return;
                    }
                    
                    learnSign(monsters, monsterName, signName); // Execute LEARN SIGN action
                }

                // Learn potion
                else if (potionIndex != -1 && potionIndex+5==size && strcmp(arr[potionIndex+1], "is") == 0 && strcmp(arr[potionIndex+2], "effective") == 0 && strcmp(arr[potionIndex+3], "against") == 0){  // After potion name exactly 5 tokens must come -> "potion is effective against X"

                    char *potionName = strstr(input, "learns") + strlen("learns"); // Points after "learns"
                    monsterName = arr[size-1]; // Last word is the monster name

                    while (*potionName == ' '){  // Skip whitespaces
                        potionName++;
                    }

                    char *potionEnd = strstr(input, "potion"); // Points to "potion"
                    

                    extractPotionName(potionName, potionEnd); // Extract the potion name

                    if (!isNameValid(potionName) || !isNameValid(monsterName)){ // Invalid name
                        printf("INVALID\n");
                        return;
                    }      
                    
                    learnPotion(monsters, monsterName, potionName); // Execute LEARN POTION action

                }
                
                // Invalid structure
                else{
                    printf("INVALID\n");
                    return;
                }
            }
        }

        // If the second word is "encounters"
        else if (strcmp(arr[1], "encounters") == 0){
            if (size != ENCOUNTER || strcmp(arr[2], "a") != 0){ // Size must be exactly # of expected tokens and "a" must come after
                printf("INVALID\n");
                return;
            }


            char *monsterName = arr[size-1]; // Last word is the monster name

            if (!isNameValid(monsterName)){ // Invalid name
                printf("INVALID\n");
                return;
            }

            encounter(monsters, potions, trophies, monsterName); // Execute ENCOUNTER action
            return;
        }

        // After "Geralt", an invalid token came
        else{
            printf("INVALID\n");
            return;
        }
    }

    // If the first word is "Total"
    else if (strcmp(arr[0], "Total") == 0){
        if(size < 2){ //There is no question with 0 or 1 word.
            printf("INVALID\n");
            return;
        }
        if(size == 2){ // If size equals to 2, there is 2 options: input is invalid or the question mark is adjoining.     
            char *word = arr[1];     
            word[strlen(word)-1] = '\0'; // Make '?' null terminator

            if(strcmp(word, "ingredient") == 0){ // "Total ingredient?"
                allIngredients(ingredients);
            }
            else if(strcmp(word, "potion") == 0){ // "Total potion?"
                allPotions(potions);
            }
            else if(strcmp(word, "trophy") == 0){ // "Total trophy?"
                allTrophies(trophies);
            }
            else{ // Wrong structure
                printf("INVALID\n");
                return;
            }
        }

        //If the second word is "ingredient"
        else if(strcmp(arr[1], "ingredient") == 0){ 
            char *word = arr[2];

            if(size == 3){ // 1: Total ingredient query -or- 2: Specific ingredient query with adjacent question mark -or- 3: INVALID
                
                if (strcmp(word, "?") == 0){ // "Total ingredient ?"
                    allIngredients(ingredients);
                    return;
                }
                else if(word[strlen(word)-1] == '?'){ // "Total ingredient X?"
                    word[strlen(word)-1] = '\0'; // Remove ? from word 

                    if(!isNameValid(word)){ // Invalid name
                        printf("INVALID\n");
                        return;
                    }

                    specificIngredients(ingredients, word); // Execute SPECIFIC INGREDIENT query
                    return;
                }
                else{ // Wrong structure
                    printf("INVALID\n");
                    return;
                }
            }

            else if(size == 4){ //Input has to be specific ingredient with disjoint question mark
                char *qMark = arr[size-1]; // Retrieve question mark

                if(!isNameValid(word) || strcmp(qMark, "?") != 0){ // Name is invalid or last word is not question mark
                    printf("INVALID\n");
                    return;
                }

                specificIngredients(ingredients, word); // Execute SPECIFIC INGREDIENTS query
                return;
            }

            else{ // The size of the input with the second word "ingredient"s size can not be more than 4
                printf("INVALID\n");
                return;
            }
        }

        // If the second word is "potion"
        else if(strcmp(arr[1], "potion") == 0){ 
            if (size < MIN_TOTAL_POTION){ // Wrong structure
                printf("INVALID\n");
                return;
            }

            char *word = arr[2]; // Potion name or question mark
            char *lastWord = arr[size-1]; // Last word

            // Total potion question with disjoint question mark
            if(strcmp(word, "?") == 0 && size == 3){
                allPotions(potions); // Execute ALL POTIONS query
                return;
            }

            // Specific potion with adjoint or disjoint question mark
            else if(lastWord[strlen(lastWord)-1] == '?'){
                char *potionName = strstr(input, "potion") + strlen("potion"); // Points after "potion"

                while (*potionName == ' '){ // Skip whitespaces
                    potionName++;
                }

                // Last word is a seperate question mark
                if (strlen(lastWord) == 1){
                    char *potionEnd = strstr(input, "?"); // Points to "?"
                    extractPotionName(potionName, potionEnd); // Extract potion name
                }

                // Last word has a question mark at the end of it
                else{
                    input[strlen(input)-1] = '\0'; // Remove '?'
                }
                
                if (!isNameValid(potionName)){ // Name is invalid
                    printf("INVALID\n");
                    return;
                }

                specificPotion(potions, potionName); // Execute SPECIFIC POTION query
            }
            else{ // Wrong structure
                printf("INVALID\n");
                return;
            }
        }

        // If the second word is "trophy"
        else if(strcmp(arr[1], "trophy") == 0){
            if (size < MIN_TOTAL_TROPHY){ // Not enough words
                printf("INVALID\n");
                return;
            }
            char *word = arr[2];

            if(strcmp(word, "?") == 0 && size == 3){ // "Total trophy ?"
                allTrophies(trophies); // Execute ALL TROPHIES query
                return;
            }
            if(size == 3){ // It has to be a specific trophy question with adjoint question mark
                if(word[strlen(word)-1] == '?'){
                    word[strlen(word)-1] = '\0'; // Make question mark null terminator

                    if(!isNameValid(word)){ // Name is invalid
                        printf("INVALID\n");
                        return;
                    }
                    specificTrophies(trophies, arr[2]); // Execute SPECIFIC TROPHIES query
                    return;
                }
                else{
                    printf("INVALID\n");
                    return;
                }
            }

            else if(size == 4){ // Input has to be specific trophy with disjoint question mark
                char *qMark = arr[3]; // Question mark
                if(!isNameValid(word) || strcmp(qMark, "?") != 0){ // Name is invalid or last word is not question mark
                    printf("INVALID\n");
                    return;
                }

                specificTrophies(trophies, word); // Execute SPECIFIC TROPHIES query
                return;
            }

            else{ // The size of the input with the second word "trophy"'s size can not be more than 4
                printf("INVALID\n");
                return;
            }  
        }

        // Invalid structure
        else{
            printf("INVALID\n");
            return;
        }
    }

    // First word is "What" and second word is "is"
    else if (strcmp(arr[0], "What") == 0 && strcmp(arr[1], "is") == 0){
        if (size < MIN_WHAT){ // Not enough words
            printf("INVALID\n");
            return;
        }

        // Third word is "in"
        if (strcmp(arr[2], "in") == 0){
            char *lastWord = arr[size-1];

            if (lastWord[strlen(lastWord)-1] != '?'){ // Last char of last word must be '?'
                printf("INVALID\n");
                return;
            }

            char *potionName = strstr(input, "in") + strlen("in"); // Points after "in"

            while (*potionName == ' '){  // Skip whitespaces
                potionName++;
            }
            
            // Disjoint question mark
            if (strcmp(lastWord, "?") == 0 && size != MIN_WHAT){
                char *potionEnd = strstr(input, "?");
                extractPotionName(potionName, potionEnd);
            }

            // Adjoint question mark
            else{
                input[strlen(input)-1] = '\0'; // Make last char null terminator
            }

            if (!isNameValid(potionName)){ // Name is invalid
                printf("INVALID\n");
                return;
            }

            potionFormula(potions, potionName); // Execute POTION FORMULA query
            return;
        
        }

        // Third word is "effective" and fourth word is "against"
        else if (strcmp(arr[2], "effective") == 0 && strcmp(arr[3], "against") == 0){
            if (size != MIN_EFFECTIVE && size != MIN_EFFECTIVE+1){ // At least 5 words must occur
                printf("INVALID\n");
                return;
            }

            char *lastWord = arr[size-1];

            if (lastWord[strlen(lastWord)-1] != '?'){ // Last char of last word must be '?'
                printf("INVALID\n");
                return;
            }

            char *monsterName;

            // Disjoint question mark
            if (strcmp(lastWord, "?") == 0 && size == MIN_EFFECTIVE+1){
                monsterName = arr[size-2]; // 2nd element from last is the monster name
            }
            
            // Adjoint question mark
            else{
                lastWord[strlen(lastWord)-1] = '\0';
                monsterName = lastWord;
            }


            if (!isNameValid(monsterName)){ // Name is invalid
                printf("INVALID\n");
                return;
            }

            potionSignEffectiveness(monsters, monsterName); // Execute POTION SIGN EFFECTIVENESS query
        }
        else{
            printf("INVALID\n");
            return;
        }
    }

    // Invalid structure
    else{
        printf("INVALID\n");
        return;
    }
}


/**
 * @brief Main function. The function reads the input line by line and executes the command.
 */
int main(void) {
    char line[1025]; // Input buffer
    char lineCopied[1025]; // Copy of input buffer
    char temp[3000];
    
    ingredients = malloc(sizeof(HashMap));  // Pointer to ingredients HashMap
    initializeMap(ingredients, INITIAL_CAPACITY);

    trophies = malloc(sizeof(HashMap));  // Pointer to trophies HashMap
    initializeMap(trophies, INITIAL_CAPACITY);

    potions = malloc(sizeof(HashMap));  // Pointer to potions HashMap
    initializeMap(potions, INITIAL_CAPACITY);

    monsters = malloc(sizeof(HashMap));  // Pointer to monsters HashMap
    initializeMap(monsters, INITIAL_CAPACITY);

    
    while (1){
        printf(">> ");
        fflush(stdout);
        if (fgets(line, sizeof(line), stdin) == NULL)
            break;

        line[strcspn(line, "\n")] = '\0';  // Discard new line character

        if (strstr(line, ",,")){
            printf("INVALID\n");
            continue;
        }

        strcpy(lineCopied, line); // Copy line to lineCopied
        int k = 0;
        for (int i = 0; i < strlen(lineCopied); i++){
            if (lineCopied[i] == ','){
                temp[k++] = ' ';
                temp[k++] = ',';
                temp[k++] = ' ';
            }else{
                temp[k++] = lineCopied[i];
            }
        }
        temp[k] = '\0';

        char *arr[strlen(line)]; // Array of strings to store tokens

        int size; // Size of arr

        // Extract tokens according to " "
        char *token = strtok(temp, " ");
        size = 0;
        while (token != NULL){
            arr[size] = strdup(token); // Memory is allocated for each token, so at the end we must free it
            size++;
            token = strtok(NULL, " ");
        }

        if (size == 1 && strcmp(arr[0], "Exit") == 0){
            // Free every HashMap
            freeHashMap(ingredients);
            freeHashMap(trophies);
            freeHashMapPotion(potions);
            freeHashMapMonster(monsters);

            for (int i = 0; i < size; i++){
                free(arr[i]);
            }
            return 1;
        }

        // No need to continue, invalid structure
        if (size < 2){
            printf("INVALID\n");
            continue;
        }

        execute_line(arr, size, line); // Execute the line

        // Free allocated memory for arr
        for (int i = 0; i < size; i++){
            free(arr[i]);
        }
    }

    // Free every HashMap
    freeHashMap(ingredients);
    freeHashMap(trophies);
    freeHashMapPotion(potions);
    freeHashMapMonster(monsters);

    return 0;
}
