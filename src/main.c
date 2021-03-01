#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "article.h"
#include "hashtable.h"

int main(void)
{
	struct HashTable* ht = ht_create(5lu);

	assert(ht != NULL);

	struct Article a = { "DOI", "Title", "Author", 2000 };

	assert(ht_is_empty(ht) == true);
	assert(ht_contains(ht, "DOI") == false);

	ht_insert(ht, a);

	assert(ht_contains(ht, "DOI") == true);

	ht_delete(ht);

	printf("✔ All Tests Passed!");

	return 0;
}

