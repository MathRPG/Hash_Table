#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdbool.h>
#include <stdlib.h>

#include "article.h"

struct HashTable;
typedef struct HashTable HashTable;

HashTable* ht_create(unsigned long i);
void ht_delete(HashTable* ht);
bool ht_is_empty(HashTable* ht);
bool ht_contains(HashTable* ht, const char* string);
void ht_insert(HashTable* ht, struct Article article);

#endif //HASH_TABLE_H
