#include "article.h"
#include "hashtable.h"

#include <stdbool.h>

struct HashTable_s
{
	unsigned long count;
	Article_t* article;
};

HashTable_t* ht_create(void)
{
	HashTable_t* new_table = (HashTable_t*)calloc(1, sizeof(HashTable_t));
	new_table->article = make_article("", "", "", 0);
	return new_table;
}

void ht_delete(HashTable_t* ht)
{
	delete_article(ht->article);
	free(ht);
}

bool ht_is_empty(HashTable_t* ht)
{
	return ht->count == 0;
}

bool ht_contains(HashTable_t* ht, const char* string)
{
	return ht->count != 0;
}

void ht_insert(HashTable_t* ht, Article_t* article)
{
	copy_article(ht->article, article);
	ht->count++;
}

unsigned long ht_count(HashTable_t* ht)
{
	return ht->count;
}

const Article_t* ht_fetch(HashTable_t* const ht, const char* const key)
{
	return article_has_key(ht->article, key) ? ht->article : NULL;
}