#include "hashmap.h"
#include "structures.h"
#include "helper_methods.h"
#include <stdio.h>

/**
 * @brief Prints the amount of a specific ingredient
 * @param ingredients The hashmap containing the ingredients
 * @param ingredient The name of the ingredient
 */
void specificIngredients(HashMap *ingredients, char *ingredient){
    if(contains(ingredients, ingredient)){ // Checks if ingredient is available
        int *amount = get(ingredients, ingredient);
        printf("%d\n", *amount);
    }
    else{ // If ingredient is not available
        printf("0\n");
    }
}

/**
 * @brief Prints the amount of a specific potion
 * @param potions The hashmap containing the potions
 * @param potion The name of the potion
 */
void specificPotion(HashMap *potions, char *potion){
    if(contains(potions, potion)){  // Checks if potion is available
        Potion *p = (Potion *)get(potions, potion);
        printf("%d\n", p->potionCount);
        return;
    }
    else{  // If it is not in the potions list
        printf("0\n");
    }
}

/**
 * @brief Prints the amount of a specific trophy
 * @param trophies The hashmap containing the trophies
 * @param trophy The name of the trophy
 */
void specificTrophies(HashMap *trophies, char *trophy){
    if(contains(trophies, trophy)){ // Checks if troph is available
        int *amount = get(trophies, trophy);
        printf("%d\n", *amount);
    }
    else{ // If it is not in the trophies list
        printf("0\n");
    }
}

/**
 * @brief Prints the amount of all ingredients
 * @param ingredients The hashmap containing the ingredients
 */
void allIngredients(HashMap *ingredients){
    int ingredientCount = countQuantity(ingredients); // Number of ingredient entries in the hashmap 
    if (ingredientCount == 0){
        printf("None\n");
        return;
    }
    char** array_of_keys = createArrayOfKeys(ingredients);  // Array consisting of the names of the ingredients
    int c = ingredients->size;  // Number of distinct ingredients

    int realCount = 0; // Number of non-zero entries in the hashmap

    // Iterate through the hashmap and find real count
    for (int i = 0; i < c; i++){
        if (*(int *)get(ingredients, array_of_keys[i]) != 0)
            realCount++;
    }

    qsort(array_of_keys, c, sizeof(char *), compareStrings);  // Sort the array by string comparion

    int k = 0; // Current iteration

    for (int i = 0; i < c; i++){
        char *key = array_of_keys[i];  // Current ingredient name
        int val = *(int *)get(ingredients, array_of_keys[i]);  // Current ingredient value

        if (val == 0)  // Do not print ingredients with value 0
            continue;
        k++;
        if (k == realCount) // Last ingredient to print
            printf("%d %s\n", val, key);
        else
            printf("%d %s, ", val, key);
    }

    freeArrayOfKeys(array_of_keys, c);  // Free the array since we allocated extra memory for it

}

/**
 * @brief Printf the amount and name of all potions
 * @param potions The hashmap containing the potions
 */
void allPotions(HashMap *potions){
    if (countPotions(potions) == 0){
        printf("None\n");
        return;
    }
    
    char** array_of_keys = createArrayOfKeys(potions);  // Array consisting of the names of the potions
    int c = potions->size;  // Number of distinct potions
    int realCount = 0;

    for (int i = 0; i < c; i++){
        Potion *potion = (Potion *)get(potions, array_of_keys[i]);  // Corresponding Potion structure of current potion name
        int potionCount = potion->potionCount;  // Amount of current potion

        if (potionCount != 0)
            realCount++;
    }

    qsort(array_of_keys, c, sizeof(char *), compareStrings);  // Sort the array by string comparison

    int count = 0;

    for (int i = 0; i < c; i++){
        char *key = array_of_keys[i];  // Current potion name
        Potion *potion = (Potion *)get(potions, array_of_keys[i]);  // Corresponding Potion structure of current potion name
        int potionCount = potion->potionCount;  // Amount of current potion


        if (potionCount == 0)  // Do not print potionts with amount 0
            continue;
        
        count++;
        
        if (count == realCount)
            printf("%d %s\n", potionCount, key);
        else
            printf("%d %s, ", potionCount, key);
    }

    freeArrayOfKeys(array_of_keys, c);  // Free the allocated memory since we created extra memory for it
}

/**
 * @brief Prints the amount and name of all trophies
 * @param trophies The hashmap containing the trophies
 */
void allTrophies(HashMap* trophies){
    int trophyCount = countQuantity(trophies); // Number of trophy entries in the hashmap
    if (trophyCount == 0){
        printf("None\n");
        return;
    }

    char** array_of_keys = createArrayOfKeys(trophies);  // Array consisting of name of the trophies
    int c = trophies->size;  // Number of distinct trophies

    int realCount = 0; // Number of non-zero entries in the hashmap

    // Iterate through the hashmap and find the real count of trophies
    for (int i = 0; i < c; i++){
        if (*(int *)get(trophies, array_of_keys[i]) != 0)
            realCount++;
    }

    qsort(array_of_keys, c, sizeof(char *), compareStrings);  // Sort the array by string comparison

    int k = 0; // Current iteration
    for (int i = 0; i < c; i++){
        char *key = array_of_keys[i];  // Current trophy name
        int val = *(int *)get(trophies, array_of_keys[i]);  // Current trophy amount

        if (val == 0)  // Do not print trophies with 0 amount
            continue;
        k++;
        if (k == realCount) // Last trophy to print
            printf("%d %s\n", val, key);
        else
            printf("%d %s, ", val, key);
    }

    freeArrayOfKeys(array_of_keys, c);  // Free the allocated memory for the array since we created extra memory for it
}

/**
 * @brief Prints the effective potions and signs for a specific monster
 * @param monsters The hashmap containing the monsters
 * @param monster The name of the monster
 */
void potionSignEffectiveness(HashMap *monsters, char *monster){
    Bestiary *b = (Bestiary*)(get(monsters, monster));

    if (!b){
        printf("No knowledge of %s\n", monster);
    }
    else{
        char **effectivePotions = b->effectivePotions; // Effective potions against the monster
        char **effectiveSigns = b->effectiveSigns; // Effective signs against the monster
        
        int c = b->potionCount + b->signCount; // Number of effective potion/signs

        char **effectiveTotal = malloc(c * sizeof(char *)); // Allocate memory for the total effective array

        // Add elements to effective total array
        int k = 0;
        for (int i = 0; i < b->potionCount; i++){
            effectiveTotal[k++] = effectivePotions[i];
        }
        for (int j = 0; j < b->signCount; j++){
            effectiveTotal[k++] = effectiveSigns[j];
        }

        qsort(effectiveTotal, k, sizeof(char*), compareStrings); // Sort the array by string comparison
        
        // Print out the effective potion/signs
        for (int i = 0; i < c; i++){
            if (i == c-1)
                printf("%s\n", effectiveTotal[i]);
            else
                printf("%s, ", effectiveTotal[i]);
        }
        
        free(effectiveTotal);
    }
}

/**
 * @brief Prints the formula of a specific potion
 * @param potions The hashmap containing the potions
 * @param potion The name of the potion
 */
void potionFormula(HashMap *potions,char *potion){
    Potion *p = (Potion *)get(potions, potion);
    if(!p){
        printf("No formula for %s\n", potion);
    }
    else{
        PairArray *recipe = p->recipe; // Recipe of the potion
        if(!recipe) // No formula for the potion (not possible I guess)
            printf("No formula for %s\n", potion);
        else{
            Pair **arr = recipe->array; // Pair array representin recipe
            qsort(arr, recipe->size, sizeof(Pair *), comparePotionFormula); // Sort the array according to potion comparison

            // Print out the ingredients in the potion
            for(int i = 0; i<recipe->size; i++){
                if(i == recipe->size - 1)
                    printf("%d %s\n", arr[i]->count, arr[i]->key);
                else
                    printf("%d %s, ", arr[i]->count, arr[i]->key);
            }
        }
    }
}