#ifndef ARTICLE_H
#define ARTICLE_H

#include <stdbool.h>

struct Article_s;
typedef struct Article_s Article_t;

Article_t* make_article(const char* doi, const char* title, const char* author, unsigned int year);
void delete_article(Article_t* a);
bool articles_are_equal(const Article_t* a, const Article_t* b);
bool article_has_key(const Article_t* article, const char* key);
Article_t* duplicate_article(const Article_t* original);

#endif //ARTICLE_H
