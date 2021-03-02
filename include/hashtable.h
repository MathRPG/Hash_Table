#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdbool.h>
#include <stdio.h>

#include "article.h"

typedef struct HashTable_s HashTable_t;

// Constructors/Destructors
HashTable_t* ht_new(void);
void ht_delete(HashTable_t* ht);

// Queries
bool ht_is_empty(const HashTable_t* ht);
bool ht_contains(const HashTable_t* ht, const char* key);
unsigned long ht_count(const HashTable_t* ht);
unsigned long ht_capacity(const HashTable_t* ht);
const Article_t* ht_fetch(const HashTable_t* ht, const char* key);

// Commands
void ht_insert(HashTable_t* ht, const Article_t* article);
void ht_remove(HashTable_t* ht, const char* key);
void ht_resize(HashTable_t* ht, unsigned long new_capacity);
void ht_expand(HashTable_t* ht);
void ht_shrink(HashTable_t* ht);
void ht_display_states(HashTable_t* ht, FILE* out);

#endif //HASH_TABLE_H
