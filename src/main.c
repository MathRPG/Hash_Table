#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/espalhamento.h"

int main()
{
	HashTable_t tabela;
	Registro_t p, q;

	if (!criar_tabela_hash(&tabela))
	{
		printf("Erro na criacao da tabela\n");
		exit(1);
	}

	ocupacao_da_tabela_hash(&tabela);

	strcpy(p.name, "Chicoh");
	strcpy(p.email, "chicoh@ceu.org.br");

	if (!inserir_na_tabela_hash(&tabela, &p))
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

	if (!inserir_na_tabela_hash(&tabela, &p))
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

	if (!inserir_na_tabela_hash(&tabela, &p))
	{
		printf("Falha na insercao\n");
		exit(1);
	}
	else
	{
		printf("Inserido\n");
	}

	// Inserir chave duplicada para produzir colisao
	if (!inserir_na_tabela_hash(&tabela, &p))
	{
		printf("Falha na insercao\n");
		exit(1);
	}
	else
	{
		printf("Inserido\n");
	}

	ocupacao_da_tabela_hash(&tabela);

	// Recuperando uma chave que existe
	strcpy(q.name, "Joao Grilo");
	if (!busca_na_tabela_hash(&tabela, &q))
	{
		printf("Erro na recuperacao\n");
	}
	else
	{
		printf("O email de %s eh %s\n", q.name, q.email);
	}

	// Recuperando uma chave que existe
	strcpy(q.name, "Fernando");
	if (!busca_na_tabela_hash(&tabela, &q))
	{
		printf("Erro na recuperacao\n");
	}
	else
	{
		printf("O email de %s eh %s\n", q.name, q.email);
	}

	// Recuperando uma chave que nao existe
	strcpy(q.name, "Raimundo Nonato");
	if (!busca_na_tabela_hash(&tabela, &q))
	{
		printf("Erro na recuperacao\n");
	}
	else
	{
		printf("O email de %s eh %s\n", q.name, q.email);
	}

	// Removendo uma chave que existe
	strcpy(q.name, "Chicoh");
	if (apagar_da_tabela_hash(&tabela, &q))
	{
		printf("%s removido\n", q.name);
	}
	else
	{
		printf("%s nao existe na tabela\n", q.name);
	}

	// Removendo uma chave que existe
	strcpy(q.name, "Fernando");
	if (apagar_da_tabela_hash(&tabela, &q))
	{
		printf("%s removido\n", q.name);
	}
	else
	{
		printf("%s nao existe na tabela\n", q.name);
	}

	// Recuperando uma chave que existe
	strcpy(q.name, "Fernando");
	if (!busca_na_tabela_hash(&tabela, &q))
	{
		printf("Erro na recuperacao\n");
	}
	else
	{
		printf("O email de %s eh %s\n", q.name, q.email);
	}

	// Removendo uma chave que nao existe
	strcpy(q.name, "Raimundo Nonato");
	if (apagar_da_tabela_hash(&tabela, &q))
	{
		printf("%s removido\n", q.name);
	}
	else
	{
		printf("%s nao existe na tabela\n", q.name);
	}

	ocupacao_da_tabela_hash(&tabela);

	printf("Expandindo a tabela...\n");
	expandir_tabela_hash(&tabela);

	ocupacao_da_tabela_hash(&tabela);

	if (destruir_tabela_hash(&tabela))
	{
		printf("Tabela destruida\n");
	}
	else
	{
		printf("Problema para destruir a tabela\n");
	}

	return 0;
}
