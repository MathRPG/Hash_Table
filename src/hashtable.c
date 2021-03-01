#include "hashtable.h"

struct HashTable
{
	bool has_something;
};

HashTable* ht_create(unsigned long i)
{
	return (HashTable*)malloc(sizeof(struct HashTable));
}

void ht_delete(HashTable* ht)
{
	free(ht);
}

bool ht_is_empty(HashTable* ht)
{
	return true;
}

bool ht_contains(HashTable* ht, const char* string)
{
	return ht->has_something;
}

void ht_insert(HashTable* ht, struct Article article)
{
	ht->has_something = true;
}
