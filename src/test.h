#ifndef TEST_
#define TEST_

#include <stdbool.h>

typedef bool (*fctTest)(void);

#define VERT "32"
#define ROUGE "31"
#define DEFAULT "0"
#define couleur_texte(couleur) printf("\033[%sm",couleur)


void printOkKo( bool cond );


void resultat_test_fonction( char * nomTest, fctTest f );

#endif
