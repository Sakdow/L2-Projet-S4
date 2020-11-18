#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "sommet.h"
#include "test.h"
#include "case.h"


bool test_creation_case(void){
	Case c;
	bool retour=false;
	int numero=2;
	s_couleur couleur=w;
	Sommet sommet;
	sommet_creer(&sommet,couleur);
	case_creer( &c,numero, sommet );
	if(case_getNumeroCase(c)==numero && sommet_getCouleur(case_getSommet(c))==couleur){
		retour=true;
	}
	sommet_detruire(&sommet);
	case_detruire( &c ); 	
	return retour;
}

bool test_copie_case(void){
	Case caseSource,caseCopie;
	bool retour=false;
	int numero=2;
	s_couleur couleur=w;
	Sommet sommet;
	sommet_creer(&sommet,couleur);
	case_creer( &caseSource,numero, sommet );
	case_copie(&caseCopie,caseSource);
	if(case_comparer(caseCopie,caseSource)){
		retour=true;
	}
	sommet_detruire(&sommet);
	case_detruire( &caseSource );
	case_detruire( &caseCopie );
	return retour;
}


int main(int argc, char const *argv[])
{
	printf("TEST MODULE CASE :\n\n");
	resultat_test_fonction("test_creation_case",test_creation_case);
	resultat_test_fonction("test_copie_case",test_copie_case);
	return 0;
}
