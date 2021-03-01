#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "article.h"

#include <stdbool.h>
#include <stdlib.h>

struct HashTable_s;
typedef struct HashTable_s HashTable_t;

HashTable_t* ht_create(unsigned long i);
void ht_delete(HashTable_t* ht);
bool ht_is_empty(HashTable_t* ht);
bool ht_contains(HashTable_t* ht, const char* string);
void ht_insert(HashTable_t* ht, Article_t* article);
unsigned long ht_count(HashTable_t* ht);

#endif //HASH_TABLE_H
