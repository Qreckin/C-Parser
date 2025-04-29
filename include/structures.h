#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    char *key;
    int count;
}Pair;

typedef struct{
    Pair **array; // array is a pointer to an array of Pair pointers
    int size; // Number of pairs inside array
    int capacity;  // Current maximum capacity of array
}PairArray;

typedef struct{
    int potionCount;
    PairArray *recipe;
}Potion;

typedef struct {
    char **effectivePotions;
    int potionCount;

    char **effectiveSigns;
    int signCount;
}Bestiary;

void resizeArray(PairArray *pairArray);


#endif



