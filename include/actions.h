#ifndef ACTIONS_H
#define ACTIONS_H

#include "structures.h"
#include "hashmap.h"

void loot(HashMap *map, PairArray *lootArray);
void trade(HashMap *ingredients, HashMap *trophies, PairArray *requiredIngredients, PairArray *requiredTrophies);
void brew(HashMap *potions, HashMap *ingredients, char *potion);
void learnPotionRecipe(HashMap *potions, char *potion, PairArray *ingredients);
void encounter(HashMap *monsters, HashMap *potions, HashMap *trophies, char *monster);


void learnSign(HashMap *monsters, char *monster, char *sign);
void learnPotion(HashMap *monsters, char *monster, char *potion);


#endif