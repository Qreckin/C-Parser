#include "structures.h"

void freePairArray(PairArray *arr);
void freeHashMap(HashMap *map);
int isNameValid(char *name);
int containsNonAlphaNumeric(char *name);
int isAlphaNumeric(char c);
int compareStrings(const void *str_ptr_1, const void *str_ptr_2);
char ** createArrayOfKeys(HashMap *map);
void freeArrayOfKeys(char **array_of_keys, int c);
int comparePotionFormula(const void *pair_ptr_1, const void *pair_ptr_2);
void extractPotionName(char *potionStart, char *lastWord);

int countPotions(HashMap *potions);
int countQuantity(HashMap *map);
void freePotion(Potion *p);
void freeBestiary(Bestiary *b);
void freeHashMapPotion(HashMap *map);
void freeHashMapMonster(HashMap *map);

PairArray* constructPairArray(char **tokens, int size);
int checkPairs(char **tokens, int size);
int findIndex(char **tokens, int size, char *key);
int containsOnlyNumbers(char *token);