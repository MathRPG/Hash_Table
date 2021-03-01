#include "article.h"
#include "hashtable.h"

#include <stdbool.h>

struct HashTable_s
{
	unsigned long count;
};

HashTable_t* ht_create(unsigned long i)
{
	return (HashTable_t*)calloc(1, sizeof(HashTable_t));
}

void ht_delete(HashTable_t* ht)
{
	free(ht);
}

bool ht_is_empty(HashTable_t* ht)
{
	return ht->count == 0;
}

bool ht_contains(HashTable_t* ht, const char* string)
{
	return ht->count != 0;
}

void ht_insert(HashTable_t* ht, Article_t* article)
{
	ht->count++;
}

unsigned long ht_count(HashTable_t* ht)
{
	return ht->count;
}