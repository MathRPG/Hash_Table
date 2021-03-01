#ifndef ARTICLE_H
#define ARTICLE_H

struct Article_s;
typedef struct Article_s Article_t;

Article_t* make_article(const char* doi, const char* title, const char* author, unsigned int year);
void delete_article(Article_t* a);

#endif //ARTICLE_H
