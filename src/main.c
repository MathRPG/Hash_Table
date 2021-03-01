#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "article.h"
#include "hashtable.h"

void test_empty_hash_table()
{
	/*
	 * Tests properties of an empty hash table
	 * Initial values - not null, is_empty, count
	 * Operations - contains, fetch, remove
	 */
	
	HashTable_t * const ht = ht_create();
	const char* some_doi = "Some_DOI";

	// Initial values
	assert(ht != NULL);
	assert(ht_is_empty(ht) == true);
	assert(ht_count(ht) == 0);

	// All searches must yield 'failure'
	assert(ht_contains(ht, some_doi) == false);
	assert(ht_fetch(ht, some_doi) == NULL);

	// Remove must maintain table intact
	ht_remove(ht, some_doi);
	assert(ht_is_empty(ht) == true);
	assert(ht_count(ht) == 0);
	assert(ht_contains(ht, some_doi) == false);
	assert(ht_fetch(ht, some_doi) == NULL);

	// Cleanup
	ht_delete(ht);
}

void test_hash_table_single_article()
{
	/*
	 * Tests properties and operations of a hash table using at most one article
	 * Check if table is non-empty after insertion, if queries work as intended,
	 * and if it's back to empty after removal of inserted key
	 */

	HashTable_t* const ht = ht_create();
	const char* const inserted_key = "Some_DOI";
	const char* const not_inserted_key = "Other_DOI";

	Article_t* a = make_article(inserted_key, "Title", "Author", 2000u);
	ht_insert(ht, a);

	// Should not be empty after one insertion
	assert(ht_is_empty(ht) == false);
	assert(ht_count(ht) == 1);

	// Queries must yield success for inserted key
	assert(ht_contains(ht, inserted_key) == true);
	const Article_t* fetched = ht_fetch(ht, inserted_key);
	assert(fetched != NULL);
	assert(articles_are_equal(a, fetched) == true);

	// Queries must yield failure for not-inserted key
	assert(ht_contains(ht, not_inserted_key) == false);
	assert(ht_fetch(ht, not_inserted_key) == NULL);

	// Remove of not-inserted key must maintain table intact
	ht_remove(ht, not_inserted_key);
	assert(ht_is_empty(ht) == false);
	assert(ht_count(ht) == 1);
	assert(ht_contains(ht, inserted_key) == true);
	assert(ht_fetch(ht, inserted_key) != NULL);

	// Remove of inserted key must regress table back to being empty
	ht_remove(ht, inserted_key);
	assert(ht_is_empty(ht) == true);
	assert(ht_count(ht) == 0);
	assert(ht_contains(ht, inserted_key) == false);
	assert(ht_fetch(ht, inserted_key) == NULL);

	// Cleanup
	delete_article(a);
	ht_delete(ht);
}

void test_hash_table_multiple_articles()
{

}

int main(void)
{
	test_empty_hash_table();
	test_hash_table_single_article();
	test_hash_table_multiple_articles();

	printf("✔ All Tests Passed!");

	return 0;
}

