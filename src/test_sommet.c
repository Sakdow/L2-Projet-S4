#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sommet.h"
#include "test.h"


bool test_creation_sommet(void){
	bool retour=false;
	Sommet sommet;
	s_couleur couleur=w;
	sommet_creer(&sommet,couleur);
	if(sommet_getCouleur(sommet)==couleur)
		retour=true;
	sommet_detruire( &sommet ); 	
	return retour;
}

bool test_copie_sommet(void){
	bool retour=false;
	Sommet sommet,sommet2;
	s_couleur couleur=w;
	sommet_creer(&sommet,couleur);
	sommet_copie(&sommet,sommet2);
	if(sommet_comparerCouleur(sommet,sommet2))
		retour=true;
	sommet_detruire( &sommet ); 
	sommet_detruire( &sommet2 ); 	
	return retour;
}


int main(int argc, char const *argv[])
{
	printf("TEST MODULE SOMMET :\n\n");
	resultat_test_fonction("test_creation_sommet",test_creation_sommet);
	resultat_test_fonction("test_copie_sommet",test_copie_sommet);

	return 0;
}
