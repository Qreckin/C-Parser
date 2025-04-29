#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_TABLE_SIZE 101
#define LOAD_FACTOR_THRESHOLD 0.7
#define MOD 1000000007

typedef struct HashNode{
    char *key; // Key of the node
    void *value; // Value of the node
    struct HashNode *next; // Pointer to the next node in the linked list
}HashNode;

typedef struct{
    HashNode **table; // Array of HashNode pointers
    int capacity; // Maximum capacity of the hashmap
    int size; // Size of the hashmap
}HashMap;

/**
 * @brief Initialize the hashmap
 * @param map The hashmap to initialize
 * @param capacity The initial capacity of the hashmap
 */
void initializeMap(HashMap *map, int capacity){
    map->size = 0;
    map->capacity = capacity;
    map->table = (HashNode **)calloc(capacity, sizeof(HashNode *)); // table contains #capacity HashNode*. And, HashNode*'s will point to HashNode structs

    if (!map->table){
        printf("Memory allocation failed.");
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Hash function to calculate the index of a key in the hashmap
 * @param map The hashmap
 * @param key The key to hash
 * @return The index of the key in the hashmap
 */
unsigned int hash(HashMap *map, const char *key){
    unsigned int hash = 0;
    int p = 1;
    while (*key){
        hash = (hash + (*key - 'a' + 1)*p)%MOD;
        p = (p*31)%MOD; // Move to the next power
        key++; // Move to next char
    }

    return hash % map->capacity; // Return the index in mod capacity
}

/**
 * @brief Retrieve the value of a key in the hashmap
 * @param map The hashmap
 * @param key The key to retrieve
 * @return The value(void pointer) of the key in the hashmap, or NULL if not found
 */
void* get(HashMap *map, const char *key){
    if (!map) // If map does not exists, we can not retrieve anything
        return NULL;

    int index = hash(map, key); // Find the possible index of the key
    
    HashNode* node = map->table[index]; // Get the head of the linked list

    // Traverse the linked list to find the key
    while (node){
        if (strcmp(node->key, key) == 0){ // If we find a match
            return node->value;
        }
        node = node->next;
    }

    return NULL; // If we cant find a match, return NULL
}

/**
 * @brief Update the value of a key in the hashmap
 * @param map The hashmap
 * @param key The key to update
 * @param value The new value to set
 * @param valueSize The size of the new value
 */
void update(HashMap *map, const char *key, void *value, size_t valueSize){
    if (!map) // map is null
        return;
    
    int index = hash(map, key); // Find the possible index of the key

    HashNode* node = map->table[index]; // Get the head of the linked list

    // Traverse the linked list to find the key
    while(node){
        if (strcmp(node->key, key) == 0){ // Key is found
            free(node->value); // Remove the previous value
            node->value = malloc(valueSize); // Allocate necessary size for new value
            if (node->value == NULL){
                printf("Memory allocation failed.");
                return;
            }
            memcpy(node->value, value, valueSize); // Copy value to node->value
            return;
        }
        node = node->next;
    }

}


/**
 * @brief Rehash the hashmap to a new capacity
 * @param map The hashmap to rehash
 */
void rehash(HashMap *map){
    int oldCapacity = map->capacity;
    int newCapacity = oldCapacity*2+1; // New capacity is twice the old capacity + 1
    HashMap newMap; 
    initializeMap(&newMap, newCapacity); // Allocate memory for the map of capacity newCapacity

    // For every HashNode* in the old map, rehash it and insert it to the newMap
    for (int i = 0; i < oldCapacity; i++){
        HashNode* node = map->table[i]; // Current HashNode
        while(node){
            HashNode* nextNode = node->next; // Save nextNode
            int newIndex = hash(&newMap, node->key); // Hash the key again for newMap
            node->next = newMap.table[newIndex]; // Insert node to the head of the linked list
            newMap.table[newIndex] = node; // Set our head pointer to head node
            node = nextNode;
        }
    }

    free(map->table); // Free the memory for the old map

    map->capacity = newCapacity; // Update capacity
    map->table = newMap.table; // Update table
}

/**
 * @brief Check if a key exists in the hashmap
 * @param map The hashmap
 * @param key The key to check
 * @return 1 if the key exists, 0 otherwise
 */
int contains(HashMap *map, const char* key){

    int index = hash(map, key); // Find the possible index of the key

    HashNode *currentNode = map->table[index]; // Get the head of the linked list

    // Traverse the linked list
    while (currentNode){
        if (strcmp(currentNode->key, key) == 0)
            return 1;
        currentNode = currentNode->next;
    }

    return 0;
}

/**
 * @brief Delete a key from the hashmap
 * @param map The hashmap
 * @param key The key to delete
 */
void deleteKey(HashMap *map, const char *key){
    if (!map) // If there is no map, do not proceed
        return;

    int index = hash(map, key); // Find the possible index of the key
    HashNode* node = map->table[index]; // Get the head of the linked list
    HashNode* prev = NULL; // Pointer to the previous node of node

    if(!node) // If there is no node, deletion fails
        return;

    // If the node to delete is the first node
    if (strcmp(node->key, key) == 0){
        // Deleting the first node
        map->table[index] = node->next;

        // Free the allocated memory for key and value
        free(node->key);
        free(node->value);
        free(node);

        map->size--;
        return;
    }
    // If we are here, that means the node to delete is not the first node, so we can proceed further
    prev = node;
    node = node->next;

    // Traverse the linked list to find the key
    while (node){
        if (strcmp(node->key, key) == 0){
            prev->next = node->next; // Set prev's next to node's next to detach node

            // Free the allocated memory for key and value
            free(node->key);
            free(node->value);
            free(node);

            map->size--; // Decrement size since we deleted a node
            return;
        }
        prev = node;
        node = node->next;
    }

}

/**
 * @brief Insert a key-value pair into the hashmap
 * @param map The hashmap
 * @param key The key to insert
 * @param value The value(void pointer) to insert
 * @param valueSize The size of the value
 */
void insert(HashMap *map, const char *key, void *value, size_t valueSize){
    if ((float)map->size/map->capacity >= LOAD_FACTOR_THRESHOLD){
        rehash(map); // If threshold is exceeded, resize the map
    }

    int index = hash(map, key); // Find the possible index of the key
    HashNode *currentNode = map->table[index]; // Get the head of the linked list

    // Traverse the linked list to find the key
    while (currentNode){
        if (strcmp(currentNode->key, key) == 0)
            return; // Already existing key, do not update!

        currentNode = currentNode->next;
    }

    // Here currentNode points to NULL, we will insert new key here
    HashNode *newNode = (HashNode *)malloc(sizeof(HashNode));
    if (!newNode){
        printf("Memory allocation failed in method INSERT");
        exit(EXIT_FAILURE);
    }

    newNode->key = malloc(strlen(key)+1);  // Allocate memory for the key
    if (!newNode->key){
        printf("Memory allocaiton failed in method INSERT/key");
        exit(EXIT_FAILURE);
    }
    strcpy(newNode->key, key); // Copy key to currentNode's key


    newNode->value = malloc(valueSize); // Allocate valueSize bytes for void pointer value
    if (!newNode->value){
        printf("Memory allocation failed in method INSERT/value");
        exit(EXIT_FAILURE);
    }
    memcpy(newNode->value, value, valueSize); // Copy value to newNode

    // newNode is at the beginning of the linkedlist
    newNode->next = map->table[index];
    map->table[index] = newNode;

    map->size++;
}

