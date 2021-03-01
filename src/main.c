#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "hashtable.h"

int main(void)
{
	HashTable_t* ht = ht_create(5lu);

	assert(ht != NULL);

	Article_t* a = make_article("DOI", "Title", "Author", 2000u);

	assert(ht_is_empty(ht) == true);
	assert(ht_contains(ht, "DOI") == false);

	ht_insert(ht, a);

	assert(ht_contains(ht, "DOI") == true);

	delete_article(a);
	ht_delete(ht);

	printf("✔ All Tests Passed!");

	return 0;
}

