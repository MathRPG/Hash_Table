#include <stdlib.h>
#include <stdbool.h>

#include "article.h"
#include "hashtable.h"

typedef unsigned long ht_index_t;
static const int HT_INITIAL_CAPACITY = 2;

typedef enum HashTableCellState CellState_t;

enum HashTableCellState
{
	OPEN, FILLED, REMOVED
};

struct HashTable_s
{
	ht_index_t count;
	ht_index_t capacity;
	Article_t** items;
	CellState_t* states;
};

#define HT_ITER_THROUGH_INDEXES(TABLE, VAR_NAME) \
    for(ht_index_t VAR_NAME = 0; VAR_NAME < TABLE->capacity; ++VAR_NAME)

HashTable_t* ht_create(void)
{
	HashTable_t* const new_table = (HashTable_t*)malloc(sizeof(HashTable_t));

	new_table->count = 0;
	new_table->capacity = HT_INITIAL_CAPACITY;

	new_table->items = (Article_t**)malloc(new_table->capacity * sizeof(Article_t*));
	new_table->states = (CellState_t*)malloc(new_table->capacity * sizeof(CellState_t));

	HT_ITER_THROUGH_INDEXES(new_table, i)
	{
		new_table->items[i] = NULL;
		new_table->states[i] = OPEN;
	}

	return new_table;
}

void ht_delete(HashTable_t* const ht)
{
	free(ht->items);
	free(ht->states);
	free(ht);
}

bool ht_is_empty(const HashTable_t* const ht)
{
	return ht->count == 0;
}

bool cell_at_index_has_key(const HashTable_t* ht, const ht_index_t i, const char* const key)
{
	return ht->states[i] == FILLED && article_has_key(ht->items[i], key);
}

bool ht_contains(const HashTable_t* const ht, const char* key)
{
	HT_ITER_THROUGH_INDEXES(ht, i)
	{
		if (cell_at_index_has_key(ht, i, key))
			return true;
	}
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
	ht->states[i] = FILLED;
	ht->count++;
}

void ht_insert(HashTable_t* const ht, const Article_t* const article)
{
	// Possibly replace old (key, value) pair
	HT_ITER_THROUGH_INDEXES(ht, i)
	{
		if (ht->states[i] == FILLED && articles_have_same_key(ht->items[i], article))
			return replace_item_at_index(ht, article, i);
	}

	// If not, key is new
	HT_ITER_THROUGH_INDEXES(ht, i)
	{
		if (ht->states[i] != FILLED)
			return insert_item_at_index(ht, article, i);
	}
}

unsigned long ht_count(const HashTable_t* const ht)
{
	return ht->count;
}

const Article_t* ht_fetch(const HashTable_t* const ht, const char* const key)
{
	HT_ITER_THROUGH_INDEXES(ht, i)
	{
		if (cell_at_index_has_key(ht, i, key))
			return ht->items[i];
	}
	return NULL;
}

void remove_item_at_index(HashTable_t* const ht, const ht_index_t i)
{
	delete_article(ht->items[i]);
	ht->states[i] = REMOVED;
	ht->count--;
}

void ht_remove(HashTable_t* const ht, const char* const key)
{
	HT_ITER_THROUGH_INDEXES(ht, i)
	{
		if (cell_at_index_has_key(ht, i, key))
			return remove_item_at_index(ht, i);
	}
}
