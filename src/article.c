#include <stdbool.h>
#include "article.h"

#include <string.h>
#include <stdlib.h>

#define MAX_STR_FIELD_LEN 32

struct Article_s
{
	char doi[MAX_STR_FIELD_LEN];
	char title[MAX_STR_FIELD_LEN];
	char author[MAX_STR_FIELD_LEN];
	unsigned year;
};

Article_t* make_article(const char* doi, const char* title, const char* author, unsigned int year)
{
	Article_t* a = (Article_t*)malloc(sizeof(Article_t));

	strncpy(a->doi, doi, strlen(doi) + 1);
	strncpy(a->title, title, strlen(title) + 1);
	strncpy(a->author, author, strlen(author) + 1);
	a->year = year;

	return a;
}

void delete_article(Article_t* a)
{
	free(a);
}

bool strings_equal_up_to(const char* str_a, const char* str_b, const unsigned len)
{
	return strncmp(str_a, str_b, len) == 0;
}

bool articles_are_equal(const Article_t* const a, const Article_t* const b)
{
	if (!strings_equal_up_to(a->doi, b->doi, MAX_STR_FIELD_LEN))
		return false;
	if (!strings_equal_up_to(a->title, b->title, MAX_STR_FIELD_LEN))
		return false;
	if (!strings_equal_up_to(a->author, b->author, MAX_STR_FIELD_LEN))
		return false;
	if (a->year != b->year)
		return false;
	return true;
}

bool article_has_key(const Article_t* article, const char* key)
{
	return strings_equal_up_to(article->doi, key, MAX_STR_FIELD_LEN);
}

Article_t* duplicate_article(const Article_t* original)
{
	Article_t* const copy = (Article_t*)malloc(sizeof(Article_t));

	strncpy(copy->doi, original->doi, MAX_STR_FIELD_LEN);
	strncpy(copy->title, original->title, MAX_STR_FIELD_LEN);
	strncpy(copy->author, original->author, MAX_STR_FIELD_LEN);
	copy->year = original->year;

	return copy;
}
