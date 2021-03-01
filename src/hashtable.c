#include "hashtable.h"

struct HashTable_s
{
	bool has_something;
};

HashTable_t* ht_create(unsigned long i)
{
	return (HashTable_t*)malloc(sizeof(HashTable_t));
}

void ht_delete(HashTable_t* ht)
{
	free(ht);
}

bool ht_is_empty(HashTable_t* ht)
{
	return true;
}

bool ht_contains(HashTable_t* ht, const char* string)
{
	return ht->has_something;
}

void ht_insert(HashTable_t* ht, Article_t* article)
{
	ht->has_something = true;
}
