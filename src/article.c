#include "article.h"
#include "hashtable.h"
#include <string.h>

struct Article_s
{
	char DOI[32];
	char title[32];
	char author[32];
	unsigned year;
};

Article_t* make_article(const char* doi, const char* title, const char* author, unsigned int year)
{
	Article_t* a = (Article_t*)malloc(sizeof(Article_t));

	strncpy(a->DOI, doi, strlen(doi) + 1);
	strncpy(a->title, title, strlen(title) + 1);
	strncpy(a->author, author, strlen(author) + 1);
	a->year = year;

	return a;
}

void delete_article(Article_t* a)
{
	free(a);
}
