#include "structures.h"
#include "hashmap.h"

/**
 * @brief Free the memory allocated for a PairArray
 * @param arr The PairArray to free
 */
void freePairArray(PairArray *arr){
    if (!arr)
        return;

    // Traverse throguh pair array and free each pair
    for (int i = 0; i < arr->size; i++){
        free(arr->array[i]->key);
        free(arr->array[i]);
    }

    free(arr->array);
    free(arr);
}

void freePotion(Potion *p) {
    if (!p) return;
    if (p->recipe)
        freePairArray(p->recipe);
    free(p);
}

void freeBestiary(Bestiary *b) {
    if (b == NULL) return;

    if (b->effectivePotions != NULL){
        for (int i = 0; i < b->potionCount; i++)
            free(b->effectivePotions[i]);
        free(b->effectivePotions);
    }

    if (b->effectiveSigns != NULL){
        for (int i = 0; i < b->signCount; i++)
            free(b->effectiveSigns[i]);
        free(b->effectiveSigns);
    }

    free(b);
}


/**
 * @brief Count the total quantity of ingredients or trophies in the hashmap
 * @param map The hashmap to count
 * @return The total quantity of ingredients or trophies
 */
int countQuantity(HashMap *map){
    int total = 0;

    // Iterate through map
    for (int i = 0; i < map->capacity; i++){
        if (!map->table[i]) // If entry is null, pass it
            continue;

        int currentSum = 0;
        HashNode* currentNode = map->table[i]; // Get the head of the linked list

        // Iterate through linked list
        while (currentNode){
            currentSum += *(int *)currentNode->value; // Add the value
            currentNode = currentNode->next;
        }
        total += currentSum;
    }
    return total;
}


/**
 * @brief Count the total quantity of potions in the hashmap
 * @param potions The hashmap to count
 * @return The total quantity of potions
 */
int countPotions(HashMap *potions){
    int total = 0;

    // Iterate through map
    for (int i = 0; i < potions->capacity; i++){
        if (!potions->table[i]) // Entry is null, pass it
            continue;
        
        int currentSum = 0;
        HashNode *currentNode = potions->table[i]; // Get the head of the linked list

        // Iterate through linked list
        while (currentNode){
            Potion *currentPotion = (Potion *)currentNode->value;
            if (!currentPotion)
                continue;

            currentSum += currentPotion->potionCount; // Add potion count
            currentNode = currentNode->next;
        }

        total += currentSum; // Increment total
    }

    return total;
}

/**
 * @brief Free the memory allocated for the hashmap
 * @param map The hashmap to free
 */
void freeHashMap(HashMap *map){
    if (!map) // Map does not exist
        return;
    if (!map->table){ // Table does not exist
        free(map);
        return;
    }
    
    // Iterate through map
    for (int i = 0; i < map->capacity; i++){
        HashNode *current = map->table[i]; // Get the head of the linked list

        // Iterate through linked list
        while (current){
            HashNode *next = current->next; // Save next node

            // Free current node with its inner values
            free(current->key);
            free(current->value);
            free(current);

            current = next; // Proceed to next node
        }
    }
    
    // Free the table and map itself
    free(map->table);
    free(map);
}
void freeHashMapPotion(HashMap *map){
    if (!map) // Map does not exist
        return;
    if (!map->table){ // Table does not exist
        free(map);
        return;
    }
    
    // Iterate through map
    for (int i = 0; i < map->capacity; i++){
        HashNode *current = map->table[i]; // Get the head of the linked list

        // Iterate through linked list
        while (current){
            HashNode *next = current->next; // Save next node

            // Free current node with its inner values
            free(current->key);
            freePotion((Potion *)current->value);
            free(current);

            current = next; // Proceed to next node
        }
    }
    
    // Free the table and map itself
    free(map->table);
    free(map);
}

void freeHashMapMonster(HashMap *map){
    if (!map) // Map does not exist
        return;
    if (!map->table){ // Table does not exist
        free(map);
        return;
    }
    
    // Iterate through map
    for (int i = 0; i < map->capacity; i++){
        HashNode *current = map->table[i]; // Get the head of the linked list

        // Iterate through linked list
        while (current){
            HashNode *next = current->next; // Save next node

            // Free current node with its inner values
            free(current->key);
            freeBestiary((Bestiary *)current->value);
            free(current);

            current = next; // Proceed to next node
        }
    }
    
    // Free the table and map itself
    free(map->table);
    free(map);
}

/**
 * @brief Check if a character is alphanumeric (A-Z, a-z, space)
 * @param c The character to check
 * @return 1 if the character is alphanumeric, 0 otherwise
 */
int isAlphaNumeric(char c){
    return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ');
    // We also excluded 0-9 since we don't want numbers in names of potions, monsters etc.
}

/**
 * @brief Check if a string contains non-alphanumeric characters
 * @param name The string to check
 * @return 1 if the string contains non-alphanumeric characters, 0 otherwise
 */
int containsNonAlphaNumeric(char *name){
    if (!name) // If name is null
        return 0;

    // Iterate until null terminator is reached
    while (*name != '\0'){
        if (!isAlphaNumeric(*name)){ // If character is not alpha numeric
            return 1;
        }
        name++;
    }
    return 0;
}

/**
 * @brief Check if a string is a valid name
 * @param name The string to check
 * @return 1 if the string is a valid name, 0 otherwise
 */
int isNameValid(char *name){
    if (!name || strlen(name) == 0 || containsNonAlphaNumeric(name) || strstr(name, "  "))
        return 0;  // Invalid name, contains consecutive spaces
    return 1;  // Valid name
}

/**
 * @brief Extract the potion name from a string
 * @param potionStart The start of the potion name
 * @param firstWord The first word after the potion name
 */
void extractPotionName(char *potionStart, char *firstWord){
    char *temp = potionStart; // Potion start address
    char *firstWhiteSpace = NULL; // First white space after potion name, initially uninitialized

    // Loop until we reach first word after potion name
    while (temp != firstWord){
        if (*temp == ' '){ // If we reach a white space, save it and skip adjacent white spaces
            firstWhiteSpace = temp;
            temp++;
            while (*temp == ' '){
                temp++;
            }
        }else{
            temp++;
        }
    }

    // Here firstWhiteSpace is pointing to the first white space after potion name
    if (firstWhiteSpace)
        *firstWhiteSpace = '\0';
}

/**
 * @brief Compare two strings for qsort
 * @param str_ptr_1 The first string
 * @param str_ptr_2 The second string
 * @return The result of strcmp
 */
int compareStrings(const void *str_ptr_1, const void *str_ptr_2){
    // Type casting is necessary since a pointer to string is given

    const char *string_1 = *(const char **)str_ptr_1;
    const char *string_2 = *(const char **)str_ptr_2;

    return strcmp(string_1, string_2); // Return basic comparison
}

/**
 * @brief Compare two Pair structures for qsort, sort according to count, if same, apply string comparison
 * @param pair_ptr_1 The first Pair
 * @param pair_ptr_2 The second Pair
 * @return The result of the comparison
 */
int comparePotionFormula(const void *pair_ptr_1, const void *pair_ptr_2){
    const Pair *p1 = *(const Pair **)pair_ptr_1;
    const Pair *p2 = *(const Pair **)pair_ptr_2;

    if (p1->count != p2->count){
        return p2->count - p1->count;  // TODO: Learn why this order matters
    }
    else{
        return strcmp(p1->key, p2->key);
    }
}

/**
 * @brief Create an array of keys from the hashmap
 * @param map The hashmap
 * @return An array of keys
 */
char **createArrayOfKeys(HashMap *map){
    int c = 0;
    char** array_of_keys = malloc(map->size * sizeof(char*)); // Allocate memory for char pointers that will hold keys

    // Iterate through map
    for (int i = 0; i < map->capacity; i++){
        HashNode* current = map->table[i]; // Get the head of the linked list

        // Iterate through linked list 
        while(current){
            array_of_keys[c++] = strdup(current->key); // Insert key into array, CAREFUL: strdup initially uses malloc
            current = current->next;
        }
    }

    return array_of_keys;
}


/**
 * @brief Free the array of keys
 * @param array_of_keys The array of keys
 * @param c The number of keys
 */
void freeArrayOfKeys(char **array_of_keys, int c){

    // Iterate through arrays
    for (int i = 0; i < c; i++){
        if (array_of_keys[i]){ // If entry is not null
            free(array_of_keys[i]); // Free the entry
        }
    }

    // Free the array itself
    free(array_of_keys);
}


/**
 * @brief Check if a string is a valid name. A name is valid if it contains only letters and is not empty.
 * @param token The string to check
 * @return 1 if the string is valid, 0 otherwise 
 */
int containsOnlyNumbers(char *token){ // Check if a string contains only numbers
    if (!token) // If token is NULL
        return 0;

    if (*token == '0')
        return 0;

    while (*token != '\0'){
        if (*token < '0' || *token > '9') // If token is not a number
            return 0;
        token++;
    }

    return 1; 
}


/**
 * @brief Find the index of a string in an array of strings.
 * @param tokens The array of strings
 * @param size The size of the array
 * @param key The string to find
 * @return The index of the string in the array, or -1 if not found
 */
int findIndex(char **tokens, int size, char *key){
    
    for (int i = 0; i < size; i++){
        if (strcmp(tokens[i], key) == 0) // If the string is found
            return i;
    }

    return -1;
}

/**
 * @brief Check if the pairs are valid. A pair is valid if it contains a number followed by a name.
 * 
 * @param tokens The array of tokens
 * @param size The size of the array
 * @return 1 if the pairs are valid, 0 otherwise
 */
int checkPairs(char **tokens, int size){
    int i = 0;
    char *currentWord = tokens[i];

    while (containsOnlyNumbers(currentWord)){
        int num = atoi(currentWord); // Convert string to integer
        i++; // Move to the next token

        if (i >= size){ // If i exceeds size, we have a wrong structure
            return 0;
        }

        currentWord = tokens[i]; // Next word to parse
        if (strcmp(currentWord, ",") == 0){ // If the next word is a comma we have a wrong structure
            return 0;
        }

        int wordSize = strlen(currentWord); // Get the size of the current word

        if (currentWord[wordSize-1] == ','){ // , is attached to the name
            currentWord[wordSize-1] = '\0'; // Remove the comma
        }
        else if (i+1 < size && strcmp(tokens[i+1], ",") == 0){ // If the next word is a comma
            i++; // Move to the next token
        }
        else{  // (1)Last word of the line or (2)missing comma
            if (i == size-1 && isNameValid(currentWord) && num > 0){ // (1)
                return 1;  // Pairs are VALID
            }
            return 0; // (2)
        }

        if (!isNameValid(currentWord) || num <= 0){  // If the current word is not a valid name or the number is not positive
            return 0;
        }

        i++; // Move to the next token
        if (i >= size){  // If i exceeds size, we have a wrong structure
            return 0;
        }

        currentWord = tokens[i]; // Next word to parse
    }

    return 0;  // Pairs are INVALID
}


/**
 * @brief construct a PairArray from an array of tokens. A PairArray is an array of pairs, where each pair consists of a number and a name.
 * @param tokens The array of tokens
 * @param size The size of the array
 * @return A pointer to the PairArray
 */
PairArray* constructPairArray(char **tokens, int size){

    PairArray* pairArray = malloc(sizeof(PairArray)); // Allocate memory for PairArray
    pairArray->size = 0; // Initialize size to 0
    pairArray->capacity = 10; // Initialize capacity to 10
    pairArray->array = malloc(sizeof(Pair*)*pairArray->capacity); // Allocate memory for the array of pairs

    int i = 0;
    while (i < size){ // While there are tokens to parse

        int count = atoi(tokens[i]); // Convert string to integer
        i++; // Move to the next token
        char *ingredientName = tokens[i]; // Get the name of the ingredient
        i++; // Move to the next token

        while (i < size && strcmp(tokens[i], ",") == 0){ // If the next token is a comma
            i++; // Move to the next token
        }

        Pair *newPair = malloc(sizeof(Pair)); // Allocate memory for the new pair
        newPair->count = count; // Set the count
        newPair->key = malloc(strlen(ingredientName)+1); // Allocate memory for the key
        strcpy(newPair->key, ingredientName); // Copy the name to the key

        if (pairArray->size == pairArray->capacity) // If the array is full
            resizeArray(pairArray); // Resize the array

        pairArray->array[pairArray->size++] = newPair; // Add the new pair to the array

    }

    return pairArray; // Return the PairArray
}