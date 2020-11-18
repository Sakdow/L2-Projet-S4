#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "pile.h"
#include "case.h"
#include "sommet.h"
#include "test.h"

#define NB_TESTS 100

Case creation_case(){
	s_couleur couleur;
	Sommet s;
	Case c;

	int random=rand();
	switch (random%3){
		case 0:
		couleur=t;
		break;
		
		case 1:
		couleur=b;
		break;
		
		default:
		couleur=w;
		break;
	}
	sommet_creer(&s,couleur);
	case_creer(&c,random%100,s);
	sommet_detruire(&s);

	return c;
}

bool test_pile_initialiser(void){
	Pile p;

	pile_initialiser(&p);
	bool test=(pile_vide(p)) ? true :false;
	pile_detruire(&p);

	return test;
}

bool test_pile_empiler(void){
	Pile p;
	Case c,sommet;


	pile_initialiser(&p);

	for(int i=0; i<NB_TESTS; i++){
		c=creation_case();
		pile_empiler(&p,c);
		sommet=pile_sommet(p);
		if(!case_comparer(sommet,c)){
			case_detruire(&c);
			case_detruire(&sommet);
			pile_detruire(&p);
			return false;
		}

		case_detruire(&c);
		case_detruire(&sommet);
	}

	pile_detruire(&p);

	return true;
}

bool test_pile_depiler(void){
	Pile p;
	Case c[NB_TESTS];
	Case sommet;


	pile_initialiser(&p);

	for(int i=0; i<NB_TESTS; i++){
		c[i]=creation_case();
		pile_empiler(&p,c[i]);
	}

	for(int i=NB_TESTS-1; !pile_vide(p); i--){
		sommet=pile_sommet(p);
		if(!case_comparer(sommet,c[i])){
			case_detruire(&c[i]);
			case_detruire(&sommet);
			pile_detruire(&p);
			return false;
		}
		pile_depiler(&p);
		case_detruire(&c[i]);
		case_detruire(&sommet);
	}

	pile_detruire(&p);

	return true;
}

bool test_pile_copie(void){
	Pile p,pCopie;
	Case c;
	
	for(int i=0; i<NB_TESTS;i++){
		pile_initialiser(&p);
		for (int j = 0; j < NB_TESTS; ++j){
			c=creation_case();
			pile_empiler(&p,c);
			case_detruire(&c);
		}
		pCopie=pile_copie(p);
		if(!pile_comparer(p,pCopie)){
			pile_detruire(&p);
			pile_detruire(&pCopie);
			return false;
		}
		pile_detruire(&pCopie);
		pile_detruire(&p);
	}

	return true;
}

int main(){
	srand(time(NULL));

	printf("TEST MODULE PILE :\n\n");
	resultat_test_fonction("pile_initialiser",test_pile_initialiser);
	resultat_test_fonction("pile_empiler",test_pile_empiler);
	resultat_test_fonction("pile_depiler",test_pile_depiler);
	resultat_test_fonction("pile_copie",test_pile_copie); 	
	
	return 0;
}
