#ifndef ARTICLE_H
#define ARTICLE_H

#include <stdbool.h>
#include <stdio.h>

typedef struct Article_s Article_t;

// Constructors/Destructors
Article_t* make_article(const char* doi, const char* title, const char* author, unsigned int year);
Article_t* duplicate_article(const Article_t* original);
Article_t* article_from_file(FILE* in);
void delete_article(Article_t* a);

// Queries
const char* key_of(const Article_t* article);
bool article_has_key(const Article_t* article, const char* key);
bool articles_are_equal(const Article_t* a, const Article_t* b);

// Commands
void display_article(const Article_t* article, FILE* out);
void dump_article(const Article_t* article, FILE* out);

#endif //ARTICLE_H
