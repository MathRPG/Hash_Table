#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/espalhamento.h"

static const unsigned short HASH_MIN_N = 4;
static const unsigned short HASH_MAX_N = 50;

Registro_t* allocate_registros(unsigned long initial_size)
{
	return (Registro_t*)malloc(sizeof(Registro_t) * initial_size);
}

SlotState_t* allocate_slot_states(unsigned long initial_size)
{
	return (SlotState_t*)malloc(sizeof(SlotState_t) * initial_size);
}

bool criar_tabela_hash(HashTable_t* table)
{
	unsigned long initial_size;

	// Inicia a tabela com espaco para 2^HASH_MIN_N - delta posicoes
	if (!calcula_primo_proximo_2aN(HASH_MIN_N, &initial_size))
	{
		return false;
	}

	table->N = HASH_MIN_N;
	table->size = initial_size;
	table->keys = allocate_registros(initial_size);
	table->states = allocate_slot_states(initial_size);
	for (unsigned int slot = 0; slot < initial_size; slot++)
	{
		table->states[slot] = OPEN;
	}
	table->ocupados = 0;
	return true;
}

bool destruir_tabela_hash(HashTable_t* table)
{
	free(table->keys);
	free(table->states);
	table->size = 0;
	table->ocupados = 0;
	table->N = 0;
	return true;
}

unsigned long calcular_valor_do_hash(char* key, unsigned long M)
{
	unsigned long h, a = 31415, b = 27183;
	for (h = 0; *key != '\0'; key++)
	{
		h = (a * h + *key) % M;
		a = (a * b) % (M - 1);
	}
	return h;
}

bool inserir_na_tabela_hash(HashTable_t* table, Registro_t* registro)
{
	unsigned long h;

	// Se estiver com densidade alta (0.75?), vamos ampliar a tabela
	if (densidade_da_tabela_hash(table) > 0.75)
	{
		// Se nao conseguir expandir e nao tiver mais espaco, aborta.
		if (!expandir_tabela_hash(table) && (table->ocupados == table->size))
		{
			return false;
		}
	}

	// A tabela tem espaco para insercao da chave
	h = calcular_valor_do_hash(registro->name, table->size);
	while (table->states[h] == OCCUPIED)
	{
		h = (h + 1) % table->size;
	}
	memcpy(&table->keys[h], registro, sizeof(Registro_t));
	table->states[h] = OCCUPIED;
	table->ocupados++;
	return true;
}

bool busca_na_tabela_hash(HashTable_t* table, Registro_t* registro)
{
	unsigned long h, h_0;

	h = calcular_valor_do_hash(registro->name, table->size);
	h_0 = h;
	while (table->states[h] != OPEN)
	{
		if ((table->states[h] == OCCUPIED) && (strcmp(registro->name, table->keys[h].name) == 0))
		{
			memcpy(registro, &table->keys[h], sizeof(Registro_t));
			return true;
		}
		h = (h + 1) % table->size;
		// demos a volta na tabela e nao achamos, entao false
		if (h == h_0)
		{
			break;
		}
	}
	return false;
}

bool apagar_da_tabela_hash(HashTable_t* table, Registro_t* registro)
{
	unsigned long h, h_0;

	if (table->ocupados == 0)
	{
		return false;
	}

	h = calcular_valor_do_hash(registro->name, table->size);
	h_0 = h;
	while (table->states[h] != OPEN)
	{
		if ((table->states[h] == OCCUPIED) && (strcmp(registro->name, table->keys[h].name) == 0))
		{
			table->states[h] = REMOVED;
			table->ocupados--;
			if (densidade_da_tabela_hash(table) < 0.25)
			{
				encolher_tabela_hash(table);
			}
			return true;
		}
		h = (h + 1) % table->size;
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
	return (table->size);
}

unsigned long registros_na_tabela_hash(HashTable_t* table)
{
	return (table->ocupados);
}

double densidade_da_tabela_hash(HashTable_t* table)
{
	return ((double)registros_na_tabela_hash(table) / tamanho_da_tabela_hash(table));
}

bool expandir_tabela_hash(HashTable_t* table)
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
	M_antigo = table->size;
	chaves_antigas = table->keys;
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
	table->keys = novas_chaves;
	table->states = novos_estados;
	table->ocupados = 0;
	table->size = novo_M;
	table->N = novo_N;

	// percorrer a tabela antiga procurando chaves e inserindo na tabela nova
	for (unsigned long slot = 0; slot < M_antigo; slot++)
	{
		if (estados_antigos[slot] == OCCUPIED)
		{
			inserir_na_tabela_hash(table, &chaves_antigas[slot]);
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
	M_antigo = table->size;
	chaves_antigas = table->keys;
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
	table->keys = novas_chaves;
	table->states = novos_estados;
	table->ocupados = 0;
	table->size = novo_M;
	table->N = novo_N;

	// percorrer a tabela antiga procurando chaves e inserindo na tabela nova
	for (unsigned long slot = 0; slot < M_antigo; slot++)
	{
		if (estados_antigos[slot] == OCCUPIED)
		{
			inserir_na_tabela_hash(table, &chaves_antigas[slot]);
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
	printf("Capacidade total.....: %10lu\n", table->size);
	printf("Valor de N...........: %10u\n", table->N);
	printf("Slots ocupados (%%)...: %10lu (%.2f%%)\n", table->ocupados, densidade_da_tabela_hash(table));
	putchar('\n');
	printf("(.) Posicao vazia\n");
	printf("(o) Posicao vazia, mas que ja foi ocupada\n");
	printf("(H) Posicao ocupada por registro na posicao correta\n");
	printf("(C) Posicao ocupada por registro relocado\n");
	printf("-------------------------------------------------------------\n");
	printf("           01234567890123456789012345678901234567890123456789");
	for (unsigned long slot = 0; slot < table->size; slot++)
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
			h = calcular_valor_do_hash(table->keys[slot].name, table->size);
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



