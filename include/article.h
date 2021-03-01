#ifndef ARTICLE_H
#define ARTICLE_H

struct Article
{
	const char DOI[32];
	const char title[32];
	const char author[32];
	const unsigned year;
};

typedef struct Article Article;

#endif //ARTICLE_H
