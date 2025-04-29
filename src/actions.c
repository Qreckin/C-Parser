#include "hashmap.h"
#include "structures.h"
#include "helper_methods.h"

/**
 * @brief Loots the ingredients from the loot array and updates the hashmap
 * @param ingredients The hashmap containing the ingredients
 * @param lootArray The array of pairs containing the ingredients and their counts
 */
void loot(HashMap *ingredients, PairArray *lootArray){
    for (int i = 0; i < lootArray->size; i++){
        char *key = lootArray->array[i]->key; // Name of the current loot
        int val = lootArray->array[i]->count; // Count of the current loot

        if (contains(ingredients, key)){ // Checks if the ingredient already exists
            int pre = *(int*)get(ingredients, key); // Gets the previous amount of the ingredient
            int newAmount = pre + val;
            update(ingredients, key, &newAmount, sizeof(newAmount)); // Update the amount of the ingredient
        }
        else{  // If it is the first we see key, insert it to hashmap
            insert(ingredients, key, &val, sizeof(int)); // If the ingredient is new, insert it to the hashmap
        }
    }
    printf("Alchemy ingredients obtained\n");
}

/**
 * @brief Trades trophies for ingredients
 * @param ingredients The hashmap containing the ingredients
 * @param trophies The hashmap containing the trophies
 * @param requiredIngredients The array of pairs containing the ingredients and their counts
 * @param requiredTrophies The array of pairs containing the trophies and their counts 
 */
void trade(HashMap *ingredients, HashMap *trophies, PairArray *requiredIngredients, PairArray *requiredTrophies){
    int len_trophies = requiredTrophies->size; // Number of trophies
    int len_ing = requiredIngredients->size; // Number of ingredients

    for (int i = 0; i < len_trophies; i++){
        Pair *currentTrophy = requiredTrophies->array[i]; // Gets the current trophy

        if(contains(trophies, currentTrophy->key)){ // Check if the corresponding trophy exists
            int val = *(int *)get(trophies, currentTrophy->key);
            if (val < currentTrophy->count){ // Checks if there exist sufficient amount
                printf("Not enough trophies\n"); 
                return;
            }
        }
        else{ //If there is no such trophy
            printf("Not enough trophies\n");
            return;
        }
    }

    // If we are here, then trophies have valid names and quantities, so we can reduce the quantities safely
    for (int i = 0; i < len_trophies; i++){
        Pair *currentTrophy = requiredTrophies->array[i];  // Gets the current trophy

        int val = *(int *)get(trophies, currentTrophy->key);
        int newVal = val - currentTrophy->count; // Update the amount
        update(trophies, currentTrophy->key, &newVal, sizeof(newVal)); // Update trophies hashmap
    }

    // Increment the ingredients by specified amount
    for(int i = 0; i < len_ing; i++){
        Pair *currentIngredient = requiredIngredients->array[i]; // Gets the current ingredient

        if(contains(ingredients, currentIngredient->key)){ // Checks if there exist such ingredient
            int pre = *(int*)get(ingredients, currentIngredient->key);
            int updated = pre + currentIngredient->count; // Update the amount
            update(ingredients, currentIngredient->key, &(updated), sizeof(int)); // Update the ingredients hashmap
        }
        else{ //If the ingredient is new, insert the ingredient with specified amount
            insert(ingredients, currentIngredient->key, &(currentIngredient->count), sizeof(int));
        }
    }
    printf("Trade successful\n");
}

/**
 * @brief Brews a potion using the ingredients from the hashmap
 * @param potions The hashmap containing the potions
 * @param ingredients The hashmap containing the ingredients
 * @param potion The name of the potion to brew
 */
void brew(HashMap *potions, HashMap *ingredients, char *potion){
    Potion *p = (Potion *)get(potions, potion); // Retrieve the potion struct

    if (!p){ // Potion is not known
        printf("No formula for %s\n", potion);
        return;
    }

    PairArray *r = p->recipe;  // Recipe array for the potion

    for (int i = 0; i < r->size; i++){
        char *currentKey = r->array[i]->key; // Current ingredient key
        int neededAmount = r->array[i]->count; // Amound needed from current ingredient

        int *availableAmount = (int*)get(ingredients, currentKey);  // Amount available of current ingredient
        if (!availableAmount || *availableAmount < neededAmount){ // Checks if there exist sufficient amount
            printf("Not enough ingredients\n");
            return;
        }
    }

    //
    for (int i = 0; i < r->size; i++){ // Update the ingredients
        char *currentKey = r->array[i]->key; // Current ingredient key
        int neededAmount = r->array[i]->count; // Amound needed from current ingredient

        int *valPtr = (int*)get(ingredients, currentKey);
        int newVal = *valPtr - neededAmount; // Update the amount
        update(ingredients, currentKey, &newVal, sizeof(int)); // Update the ingredients hashmap
    }
    p->potionCount += 1; // Increase the amount of the potion
    printf("Alchemy item created: %s\n", potion);
}

/**
 * @brief Learns the effectiveness of a sign against a monster
 * @param monsters The hashmap containing the monsters
 * @param monster The name of the monster
 * @param sign The name of the sign 
 */
void learnSign(HashMap *monsters, char *monster, char *sign){

    if (contains(monsters, monster)){ // Checks if the Gerald knows monster
        Bestiary *b = (Bestiary*) get(monsters, monster); // Retrieves the effective signs and potions to corresponding monster

        for(int i=0; i < b->signCount; i++){ // Iterate through the effective signs
            if(strcmp(sign, b->effectiveSigns[i]) == 0){ // Checks if Gerald already knows this sign
                printf("Already known effectiveness\n");
                return;
            }
        }
        // If code reaches here, it means the sign is new

        if (b->signCount == 0)
            b->effectiveSigns = malloc(sizeof(char *));
        else
            b->effectiveSigns = realloc(b->effectiveSigns, sizeof(char*) * (b->signCount + 1)); // Increase the size of array for new sign
        b->effectiveSigns[b->signCount] = strdup(sign); // Insert the sign
        b->signCount++; // Increase the size of array

        printf("Bestiary entry updated: %s\n", monster);
    }
    else{ // If the monster is new

        // Complete necessary allocations
        Bestiary *newEntry = malloc(sizeof(Bestiary));
        newEntry->effectiveSigns = malloc(sizeof(char*));
        newEntry->effectivePotions = NULL;

        newEntry->effectiveSigns[0] = strdup(sign); // Insert the sign to the array
        newEntry->signCount = 1; // Increase the size
        newEntry->potionCount = 0;

        insert(monsters, monster, newEntry, sizeof(Bestiary)); // Insert the monster to the Monsters Hashmap
        printf("New bestiary entry added: %s\n", monster);
    }
}

/**
 * @brief Learns the effectiveness of a potion against a monster
 * @param monsters The hashmap containing the monsters
 * @param monster The name of the monster
 * @param potion The name of the potion
 */
void learnPotion(HashMap *monsters, char *monster, char *potion){
    if (contains(monsters, monster)){ //Checks if the Gerald knows monster
        Bestiary *b = (Bestiary*) get(monsters, monster);

        for(int i=0; i < b->potionCount; i++){ // Iterate through the effective potions
            if(strcmp(potion, b->effectivePotions[i]) == 0){ // Checks if Gerald already knows this potion
                printf("Already known effectiveness\n");
                return;
            }
        }
        // If code reaches here, it means the potion is new
        if (b->potionCount == 0){
            b->effectivePotions = malloc(sizeof(char *));
        }
        else{
            b->effectivePotions = realloc(b->effectivePotions, sizeof(char*) * (b->potionCount + 1)); // Increase the size of array for new potion
        }
        b->effectivePotions[b->potionCount] = strdup(potion); // Put the potion to the array
        b->potionCount++; // Increase the size of array

        printf("Bestiary entry updated: %s\n", monster);
    }
    else{
        // Complete necessary allocations
        Bestiary *newEntry = malloc(sizeof(Bestiary));
        newEntry->effectivePotions = malloc(sizeof(char*));
        newEntry->effectiveSigns = NULL;

        newEntry->effectivePotions[0] = strdup(potion); // Insert the potion to the array
        newEntry->potionCount = 1; // Increase the size
        newEntry->signCount = 0;

        insert(monsters, monster, newEntry, sizeof(Bestiary)); // Insert the monster to the Monsters Hashmap
        printf("New bestiary entry added: %s\n", monster);
    }
}

/**
 * @brief Learns the recipe of a potion
 * @param potions The hashmap containing the potions
 * @param potion The name of the potion
 * @param ingredients The array of pairs containing the ingredients and their counts
 */
void learnPotionRecipe(HashMap *potions, char *potion, PairArray *ingredients){

    if (contains(potions, potion)){ // If formula is already known
        printf("Already known formula\n");
        freePairArray(ingredients);
        return;
    }

    Potion *potionWithRecipe = malloc(sizeof(Potion)); // Allocate memory for the potion
    potionWithRecipe->recipe = ingredients; // Set the recipe
    potionWithRecipe->potionCount = 0;

    insert(potions, potion, potionWithRecipe, sizeof(Potion)); // Insert the potion to the potions Hashmap
    printf("New alchemy formula obtained: %s\n" , potion);
}


/**
 * @brief Geralt encounters a monster and tries to defeat it
 * @param monsters The hashmap containing the monsters
 * @param potions The hashmap containing the potions
 * @param trophies The hashmap containing the trophies
 * @param monster The name of the monster
 */
void encounter(HashMap *monsters, HashMap *potions, HashMap *trophies, char *monster){

    if(contains(monsters, monster)){ //Checks if Geralt knows the monster
        Bestiary *b = (Bestiary*) get(monsters, monster);
        int canDefeat = 0; // Boolean variable to check if Geralt can defeat the monster

        for (int i = 0; i < b->potionCount; i++){ // Iterate through the effective potions
            if(contains(potions, b->effectivePotions[i])){ // Checks if the potion is known
                Potion *p = get(potions,b->effectivePotions[i]);

                if (p->potionCount > 0){
                    p->potionCount -= 1;  // Decrease the amount of the potion
                    canDefeat = 1; // Geralt can defeat the monster since we can at least utilize this specific potion
                }
            }
        }
        
        if (b->signCount > 0) // There is a sign that can defeat the monster
            canDefeat = 1;

        if (!canDefeat){ // If Geralt does not know any effective potion or sign
            printf("Geralt is unprepared and barely escapes with his life\n");
            return;
        }

        if(contains(trophies, monster)){ // If trophy already exist
            int *amount = get(trophies, monster);
            int newAmount = *amount + 1; // Increase the amount of the trophy
            update(trophies, monster, &newAmount, sizeof(int)); // Update the trophies hashmap
        }
        else{ // If the trophy is new
            int amount = 1;
            insert(trophies, monster, &amount, sizeof(int)); // Insert the trophy to the trophies Hashmap
        }
        printf("Geralt defeats %s\n" , monster);
        return;
    }
    else{ // If the monster is new
        printf("Geralt is unprepared and barely escapes with his life\n");
        return;
    }
}