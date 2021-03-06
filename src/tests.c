#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#include "article.h"
#include "hashtable.h"

bool global_failure;

void debug(const char* message)
{
#ifndef NDEBUG
	puts(message);
#endif
}

void test_empty_hash_table()
{
	/*
	 * Tests properties of an empty hash table
	 * Initial values - not null, is_empty, count
	 * Operations - contains, fetch, remove
	 */

	HashTable_t* const ht = ht_new();
	const char* some_doi = "Some_DOI";

	// Initial values
	assert(ht != NULL);
	assert(ht_is_empty(ht) == true);
	assert(ht_count(ht) == 0);
	debug("Empty table: initial values");

	// All searches must yield 'failure'
	assert(ht_contains(ht, some_doi) == false);
	assert(ht_fetch(ht, some_doi) == NULL);
	debug("Empty table: queries fail");

	// Remove must maintain table intact
	ht_remove(ht, some_doi);
	assert(ht_is_empty(ht) == true);
	assert(ht_count(ht) == 0);
	assert(ht_contains(ht, some_doi) == false);
	assert(ht_fetch(ht, some_doi) == NULL);
	debug("Empty table: remove does nothing");

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

	HashTable_t* const ht = ht_new();
	const char* const inserted_key = "Some_DOI";
	const char* const not_inserted_key = "Other_DOI";

	Article_t* a = make_article(inserted_key, "Title", "Author", 2000u);
	ht_insert(ht, a);

	// Should not be empty after one insertion
	assert(ht_is_empty(ht) == false);
	assert(ht_count(ht) == 1);
	debug("One value in table: properties are correct");

	// Queries must yield success for inserted key
	assert(ht_contains(ht, inserted_key) == true);
	const Article_t* fetched = ht_fetch(ht, inserted_key);
	assert(fetched != NULL);
	assert(articles_are_equal(a, fetched) == true);
	debug("One value in table: query success");

	// Queries must yield failure for not-inserted key
	assert(ht_contains(ht, not_inserted_key) == false);
	assert(ht_fetch(ht, not_inserted_key) == NULL);
	debug("One value in table: query failure");

	// Remove of not-inserted key must maintain table intact
	ht_remove(ht, not_inserted_key);
	assert(ht_is_empty(ht) == false);
	assert(ht_count(ht) == 1);
	assert(ht_contains(ht, inserted_key) == true);
	fetched = ht_fetch(ht, inserted_key);
	assert(fetched != NULL);
	assert(articles_are_equal(a, fetched) == true);
	debug("One value table: useless remove keeps table intact");

	// Remove of inserted key must regress table back to being empty
	ht_remove(ht, inserted_key);
	assert(ht_is_empty(ht) == true);
	assert(ht_count(ht) == 0);
	assert(ht_contains(ht, inserted_key) == false);
	assert(ht_fetch(ht, inserted_key) == NULL);
	debug("One value table: remove empties table");

	// Cleanup
	delete_article(a);
	ht_delete(ht);
}

void test_hash_table_multiple_articles()
{
	HashTable_t* ht = ht_new();
	const char* key_one = "DOI_one";
	const char* key_two = "DOI_two";
	Article_t* article_one = make_article(key_one, "", "", 0);
	Article_t* article_two = make_article(key_two, "", "", 0);

	ht_insert(ht, article_one);
	ht_insert(ht, article_two);

	assert(ht_count(ht) == 2);
	debug("Two value table: properties are correct");

	// Remove key_one keeps key_two in table
	ht_remove(ht, key_one);
	assert(ht_is_empty(ht) == false);
	assert(ht_count(ht) == 1);
	assert(ht_contains(ht, key_two) == true);
	const Article_t* fetched = ht_fetch(ht, key_two);
	assert(fetched != NULL);
	assert(articles_are_equal(article_two, fetched));
	debug("Two value table: Removing key_one keeps key_two");

	ht_insert(ht, article_one);

	// Remove key_two keeps key_one in table
	ht_remove(ht, key_two);
	assert(ht_is_empty(ht) == false);
	assert(ht_count(ht) == 1);
	assert(ht_contains(ht, key_one) == true);
	fetched = ht_fetch(ht, key_one);
	assert(fetched != NULL);
	assert(articles_are_equal(article_one, fetched));
	debug("Two value table: Removing key_two keeps key_one");

	delete_article(article_one);
	delete_article(article_two);
	ht_delete(ht);
}

void test_hash_table_insert_override_key()
{
	HashTable_t* ht = ht_new();
	const char* repeat_key = "Repeat_DOI";
	const char* other_key = "Other_DOI";
	Article_t* first_version = make_article(repeat_key, "First_Title", "", 0);
	Article_t* second_version = make_article(repeat_key, "Second_Title", "", 0);
	Article_t* other_article = make_article(other_key, "", "", 0);

	// Inserting two articles with same key replaces the first version
	ht_insert(ht, first_version);
	ht_insert(ht, second_version);
	assert(ht_count(ht) == 1);
	const Article_t* fetched = ht_fetch(ht, repeat_key);
	assert(articles_are_equal(second_version, fetched));
	debug("Insertion override: inserting repeat key overrides old value");

	ht_delete(ht);

	// Inserting two articles, removing the first and replacing the second replaces correctly
	ht = ht_new();

	ht_insert(ht, other_article);
	ht_insert(ht, first_version);
	ht_remove(ht, other_key);
	ht_insert(ht, second_version);

	assert(ht_count(ht) == 1);
	debug("Insertion Ov.: ins. two, removing first, replacing second does the thing");

	delete_article(first_version);
	delete_article(second_version);
	delete_article(other_article);
	ht_delete(ht);
}

void test_hash_table_resize()
{
	HashTable_t* ht = ht_new();

	const unsigned long original_capacity = ht_capacity(ht);
	assert(original_capacity > 0);
	debug("Empty table fits at least 1 item");

	ht_shrink(ht);
	assert(ht_capacity(ht) == original_capacity);
	debug("Cannot shrink empty table");

	ht_expand(ht);
	debug("Hello");
	const unsigned long expanded_capacity = ht_capacity(ht);
	assert(expanded_capacity > original_capacity);
	debug("Expansion increases size");

	ht_shrink(ht);
	debug("Hello again!");
	assert(ht_capacity(ht) < expanded_capacity);
	debug("Shrinkage decreases size");

	ht_expand(ht);
	ht_expand(ht);
	const unsigned long super_expanded_capacity = ht_capacity(ht);
	assert(super_expanded_capacity > expanded_capacity);
	debug("Super EXPANSION increases size even more");

	ht_delete(ht);
}

void test_hash_table_file_operations_empty_table()
{
	HashTable_t* ht = ht_new();

	// Dumping empty hash table
	FILE* fp = fopen("hash.bin", "w");
	ht_dump(ht, fp);

	ht_delete(ht);

	// Reading ht from same file
	freopen("hash.bin", "r", fp);
	ht = ht_from_file(fp);

	// Make sure it's an empty hash table
	assert(ht_is_empty(ht) == true);
	assert(ht_count(ht) == 0);
	debug("Can dump empty table");

	ht_delete(ht);
	fclose(fp);
}

void test_hash_table_file_operations_resized_table()
{// Can dump/read resized ht
	HashTable_t* ht = ht_new();
	unsigned long resized_capacity = ht_capacity(ht) + 1;
	FILE* fp = fopen("hash.bin", "w");

	ht_resize(ht, resized_capacity);
	ht_dump(ht, fp);

	freopen("hash.bin", "r", fp);
	ht_delete(ht);
	ht = ht_from_file(fp);

	assert(ht_capacity(ht) == resized_capacity);
	debug("Can dump resized table");

	ht_delete(ht);
	fclose(fp);
}

void test_hash_table_file_operations_one_article()
{// Test hashtable with one item
	HashTable_t* ht = ht_new();
	const char* const a_key = "DOI";
	Article_t* const a = make_article(a_key, "Title", "Author", 2000);
	FILE* fp = fopen("hash.bin", "w");

	ht_insert(ht, a);
	ht_dump(ht, fp);

	freopen("hash.bin", "r", fp);
	ht_delete(ht);
	ht = ht_from_file(fp);

	assert(ht_is_empty(ht) == false);
	assert(ht_count(ht) == 1);
	assert(ht_contains(ht, a_key) == true);
	const Article_t* const fetched = ht_fetch(ht, a_key);
	assert(fetched != NULL);
	assert(articles_are_equal(a, fetched));
	debug("Can dump ht with one article");

	delete_article(a);
	ht_delete(ht);
}

void test_hash_table_file_operations_two_articles()
{
	HashTable_t* ht = ht_new();
	const char* const a_key = "DOI_without_spaces";
	const char* const b_key = "DOI with spaces";
	Article_t* const a = make_article(a_key, "Title with spaces", "Author", 2000);
	Article_t* const b = make_article(b_key, "Title_without_spaces", "Author with spaces", 2000);
	FILE* fp = fopen("hash.bin", "w");

	ht_insert(ht, a);
	ht_insert(ht, b);
	ht_dump(ht, fp);
	ht_delete(ht);

	freopen("hash.bin", "r", fp);
	ht = ht_from_file(fp);

	assert(ht_is_empty(ht) == false);
	assert(ht_count(ht) == 2);

	assert(ht_contains(ht, a_key) == true);
	const Article_t* fetched = ht_fetch(ht, a_key);
	assert(fetched != NULL);
	assert(articles_are_equal(a, fetched));

	assert(ht_contains(ht, b_key) == true);
	fetched = ht_fetch(ht, b_key);
	assert(fetched != NULL);
	assert(articles_are_equal(b, fetched));

	debug("Can dump ht with two articles");

	delete_article(a);
	delete_article(b);
	ht_delete(ht);
}

void test_hash_table_file_operations()
{
	test_hash_table_file_operations_empty_table();
	test_hash_table_file_operations_resized_table();
	test_hash_table_file_operations_one_article();
	test_hash_table_file_operations_two_articles();
}

void print_test_status()
{
	if (global_failure)
	{
		printf("\033[1;31m");
		printf("❌ Tests Failed!");
	}
	else
	{
		printf("\033[1;32m");
		printf("✔ All Tests Passed!");
	}
	printf("\033[0m");
}

int main(void)
{
	global_failure = true;
	atexit(print_test_status);

	test_empty_hash_table();
	test_hash_table_single_article();
	test_hash_table_multiple_articles();
	test_hash_table_insert_override_key();
	test_hash_table_resize();
	test_hash_table_file_operations();

	global_failure = false;

	return 0;
}
