#ifndef HASHMAP_H
#define HASHMAP_H

#include "stddef.h"

#define INITIAL_TABLE_SIZE 101
#define LOAD_FACTOR_THRESHOLD 0.7
#define MOD 1000000007

typedef struct HashNode{
    char *key;
    void *value;
    struct HashNode *next;
}HashNode;

typedef struct{
    HashNode **table; // We use double pointers because HashNodes are also pointers to other HashNodes (because of next)
    int capacity;
    int size;
}HashMap;

void initializeMap(HashMap *map, int capacity);
unsigned int hash(HashMap *map, const char *key);
void rehash(HashMap *map);
int contains(HashMap *map, const char *key);
void insert(HashMap *map, const char *key, void *value, size_t valueSize);
void* get(HashMap *map, const char *key);
void deleteKey(HashMap *map, const char *key);
void update(HashMap *map, const char *key, void *value, size_t valueSize);


#endif

