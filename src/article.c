#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "article.h"

#define MAX_STR_FIELD_LEN 32lu

struct Article_s
{
	char doi[MAX_STR_FIELD_LEN];
	char title[MAX_STR_FIELD_LEN];
	char author[MAX_STR_FIELD_LEN];
	unsigned year;
};

unsigned long capped_str_len(const char* const str)
{
	const unsigned long len = strlen(str) + 1;
	return len < MAX_STR_FIELD_LEN ? len : MAX_STR_FIELD_LEN;
}

void copy_values_to_structure(
		Article_t* const a, const char* const doi, const char* const title,
		const char* const author, const unsigned int year)
{
	strncpy(a->doi, doi, capped_str_len(doi));
	strncpy(a->title, title, capped_str_len(title));
	strncpy(a->author, author, capped_str_len(author));
	a->year = year;
}

Article_t* make_article(
		const char* const doi, const char* const title,
		const char* const author, const unsigned int year)
{
	Article_t* const a = (Article_t*)malloc(sizeof(Article_t));
	copy_values_to_structure(a, doi, title, author, year);
	return a;
}

Article_t* duplicate_article(const Article_t* const original)
{
	Article_t* const copy = (Article_t*)malloc(sizeof(Article_t));
	copy_values_to_structure(copy, original->doi, original->title, original->author, original->year);
	return copy;
}

void delete_article(Article_t* const a)
{
	free(a);
}

const char* key_of(const Article_t* const article)
{
	return article->doi;
}

bool strings_equal_up_to(const char* const str_a, const char* const str_b, const unsigned len)
{
	return strncmp(str_a, str_b, len) == 0;
}

bool article_has_key(const Article_t* const article, const char* const key)
{
	return strings_equal_up_to(article->doi, key, MAX_STR_FIELD_LEN);
}

bool articles_are_equal(const Article_t* const a, const Article_t* const b)
{
	if (!article_has_key(a, key_of(b)))
		return false;
	if (!strings_equal_up_to(a->title, b->title, MAX_STR_FIELD_LEN))
		return false;
	if (!strings_equal_up_to(a->author, b->author, MAX_STR_FIELD_LEN))
		return false;
	if (a->year != b->year)
		return false;
	return true;
}
