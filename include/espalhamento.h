#ifndef ESPALHAMENTO_ESPALHAMENTO_H
#define ESPALHAMENTO_ESPALHAMENTO_H

#include <stdbool.h>

typedef struct registro
{
	char name[30];
	char email[50];
} Registro_t;

typedef enum
{
	OPEN, OCCUPIED, REMOVED
} SlotState_t;

typedef struct hash_table
{
	Registro_t* items;
	SlotState_t* states;
	unsigned long capacity;
	unsigned long count;
	unsigned short N;
} HashTable_t;

static const int A = 31415;
static const int B = 27183;
static const double HT_HIGH_DENSITY = 0.75;
static const bool HT_FAILURE = false;
static const bool HT_SUCCESS = true;
bool ht_init(HashTable_t* table);
bool ht_clear(HashTable_t* table);
bool ht_insert(HashTable_t* table, Registro_t* registro);
bool apagar_da_tabela_hash(HashTable_t* table, Registro_t* registro);
bool ht_search(HashTable_t* table, Registro_t* registro);

void ocupacao_da_tabela_hash(HashTable_t* table);

unsigned long tamanho_da_tabela_hash(HashTable_t* table);
unsigned long registros_na_tabela_hash(HashTable_t* table);
double densidade_da_tabela_hash(HashTable_t* table);
bool ht_expand(HashTable_t* table);
bool encolher_tabela_hash(HashTable_t* table);
bool calcula_primo_proximo_2aN(unsigned short N, unsigned long* primo);

unsigned long ht_hash_string(char* key, unsigned long table_capacity);

#endif
