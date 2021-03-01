#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdbool.h>

#include "article.h"

typedef struct HashTable_s HashTable_t;

HashTable_t* ht_create(void);
void ht_delete(HashTable_t* ht);
bool ht_is_empty(const HashTable_t* ht);
bool ht_contains(const HashTable_t* ht, const char* key);
void ht_insert(HashTable_t* ht, const Article_t* article);
unsigned long ht_count(const HashTable_t* ht);
const Article_t* ht_fetch(const HashTable_t* ht, const char* key);
void ht_remove(HashTable_t* ht, const char* key);

#endif //HASH_TABLE_H
