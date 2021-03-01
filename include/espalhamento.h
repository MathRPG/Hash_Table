#ifndef ESPALHAMENTO_ESPALHAMENTO_H
#define ESPALHAMENTO_ESPALHAMENTO_H

#include <stdbool.h>

typedef struct registro
{
	char name[30];
	char email[50];
} Item_t;

typedef enum
{
	OPEN, FILLED, REMOVED
} SlotState_t;

typedef struct hash_table
{
	Item_t* items;
	SlotState_t* states;
	unsigned long capacity;
	unsigned long count;
	unsigned short capacity_exponent;
} HashTable_t;

typedef int HT_STATUS_FLAG;
static HT_STATUS_FLAG HT_FAILURE = 0;
static HT_STATUS_FLAG HT_SUCCESS = 1;

HT_STATUS_FLAG ht_init(HashTable_t* table);
HT_STATUS_FLAG ht_clear(HashTable_t* table);
HT_STATUS_FLAG ht_insert(HashTable_t* table, Item_t* item);
HT_STATUS_FLAG ht_remove_item(HashTable_t* table, Item_t* item);
HT_STATUS_FLAG ht_search(HashTable_t* table, Item_t* item);

double ht_density(HashTable_t* table);
HT_STATUS_FLAG ht_expand(HashTable_t* table);
HT_STATUS_FLAG ht_shrink(HashTable_t* table);
HT_STATUS_FLAG ht_get_appropriate_capacity_from_capacity_exponent(unsigned short capacity_exponent, unsigned long* prime_capacity);

unsigned long ht_hash_string(char* key, unsigned long table_capacity);

void ht_print(HashTable_t* table);

#endif
