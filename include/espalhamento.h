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
	Registro_t* keys;
	SlotState_t* states;
	unsigned long tamanho;
	unsigned long ocupados;
	unsigned short N;
} HashTable_t;

bool criar_tabela_hash(HashTable_t* table);
bool destruir_tabela_hash(HashTable_t* table);
bool inserir_na_tabela_hash(HashTable_t* table, Registro_t* registro);
bool apagar_da_tabela_hash(HashTable_t* table, Registro_t* registro);
bool busca_na_tabela_hash(HashTable_t* table, Registro_t* registro);

void ocupacao_da_tabela_hash(HashTable_t* table);

unsigned long tamanho_da_tabela_hash(HashTable_t* table);
unsigned long registros_na_tabela_hash(HashTable_t* table);
double densidade_da_tabela_hash(HashTable_t* table);
bool expandir_tabela_hash(HashTable_t* table);
bool encolher_tabela_hash(HashTable_t* table);
bool calcula_primo_proximo_2aN(unsigned short N, unsigned long* primo);

unsigned long calcular_valor_do_hash(char* key, unsigned long M);

#endif
