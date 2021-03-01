#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "article.h"
#include "hashtable.h"

void test_empty_hash_table()
{
	/*
	 * Tests properties of an empty hash table
	 * Initial values - is_empty, count
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
	
	ht_delete(ht);
}

void test_hash_table_single_article()
{
	HashTable_t* const ht = ht_create();
	const char* const doi = "DOI";
	const char* const other_doi = "Other_DOI";

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

void test_hash_table_multiple_articles()
{

}

void test_hash_table_insert_override()
{

}

int main(void)
{
	test_empty_hash_table();
	test_hash_table_single_article();
	test_hash_table_insert_override();
	test_hash_table_multiple_articles();

	printf("✔ All Tests Passed!");

	return 0;
}

