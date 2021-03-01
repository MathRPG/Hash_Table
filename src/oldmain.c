#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/espalhamento.h"

int main()
{
	HashTable_t tabela;
	Item_t p, q;

	if (!ht_init(&tabela))
	{
		printf("Erro na criacao da tabela\n");
		exit(1);
	}

	ht_print(&tabela);

	strcpy(p.name, "Chicoh");
	strcpy(p.email, "chicoh@ceu.org.br");

	if (!ht_insert(&tabela, &p))
	{
		printf("Falha na insercao\n");
		exit(1);
	}
	else
	{
		printf("Inserido\n");
	}

	strcpy(p.name, "Joao Grilo");
	strcpy(p.email, "joaogrilo@ceu.org.br");

	if (!ht_insert(&tabela, &p))
	{
		printf("Falha na insercao\n");
		exit(1);
	}
	else
	{
		printf("Inserido\n");
	}

	strcpy(p.name, "Fernando");
	strcpy(p.email, "fernando@andrade.org.br");

	if (!ht_insert(&tabela, &p))
	{
		printf("Falha na insercao\n");
		exit(1);
	}
	else
	{
		printf("Inserido\n");
	}

	// Inserir chave duplicada para produzir colisao
	if (!ht_insert(&tabela, &p))
	{
		printf("Falha na insercao\n");
		exit(1);
	}
	else
	{
		printf("Inserido\n");
	}

	ht_print(&tabela);

	// Recuperando uma chave que existe
	strcpy(q.name, "Joao Grilo");
	if (!ht_search(&tabela, &q))
	{
		printf("Erro na recuperacao\n");
	}
	else
	{
		printf("O email de %s eh %s\n", q.name, q.email);
	}

	// Recuperando uma chave que existe
	strcpy(q.name, "Fernando");
	if (!ht_search(&tabela, &q))
	{
		printf("Erro na recuperacao\n");
	}
	else
	{
		printf("O email de %s eh %s\n", q.name, q.email);
	}

	// Recuperando uma chave que nao existe
	strcpy(q.name, "Raimundo Nonato");
	if (!ht_search(&tabela, &q))
	{
		printf("Erro na recuperacao\n");
	}
	else
	{
		printf("O email de %s eh %s\n", q.name, q.email);
	}

	// Removendo uma chave que existe
	strcpy(q.name, "Chicoh");
	if (ht_remove_item(&tabela, &q))
	{
		printf("%s removido\n", q.name);
	}
	else
	{
		printf("%s nao existe na tabela\n", q.name);
	}

	// Removendo uma chave que existe
	strcpy(q.name, "Fernando");
	if (ht_remove_item(&tabela, &q))
	{
		printf("%s removido\n", q.name);
	}
	else
	{
		printf("%s nao existe na tabela\n", q.name);
	}

	// Recuperando uma chave que existe
	strcpy(q.name, "Fernando");
	if (!ht_search(&tabela, &q))
	{
		printf("Erro na recuperacao\n");
	}
	else
	{
		printf("O email de %s eh %s\n", q.name, q.email);
	}

	// Removendo uma chave que nao existe
	strcpy(q.name, "Raimundo Nonato");
	if (ht_remove_item(&tabela, &q))
	{
		printf("%s removido\n", q.name);
	}
	else
	{
		printf("%s nao existe na tabela\n", q.name);
	}

	ht_print(&tabela);

	printf("Expandindo a tabela...\n");
	ht_expand(&tabela);

	ht_print(&tabela);

	if (ht_clear(&tabela))
	{
		printf("Tabela destruida\n");
	}
	else
	{
		printf("Problema para destruir a tabela\n");
	}

	return 0;
}