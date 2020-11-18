#include <stdio.h>
#include <stdbool.h>
#include "test.h"


void printOkKo( bool cond ){
	if( cond ){
		couleur_texte(VERT);
		printf("OK\n");
	} else {
		couleur_texte(ROUGE);
		printf("KO\n");
	}
	couleur_texte(DEFAULT);
}

void resultat_test_fonction( char * nomTest, fctTest f ){
	printf("%s: ",nomTest);
	printOkKo( f() );
}
