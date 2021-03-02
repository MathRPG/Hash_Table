#include <stdlib.h>
#include <stdbool.h>

#include "article.h"
#include "hashtable.h"

typedef unsigned long ht_index_t;
static const int HT_INITIAL_CAPACITY = 13;
static const int HT_KEY_NOT_FOUND = -1;

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

HashTable_t* ht_new(void)
{
	HashTable_t* const new_table = (HashTable_t*)malloc(sizeof(HashTable_t));

	new_table->count = 0;
	new_table->capacity = HT_INITIAL_CAPACITY;

	new_table->items = (Article_t**)malloc(new_table->capacity * sizeof(Article_t*));
	new_table->states = (CellState_t*)malloc(new_table->capacity * sizeof(CellState_t));

	for (ht_index_t i = 0; i < new_table->capacity; ++i)
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

static unsigned long const HASH_FUNCTION_A = 31415, HASH_FUNCTION_B = 27183;

ht_index_t ht_hash_key(const HashTable_t* const ht, const char* key)
{
	ht_index_t candidate_index = 0, multiplier = HASH_FUNCTION_A;

	for (; *key != '\0'; ++key)
	{
		candidate_index = (multiplier * candidate_index + *key) % ht->capacity;
		multiplier = (multiplier * HASH_FUNCTION_B) % (ht->capacity - 1);
	}

	return candidate_index;
}

bool cell_at_index_has_key(const HashTable_t* ht, const ht_index_t i, const char* const key)
{
	return ht->states[i] == FILLED && article_has_key(ht->items[i], key);
}

ht_index_t next_index_in_cycle(const HashTable_t* const ht, const ht_index_t i)
{
	return (i + 1) % ht->capacity;
}

ht_index_t find_index_of_key(const HashTable_t* const ht, const char* const key)
{
	ht_index_t const hashed_index = ht_hash_key(ht, key);
	ht_index_t current_index = hashed_index;

	do
	{
		if (ht->states[current_index] == OPEN)
			return HT_KEY_NOT_FOUND;

		if (cell_at_index_has_key(ht, current_index, key))
			return current_index;

		current_index = next_index_in_cycle(ht, current_index);

	} while (current_index != hashed_index);

	return HT_KEY_NOT_FOUND;
}

bool ht_contains(const HashTable_t* const ht, const char* key)
{
	return find_index_of_key(ht, key) != HT_KEY_NOT_FOUND;
}

void insert_item_at_index(HashTable_t* const ht, const Article_t* const article, const ht_index_t i)
{
	ht->items[i] = duplicate_article(article);
	ht->states[i] = FILLED;
	ht->count++;
}

void replace_item_at_index(HashTable_t* const ht, const Article_t* const article, const ht_index_t i)
{
	delete_article(ht->items[i]);
	ht->items[i] = duplicate_article(article);
}

void ht_insert(HashTable_t* const ht, const char* const key, const Article_t* const article)
{
	ht_index_t const hashed_index = ht_hash_key(ht, key);
	ht_index_t current_index = hashed_index;

	do
	{
		if (ht->states[current_index] == OPEN)
			return insert_item_at_index(ht, article, current_index);

		if (cell_at_index_has_key(ht, current_index, key))
			return replace_item_at_index(ht, article, current_index);

		current_index = next_index_in_cycle(ht, current_index);

	} while (current_index != hashed_index);
}

unsigned long ht_count(const HashTable_t* const ht)
{
	return ht->count;
}

const Article_t* ht_fetch(const HashTable_t* const ht, const char* const key)
{
	const ht_index_t i = find_index_of_key(ht, key);
	return i != HT_KEY_NOT_FOUND ? ht->items[i] : NULL;
}

void remove_item_at_index(HashTable_t* const ht, const ht_index_t i)
{
	delete_article(ht->items[i]);
	ht->states[i] = REMOVED;
	ht->count--;
}

void ht_remove(HashTable_t* const ht, const char* const key)
{
	const ht_index_t i = find_index_of_key(ht, key);

	if (i != HT_KEY_NOT_FOUND)
		remove_item_at_index(ht, i);
}
