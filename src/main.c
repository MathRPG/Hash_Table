#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "article.h"
#include "hashtable.h"

int main(void)
{
	HashTable_t* ht = ht_create(5lu);

	assert(ht != NULL);
	assert(ht_is_empty(ht) == true);
	assert(ht_count(ht) == 0);
	assert(ht_contains(ht, "DOI") == false);

	Article_t* a = make_article("DOI", "Title", "Author", 2000u);
	ht_insert(ht, a);
	delete_article(a);

	assert(ht_is_empty(ht) == false);
	assert(ht_count(ht) == 1);
	assert(ht_contains(ht, "DOI") == true);

	Article_t* b = make_article("Other_DOI", "Title", "Author", 2000u);
	ht_insert(ht, b);
	delete_article(b);

	assert(ht_is_empty(ht) == false);
	assert(ht_count(ht) == 2);

	ht_delete(ht);

	printf("✔ All Tests Passed!");

	return 0;
}

