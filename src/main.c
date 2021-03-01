#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "article.h"
#include "hashtable.h"

int main(void)
{
	HashTable_t* const ht = ht_create();
	const char* const first_doi = "DOI";
	const char* const second_doi = "Other_DOI";

	assert(ht != NULL);
	assert(ht_is_empty(ht) == true);
	assert(ht_count(ht) == 0);
	assert(ht_contains(ht, first_doi) == false);

	Article_t* a = make_article(first_doi, "Title", "Author", 2000u);
	ht_insert(ht, a);

	assert(ht_is_empty(ht) == false);
	assert(ht_count(ht) == 1);
	assert(ht_contains(ht, first_doi) == true);

	const Article_t* fetched = ht_fetch(ht, first_doi);

	assert(fetched != NULL);
	assert(articles_are_equal(a, fetched) == true);

	const Article_t* new_fetched = ht_fetch(ht, second_doi);

	assert(new_fetched == NULL);

//	Article_t* b = make_article(second_doi, "Title", "Author", 2000u);
//	ht_insert(ht, b);
//
//	assert(ht_is_empty(ht) == false);
//	assert(ht_count(ht) == 2);
//	assert(ht_contains(ht, second_doi) == true);

	delete_article(a);
//	delete_article(b);
	ht_delete(ht);

	printf("✔ All Tests Passed!");

	return 0;
}

