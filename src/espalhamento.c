#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/espalhamento.h"

static const unsigned short HASH_MIN_N = 4;
static const unsigned short HASH_MAX_N = 50;

static const int A = 31415;
static const int B = 27183;
static const double HT_LOW_DENSITY = 0.25;
static const double HT_HIGH_DENSITY = 0.75;
Item_t* alloc_table_items(unsigned long initial_capacity)
{
	return (Item_t*)malloc(sizeof(Item_t) * initial_capacity);
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

HT_STATUS_FLAG ht_init(HashTable_t* table)
{
	unsigned long initial_capacity;

	// Inicia a tabela com espaco para 2^HASH_MIN_N - delta posicoes
	if (!calcula_primo_proximo_2aN(HASH_MIN_N, &initial_capacity))
	{
		return HT_FAILURE;
	}

	table->N = HASH_MIN_N;
	table->capacity = initial_capacity;
	table->items = alloc_table_items(initial_capacity);

	table->states = alloc_table_states(initial_capacity);
	init_table_states(table, initial_capacity);

	table->count = 0;
	return HT_SUCCESS;
}

HT_STATUS_FLAG ht_clear(HashTable_t* table)
{
	free(table->items);
	free(table->states);
	table->capacity = 0;
	table->count = 0;
	table->N = 0;
	return HT_SUCCESS;
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
	return ht_density(table) > HT_HIGH_DENSITY;
}

bool ht_has_available_space(const HashTable_t* table)
{
	return table->count != table->capacity;
}

bool ht_attempt_expansion(HashTable_t* table)
{
	return (ht_expand(table) || ht_has_available_space(table));
}

void ht_insert_item_at_index(HashTable_t* table, const Item_t* item, const unsigned long cell_index)
{
	memcpy(&table->items[cell_index], item, sizeof(Item_t));
	table->states[cell_index] = OCCUPIED;
	table->count++;
}

unsigned long find_cell_index_for_insertion(const HashTable_t* table, Item_t* registro)
{
	unsigned long candidate_cell_index = ht_hash_string(registro->name, table->capacity);

	while (table->states[candidate_cell_index] == OCCUPIED)
	{
		candidate_cell_index = (candidate_cell_index + 1) % table->capacity;
	}

	return candidate_cell_index;
}

HT_STATUS_FLAG ht_insert(HashTable_t* table, Item_t* item)
{
	if (ht_has_high_density(table))
	{
		if (!ht_attempt_expansion(table)) // TODO: more explicit comparison
		{
			return HT_FAILURE;
		}
	}

	const unsigned long cell_index = find_cell_index_for_insertion(table, item);
	ht_insert_item_at_index(table, item, cell_index);

	return HT_SUCCESS;
}

bool str_eq(const char* str1, const char* str2)
{
	return strcmp(str1, str2) == 0;
}

unsigned long next_candidate_index(const HashTable_t* table, unsigned long candidate_index)
{
	return (candidate_index + 1) % table->capacity;
}

HT_STATUS_FLAG ht_search(HashTable_t* table, Item_t* item)
{
	unsigned long candidate_index = ht_hash_string(item->name, table->capacity);
	const unsigned long initial_guess = candidate_index;

	while (table->states[candidate_index] != OPEN)
	{
		if ((table->states[candidate_index] == OCCUPIED) && (str_eq(item->name, table->items[candidate_index].name)))
		{
			memcpy(item, &table->items[candidate_index], sizeof(Item_t));
			return HT_SUCCESS;
		}

		candidate_index = next_candidate_index(table, candidate_index);

		if (candidate_index == initial_guess)
		{
			return HT_FAILURE;
		}
	}

	return HT_FAILURE;
}

bool ht_has_low_density(HashTable_t* table)
{
	return ht_density(table) < HT_LOW_DENSITY;
}

HT_STATUS_FLAG ht_remove_item(HashTable_t* table, Item_t* item)
{
	if (table->count == 0)
	{
		return HT_FAILURE;
	}

	unsigned long candidate_index = ht_hash_string(item->name, table->capacity);
	const unsigned long initial_guess = candidate_index;

	while (table->states[candidate_index] != OPEN)
	{
		if ((table->states[candidate_index] == OCCUPIED) && str_eq(item->name, table->items[candidate_index].name))
		{
			table->states[candidate_index] = REMOVED;
			table->count--;

			if (ht_has_low_density(table))
			{
				ht_shrink(table);
			}

			return HT_SUCCESS;
		}

		candidate_index = next_candidate_index(table, candidate_index);

		if (candidate_index == initial_guess)
		{
			return HT_FAILURE;
		}
	}

	return HT_FAILURE;
}

double ht_density(HashTable_t* table)
{
	return (double)table->count / (double)table->capacity;
}

HT_STATUS_FLAG ht_expand(HashTable_t* table)
{
	if (table->N == HASH_MAX_N)
	{
		return HT_FAILURE;
	}

	// Guarda os componentes da tabela anterior
	unsigned long old_capacity = table->capacity;
	Item_t* old_items = table->items;
	SlotState_t* old_states = table->states;

	// Calcula e aloca componentes da tabela expandida
	unsigned short new_N = table->N + 1;

	unsigned long new_capacity;

	if (!calcula_primo_proximo_2aN(new_N, &new_capacity))
	{
		return HT_FAILURE;
	}

	table->states = alloc_table_states(new_capacity);
	init_table_states(table, new_capacity);
	table->items = alloc_table_items(new_capacity);
	table->count = 0;
	table->capacity = new_capacity;
	table->N = new_N;

	// percorrer a tabela antiga procurando chaves e inserindo na tabela nova
	for (unsigned long slot = 0; slot < old_capacity; slot++)
	{
		if (old_states[slot] == OCCUPIED)
		{
			ht_insert(table, &old_items[slot]);
		}
	}

	free(old_items);
	free(old_states);

	return HT_SUCCESS;
}

HT_STATUS_FLAG ht_shrink(HashTable_t* table)
{
	unsigned long novo_M, M_antigo;
	unsigned short novo_N;

	Item_t* novas_chaves, * chaves_antigas;
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
	if (novo_M < table->count)
	{
		return false;
	}
	novas_chaves = (Item_t*)malloc(sizeof(Item_t) * novo_M);
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

HT_STATUS_FLAG calcula_primo_proximo_2aN(unsigned short N, unsigned long* primo)
{
	// https://primes.utm.edu/lists/2small/0bit.html
	// https://en.wikipedia.org/wiki/List_of_prime_numbers
	//
	// 2^n - delta[] eh um primo, com HASH_MIN_N <= n <= HASH_MAX_N
	static const unsigned short deltas[] = {
			3, 1, 3, 1, 5, 3, 3,
			9, 3, 1, 3, 19, 15, 1, 5, 1, 3,
			9, 3, 15, 3, 39, 5, 39, 57, 3, 35,
			1, 5, 9, 41, 31, 5, 25, 45, 7, 87,
			21, 11, 57, 17, 55, 21, 115, 59, 81, 27 };

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
	printf("Slots ocupados (%%)...: %10lu (%.2f%%)\n", table->count, ht_density(table));
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



