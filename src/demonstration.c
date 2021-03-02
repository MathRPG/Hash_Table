#include <stdlib.h>

#include "article.h"
#include "hashtable.h"

int main(void)
{
	HashTable_t* ht = ht_new();

	// Show initial state and capacity
	ht_display_states(ht, stdout);

	const char* const key_one = "DOI_one";
	Article_t* article_one = make_article(key_one, "Some_Title", "Some Author", 2000);

	const char* const key_two = "DOI_two";
	Article_t* article_two = make_article(key_two, "Another_Title", "Some other author", 2013);
	Article_t* article_two_v2 = make_article(key_two, "Another_Title", "Some other author et al.", 2015);

	const char* const key_three = "Yet Another DOI";
	Article_t* article_three = make_article(key_three, "Last Title", "Last Author", 2011);

	ht_insert(ht, article_one);
	ht_insert(ht, article_two);
	ht_insert(ht, article_three);
	ht_insert(ht, article_two_v2); // Inserting same key again only updates value already in table

	delete_article(article_one);
	delete_article(article_two);
	delete_article(article_two_v2);
	delete_article(article_three);

	// Should have 3 articles
	ht_display_states(ht, stdout);

	// Searches
	printf("Articles in ht:\n");
	display_article(ht_fetch(ht, key_one), stdout);
	display_article(ht_fetch(ht, key_two), stdout);
	display_article(ht_fetch(ht, key_three), stdout);

	// Non-existent keys can be queried through ht_contains
	const char* const non_existent_key = "HELLO I AM A DOI THAT DOES NOT EXIST";
	printf("%s in ht: %s\n", key_two, ht_contains(ht, key_two) ? "True" : "False");
	printf("%s in ht: %s\n", non_existent_key, ht_contains(ht, non_existent_key) ? "True" : "False");

	// ht_fetch returns NULL for non-existent keys
	printf("Make sure this is NULL: %p\n", ht_fetch(ht, non_existent_key));

	// Removing
	ht_remove(ht, key_one);
	ht_remove(ht, key_three);

	// Removed keys are not in table anymore
	ht_display_states(ht, stdout);
	printf("%s in ht: %s\n", key_one, ht_contains(ht, key_one) ? "True" : "False");
	printf("%s in ht: %s\n", key_three, ht_contains(ht, key_three) ? "True" : "False");

	// Removing non-valid key does nothing to the table
	ht_remove(ht, non_existent_key);
	ht_display_states(ht, stdout);

	// Table can be expanded
	ht_expand(ht);
	ht_display_states(ht, stdout);

	// ... and shrunk
	ht_shrink(ht);
	ht_display_states(ht, stdout);

	// These last two operations don't work for file-loaded tables, sorry

	// The table resizes manually to fit more keys (expand) or save memory (shrink) when if seems fit
	// But you can specify the size manually if you're feeling fancy!
	ht_resize(ht, 17); // After doing this, expand and shrink don't work anymore
	ht_display_states(ht, stdout);

	// Table can be dumped to a file
	FILE* fp = fopen("demonstration_ht.txt", "w");
	ht_dump(ht, fp);

	// You can still manipulate ht here...

	ht_delete(ht);

	// And read back from it
	freopen("demonstration_ht.txt", "r", fp);
	ht = ht_from_file(fp);

	// Dumping / reading and/or resizing 'cleans' the 'REMOVED' cells of the table
	ht_display_states(ht, stdout);

	ht_delete(ht);

	return EXIT_SUCCESS;
}