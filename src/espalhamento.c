#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/espalhamento.h"

static const unsigned short HASH_MIN_N = 4;
static const unsigned short HASH_MAX_N = 50;

Registro_t* alloc_table_items(unsigned long initial_capacity)
{
	return (Registro_t*)malloc(sizeof(Registro_t) * initial_capacity);
}

SlotState_t* alloc_table_states(unsigned long initial_capacity)
{
	return (SlotState_t*)malloc(sizeof(SlotState_t) * initial_capacity);
}

void init_table_states(HashTable_t* table, unsigned long initial_capacity)
{
	for (unsigned int slot = 0; slot < initial_capacity; slot++)
	{
		table->states[slot] = OPEN;
	}
}

bool ht_init(HashTable_t* table)
{
	unsigned long initial_capacity;

	// Inicia a tabela com espaco para 2^HASH_MIN_N - delta posicoes
	if (!calcula_primo_proximo_2aN(HASH_MIN_N, &initial_capacity))
	{
		return false;
	}

	table->N = HASH_MIN_N;
	table->capacity = initial_capacity;
	table->items = alloc_table_items(initial_capacity);

	table->states = alloc_table_states(initial_capacity);
	init_table_states(table, initial_capacity);

	table->count = 0;
	return true;
}

bool ht_clear(HashTable_t* table)
{
	free(table->items);
	free(table->states);
	table->capacity = 0;
	table->count = 0;
	table->N = 0;
	return true;
}

unsigned long ht_hash_string(char* key, unsigned long table_capacity)
{
	unsigned long total = 0, a = A;

	for (; *key != '\0'; key++)
	{
		total = (a * total + *key) % table_capacity;
		a = (a * B) % (table_capacity - 1);
	}

	return total;
}

bool ht_has_high_density(HashTable_t* table)
{
	return densidade_da_tabela_hash(table) > HT_HIGH_DENSITY;
}

bool ht_has_available_space(const HashTable_t* table)
{
	return table->count != table->capacity;
}

bool ht_attempt_expansion(HashTable_t* table)
{
	return (ht_expand(table) || ht_has_available_space(table));
}

void ht_insert_registro_at_index(HashTable_t* table, const Registro_t* registro, const unsigned long cell_index)
{
	memcpy(&table->items[cell_index], registro, sizeof(Registro_t));
	table->states[cell_index] = OCCUPIED;
	table->count++;
}

unsigned long find_cell_index_for_insertion(const HashTable_t* table, Registro_t* registro)
{
	unsigned long candidate_cell_index = ht_hash_string(registro->name, table->capacity);

	while (table->states[candidate_cell_index] == OCCUPIED)
	{
		candidate_cell_index = (candidate_cell_index + 1) % table->capacity;
	}
	return candidate_cell_index;
}

bool ht_insert(HashTable_t* table, Registro_t* registro)
{
	if (ht_has_high_density(table))
	{
		if (!ht_attempt_expansion(table)) // TODO: more explicit comparison
		{
			return HT_FAILURE;
		}
	}

	const unsigned long cell_index = find_cell_index_for_insertion(table, registro);
	ht_insert_registro_at_index(table, registro, cell_index);

	return HT_SUCCESS;
}

bool ht_search(HashTable_t* table, Registro_t* registro)
{
	unsigned long h, h_0;

	h = ht_hash_string(registro->name, table->capacity);
	h_0 = h;
	while (table->states[h] != OPEN)
	{
		if ((table->states[h] == OCCUPIED) && (strcmp(registro->name, table->items[h].name) == 0))
		{
			memcpy(registro, &table->items[h], sizeof(Registro_t));
			return HT_SUCCESS;
		}
		h = (h + 1) % table->capacity;
		// demos a volta na tabela e nao achamos, entao false
		if (h == h_0)
		{
			break;
		}
	}
	return HT_FAILURE;
}

bool apagar_da_tabela_hash(HashTable_t* table, Registro_t* registro)
{
	unsigned long h, h_0;

	if (table->count == 0)
	{
		return false;
	}

	h = ht_hash_string(registro->name, table->capacity);
	h_0 = h;
	while (table->states[h] != OPEN)
	{
		if ((table->states[h] == OCCUPIED) && (strcmp(registro->name, table->items[h].name) == 0))
		{
			table->states[h] = REMOVED;
			table->count--;
			if (densidade_da_tabela_hash(table) < 0.25)
			{
				encolher_tabela_hash(table);
			}
			return true;
		}
		h = (h + 1) % table->capacity;
		// demos a volta na tabela e nao achamos, entao false
		if (h == h_0)
		{
			break;
		}
	}
	return false;
}

unsigned long tamanho_da_tabela_hash(HashTable_t* table)
{
	return (table->capacity);
}

unsigned long registros_na_tabela_hash(HashTable_t* table)
{
	return (table->count);
}

double densidade_da_tabela_hash(HashTable_t* table)
{
	return ((double)registros_na_tabela_hash(table) / tamanho_da_tabela_hash(table));
}

bool ht_expand(HashTable_t* table)
{
	unsigned long novo_M, M_antigo;
	unsigned short novo_N;

	Registro_t* novas_chaves, * chaves_antigas;
	SlotState_t* novos_estados, * estados_antigos;

	if (table->N == HASH_MAX_N)
	{
		return false;
	}

	// Guarda os componentes da tabela anterior
	M_antigo = table->capacity;
	chaves_antigas = table->items;
	estados_antigos = table->states;

	// Calcula e aloca componentes da tabela expandida
	novo_N = table->N + 1;
	if (!calcula_primo_proximo_2aN(novo_N, &novo_M))
	{
		return false;
	}
	novas_chaves = (Registro_t*)malloc(sizeof(Registro_t) * novo_M);
	novos_estados = (SlotState_t*)malloc(sizeof(SlotState_t) * novo_M);
	for (unsigned int slot = 0; slot < novo_M; slot++)
	{
		novos_estados[slot] = OPEN;
	}

	// Tabela nova vazia
	table->items = novas_chaves;
	table->states = novos_estados;
	table->count = 0;
	table->capacity = novo_M;
	table->N = novo_N;

	// percorrer a tabela antiga procurando chaves e inserindo na tabela nova
	for (unsigned long slot = 0; slot < M_antigo; slot++)
	{
		if (estados_antigos[slot] == OCCUPIED)
		{
			ht_insert(table, &chaves_antigas[slot]);
		}
	}

	free(chaves_antigas);
	free(estados_antigos);

	return true;
}

bool encolher_tabela_hash(HashTable_t* table)
{
	unsigned long novo_M, M_antigo;
	unsigned short novo_N;

	Registro_t* novas_chaves, * chaves_antigas;
	SlotState_t* novos_estados, * estados_antigos;

	if (table->N == HASH_MIN_N)
	{
		return false;
	}

	// Guarda os componentes da tabela anterior
	M_antigo = table->capacity;
	chaves_antigas = table->items;
	estados_antigos = table->states;

	// Calcula e aloca componentes da tabela expandida
	novo_N = table->N - 1;
	// Ja estamos no menor tamanho?
	if (!calcula_primo_proximo_2aN(novo_N, &novo_M))
	{
		return false;
	}
	// Ha mais registros do que o numero de slots na nova tabela?
	if (novo_M < registros_na_tabela_hash(table))
	{
		return false;
	}
	novas_chaves = (Registro_t*)malloc(sizeof(Registro_t) * novo_M);
	novos_estados = (SlotState_t*)malloc(sizeof(SlotState_t) * novo_M);
	for (unsigned int slot = 0; slot < novo_M; slot++)
	{
		novos_estados[slot] = OPEN;
	}

	// Tabela nova vazia
	table->items = novas_chaves;
	table->states = novos_estados;
	table->count = 0;
	table->capacity = novo_M;
	table->N = novo_N;

	// percorrer a tabela antiga procurando chaves e inserindo na tabela nova
	for (unsigned long slot = 0; slot < M_antigo; slot++)
	{
		if (estados_antigos[slot] == OCCUPIED)
		{
			ht_insert(table, &chaves_antigas[slot]);
		}
	}

	// liberar os componentes da tabela antiga
	free(chaves_antigas);
	free(estados_antigos);

	return true;
}

bool calcula_primo_proximo_2aN(unsigned short N, unsigned long* primo)
{
	// https://primes.utm.edu/lists/2small/0bit.html
	// https://en.wikipedia.org/wiki/List_of_prime_numbers
	//
	// 2^n - delta[] eh um primo, com HASH_MIN_N <= n <= HASH_MAX_N
	static const unsigned short deltas[] = { 3, 1, 3, 1, 5, 3, 3, 9, 3, 1, 3, 19, 15, 1, 5, 1, 3, 9, 3, 15, 3, 39, 5,
											 39,
											 57, 3, 35, 1, 5, 9, 41, 31, 5, 25, 45, 7, 87, 21, 11, 57, 17, 55, 21, 115,
											 59,
											 81, 27 };

	if ((N < HASH_MIN_N) || (N > HASH_MAX_N))
	{
		return false;
	}

	*primo = (unsigned long)pow(2, N) - deltas[N - HASH_MIN_N];
	return true;
}

void ocupacao_da_tabela_hash(HashTable_t* table)
{
	unsigned long h;
	printf("--- Ocupacao da tabela ---------------------------------------\n");
	printf("Capacidade total.....: %10lu\n", table->capacity);
	printf("Valor de N...........: %10u\n", table->N);
	printf("Slots ocupados (%%)...: %10lu (%.2f%%)\n", table->count, densidade_da_tabela_hash(table));
	putchar('\n');
	printf("(.) Posicao vazia\n");
	printf("(o) Posicao vazia, mas que ja foi ocupada\n");
	printf("(H) Posicao ocupada por registro na posicao correta\n");
	printf("(C) Posicao ocupada por registro relocado\n");
	printf("-------------------------------------------------------------\n");
	printf("           01234567890123456789012345678901234567890123456789");
	for (unsigned long slot = 0; slot < table->capacity; slot++)
	{
		if (slot % 50 == 0)
		{
			putchar('\n');
			printf("%10lu ", slot);
		}
		switch (table->states[slot])
		{
		case OPEN:
			putchar('.');
			break;
		case REMOVED:
			putchar('o');
			break;
		case OCCUPIED:
			h = ht_hash_string(table->items[slot].name, table->capacity);
			if (h == slot)
			{
				putchar('H');
			}
			else
			{
				putchar('C');
			}
			break;
		}
	}
	putchar('\n');
	printf("           01234567890123456789012345678901234567890123456789\n");
	printf("-------------------------------------------------------------\n");
}



