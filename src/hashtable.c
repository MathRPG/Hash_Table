#include <stdlib.h>
#include <stdbool.h>

#include "article.h"
#include "hashtable.h"

typedef unsigned long ht_index_t;
static const int HT_KEY_NOT_FOUND = -1;

typedef enum HashTableCellState CellState_t;

enum HashTableCellState
{
	OPEN, OCCUPIED, REMOVED
};

struct HashTable_s
{
	ht_index_t count;
	unsigned short capacity_index;
	ht_index_t capacity;
	Article_t** items;
	CellState_t* states;
};

// Deltas are such that 2 ^ (index + 4) - deltas[index] is a prime
// Keeping capacity as a prime is good for hash function dispersion
static const unsigned short capacity_deltas[] =
		{
				3, 1, 3, 1, 5, 3, 3,
				9, 3, 1, 3, 19, 15, 1, 5, 1, 3,
				9, 3, 15, 3, 39, 5, 39, 57, 3, 35,
				1, 5, 9, 41, 31, 5, 25, 45, 7, 87,
				21, 11, 57, 17, 55, 21, 115, 59, 81, 27
		};

static const unsigned long HT_MAXIMUM_CAPACITY_INDEX = sizeof capacity_deltas / sizeof *capacity_deltas;

unsigned long calculate_optimal_capacity_for_index(unsigned short index)
{
	return (1lu << (index + 4)) - capacity_deltas[index];
}

void alloc_and_init_items_and_states(HashTable_t* const ht)
{
	ht->items = (Article_t**)malloc(ht->capacity * sizeof(Article_t*));
	ht->states = (CellState_t*)malloc(ht->capacity * sizeof(CellState_t));

	for (ht_index_t i = 0; i < ht->capacity; ++i)
	{
		ht->items[i] = NULL;
		ht->states[i] = OPEN;
	}
}

void delete_and_free_items_and_states(HashTable_t* const ht)
{
	for (ht_index_t i = 0; i < ht->capacity; ++i)
		if (ht->states[i] == OCCUPIED)
			delete_article(ht->items[i]);

	free(ht->items);
	free(ht->states);
}

void ht_resize(HashTable_t* const ht, const ht_index_t new_capacity)
{
	if (new_capacity == 0 || new_capacity < ht->count)
		return;

	HashTable_t old_table = *ht;

	// EXPAND DONG
	ht->capacity = new_capacity;
	alloc_and_init_items_and_states(ht);

	for (ht_index_t i = 0, transferred = 0;
		 i < old_table.capacity && transferred < old_table.count; ++i)
	{
		if (old_table.states[i] == OCCUPIED)
		{
			ht_insert(ht, old_table.items[i]);
			transferred++;
		}
	}

	delete_and_free_items_and_states(&old_table);
}

static const double HT_LOW_DENSITY_BOUND = 0.25;
static const double HT_HIGH_DENSITY_BOUND = 0.75;

HashTable_t* ht_new(void)
{
	HashTable_t* const new_table = (HashTable_t*)malloc(sizeof(HashTable_t));

	new_table->count = 0;
	new_table->capacity_index = 0;
	new_table->capacity = calculate_optimal_capacity_for_index(0);

	alloc_and_init_items_and_states(new_table);

	return new_table;
}

void ht_delete(HashTable_t* const ht)
{
	delete_and_free_items_and_states(ht);
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
	return ht->states[i] == OCCUPIED && article_has_key(ht->items[i], key);
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
	ht->states[i] = OCCUPIED;
	ht->count++;
}

void replace_item_at_index(HashTable_t* const ht, const Article_t* const article, const ht_index_t i)
{
	delete_article(ht->items[i]);
	ht->items[i] = duplicate_article(article);
}

double ht_density(const HashTable_t* const ht)
{
	return ((double)ht->count) / ht->capacity;
}

void ht_expand(HashTable_t* const ht)
{
	if (ht->capacity_index != HT_MAXIMUM_CAPACITY_INDEX)
		ht_resize(ht, calculate_optimal_capacity_for_index(++ht->capacity_index));
}

void ht_insert(HashTable_t* const ht, const Article_t* const article)
{
	ht_index_t const hashed_index = ht_hash_key(ht, key_of(article));
	ht_index_t current_index = hashed_index;

	do
	{
		if (ht->states[current_index] == OPEN)
		{
			insert_item_at_index(ht, article, current_index);

			if (ht_density(ht) > HT_HIGH_DENSITY_BOUND)
				ht_expand(ht);

			return;
		}

		if (cell_at_index_has_key(ht, current_index, key_of(article)))
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

void ht_shrink(HashTable_t* const ht)
{
	if (ht->capacity_index != 0)
		ht_resize(ht, calculate_optimal_capacity_for_index(--ht->capacity_index));
}

void ht_remove(HashTable_t* const ht, const char* const key)
{
	const ht_index_t i = find_index_of_key(ht, key);

	if (i == HT_KEY_NOT_FOUND)
		return;

	remove_item_at_index(ht, i);

	if (ht_density(ht) < HT_LOW_DENSITY_BOUND)
		ht_shrink(ht);
}

unsigned long ht_capacity(const HashTable_t* ht)
{
	return ht->capacity;
}
