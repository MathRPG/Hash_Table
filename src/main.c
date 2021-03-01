#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "article.h"
#include "hashtable.h"

void test_hash_table_single_article()
{
	HashTable_t* const ht = ht_create();
	const char* const doi = "DOI";
	const char* const other_doi = "Other_DOI";

	// Properties of an empty ht
	assert(ht != NULL);
	assert(ht_is_empty(ht) == true);
	assert(ht_count(ht) == 0);
	// All searches must yield failure
	assert(ht_contains(ht, doi) == false);
	assert(ht_fetch(ht, doi) == NULL);

	// Testing inserting one article
	Article_t* a = make_article(doi, "Title", "Author", 2000u);
	ht_insert(ht, a);

	assert(ht_is_empty(ht) == false);
	assert(ht_count(ht) == 1);

	// Search for inserted
	assert(ht_contains(ht, doi) == true);
	const Article_t* fetched = ht_fetch(ht, doi);
	assert(fetched != NULL);
	assert(articles_are_equal(a, fetched) == true);

	// Search for non-inserted
	assert(ht_contains(ht, other_doi) == false);
	assert(ht_fetch(ht, other_doi) == NULL);

	// Testing removal of non-existent value
	ht_remove(ht, other_doi);

	assert(ht_is_empty(ht) == false);
	assert(ht_count(ht) == 1);

	assert(ht_contains(ht, doi) == true);
	assert(ht_fetch(ht, doi) != NULL);

	// Testing removal of value inserted
	ht_remove(ht, doi);

	assert(ht_is_empty(ht) == true);
	assert(ht_count(ht) == 0);

	assert(ht_contains(ht, doi) == false);
	assert(ht_fetch(ht, doi) == NULL);

	delete_article(a);
	ht_delete(ht);
}

int main(void)
{
	test_hash_table_single_article();

	printf("✔ All Tests Passed!");

	return 0;
}

