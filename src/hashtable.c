#include <stdlib.h>
#include <stdbool.h>

#include "article.h"
#include "hashtable.h"

typedef unsigned long ht_index_t;
static const int HT_INITIAL_CAPACITY = 2;

struct HashTable_s
{
	ht_index_t count;
	ht_index_t capacity;
	Article_t** items;
};

HashTable_t* ht_create(void)
{
	HashTable_t* const new_table = (HashTable_t*)malloc(sizeof(HashTable_t));

	new_table->count = 0;
	new_table->capacity = HT_INITIAL_CAPACITY;

	new_table->items = (Article_t**)malloc(new_table->capacity * sizeof(Article_t*));
	for (ht_index_t i = 0; i < new_table->capacity; ++i)
		new_table->items[i] = NULL;

	return new_table;
}

void ht_delete(HashTable_t* const ht)
{
	free(ht->items);
	free(ht);
}

bool ht_is_empty(const HashTable_t* const ht)
{
	return ht->count == 0;
}

bool ht_contains(const HashTable_t* const ht, const char* key)
{
	for (ht_index_t i = 0; i < ht->capacity; ++i)
		if (ht->items[i] != NULL && article_has_key(ht->items[i], key))
			return true;
	return false;
}

void replace_item_at_index(HashTable_t* const ht, const Article_t* const article, const ht_index_t i)
{
	delete_article(ht->items[i]);
	ht->items[i] = duplicate_article(article);
}

void insert_item_at_index(HashTable_t* const ht, const Article_t* const article, const ht_index_t i)
{
	ht->items[i] = duplicate_article(article);
	ht->count++;
}

void ht_insert(HashTable_t* const ht, const Article_t* const article)
{
	// Find and replace?
	for (ht_index_t i = 0; i < ht->capacity; ++i)
	{
		if (ht->items[i] != NULL && articles_have_same_key(ht->items[i], article))
		{
			replace_item_at_index(ht, article, i);
			return;
		}
	}

	// New key who dis?
	for (ht_index_t i = 0; i < ht->capacity; ++i)
	{
		if (ht->items[i] == NULL)
		{
			insert_item_at_index(ht, article, i);
			return;
		}
	}
}

unsigned long ht_count(const HashTable_t* const ht)
{
	return ht->count;
}

const Article_t* ht_fetch(const HashTable_t* const ht, const char* const key)
{
	for (ht_index_t i = 0; i < ht->capacity; ++i)
		if (ht->items[i] != NULL && article_has_key(ht->items[i], key))
			return ht->items[i];
	return NULL;
}

void ht_remove(HashTable_t* const ht, const char* const key)
{
	for (ht_index_t i = 0; i < ht->count; ++i)
		if (ht->items[i] != NULL && article_has_key(ht->items[i], key))
		{
			delete_article(ht->items[i]);
			ht->items[i] = NULL;
			ht->count--;
			return;
		}
}
