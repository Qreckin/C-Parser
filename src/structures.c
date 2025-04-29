#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct{
    char *key; // String of the pair
    int count; // Number of the pair
}Pair;

typedef struct{
    Pair **array; // array is a pointer to an array of Pair pointers
    int size; // Number of pairs inside array
    int capacity;  // Current maximum capacity of array
}PairArray;

typedef struct{
    int potionCount; // number of potions in recipe
    PairArray *recipe; // PairArray
}Potion;

typedef struct {
    char **effectivePotions; // Array of strings to hold effective potions
    int potionCount; // Size of effective potions

    char **effectiveSigns; // Array of strings to hold effective signs
    int signCount; // Size of effective signs
}Bestiary;


/**
 * @brief Resize the PairArray to double its capacity
 * @param pairArray The PairArray to resize
 */
void resizeArray(PairArray *pairArray){
    int newSize = pairArray->capacity*2;
    Pair **newArray = realloc(pairArray->array, newSize*sizeof(Pair*)); // Reallocate 2x the old size

    if (newArray == NULL){
        printf("Memory reallocation failed!");
        exit(1);
    }

    // Update values
    pairArray->capacity = newSize;
    pairArray->array = newArray;
}