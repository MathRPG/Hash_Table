#include "article.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct Article_s
{
	char doi[32];
	char title[32];
	char author[32];
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
	if (!strings_equal_up_to(a->doi, b->doi, 32))
		return false;
	if (!strings_equal_up_to(a->title, b->title, 32))
		return false;
	if (!strings_equal_up_to(a->author, b->author, 32))
		return false;
	if (a->year != b->year)
		return false;
	return true;
}

void copy_article(Article_t* dest, Article_t* origin)
{
	strncpy(dest->doi, origin->doi, 32);
	strncpy(dest->title, origin->title, 32);
	strncpy(dest->author, origin->author, 32);
	dest->year = origin->year;
}
