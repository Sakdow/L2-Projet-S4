#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "groupe.h"
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

bool test_groupe_initialiser(void){
	Groupe g[NB_TESTS];
	for(int i=0; i<NB_TESTS; i++){
		groupe_initialiser(&g[i]);
		if(!groupe_vide(g[i])){
			groupe_detruire(&g[i]);
			return false;
		}
	}
	for(int i=0; i<NB_TESTS; i++)
		groupe_detruire(&g[i]);
	return true;	
}

bool test_groupe_ajouter(void){
	Groupe g;
	Case c;
	bool appartient;
	int taille;
	groupe_initialiser(&g);

	for(int i=0; i<NB_TESTS; i++){
		c=creation_case();
		appartient=(groupe_appartenir(g,c)? true : false);
		taille=groupe_getTaille(g);

		groupe_ajouter(&g,c);
		if(!groupe_appartenir(g,c)){
			case_detruire(&c);
			groupe_detruire(&g);
			return false;
		}
		if((appartient && groupe_getTaille(g)!=taille) 
		||(!appartient && groupe_getTaille(g)!=taille+1)){
			case_detruire(&c);
			groupe_detruire(&g);
			return false;
		}
		case_detruire(&c);
	}
	groupe_detruire(&g);
	return true;	
}

bool test_groupe_supprimer(void){
	Groupe g;
	Case c;
	int taille;
	groupe_initialiser(&g);

	for(int i=0; i<NB_TESTS; i++){
		c=creation_case();
		
		groupe_ajouter(&g,c);
		taille=groupe_getTaille(g);
		
		groupe_supprimer(&g,c);
		
		if(groupe_appartenir(g,c) || groupe_getTaille(g)!=taille-1){
			case_detruire(&c);
			groupe_detruire(&g);
			return false;
		}
		case_detruire(&c);
	}
	groupe_detruire(&g);
	return true;	
}

bool test_groupe_fusion(void){
	Groupe g[3];
	Case tab_g[2][NB_TESTS];
	Case c;

	for(int i=0; i<2; i++){
		groupe_initialiser(&g[i]);
		for(int j=0; j<NB_TESTS; j++){
			c=creation_case();
			groupe_ajouter(&g[i],c);
			tab_g[i][j]=c;
		}
	}

	groupe_initialiser(&g[2]);
	for(int i=0; i<2; i++){
		for(int j=0; j<NB_TESTS; j++){
			groupe_ajouter(&g[2],tab_g[i][j]);
		}
	}

	groupe_fusion(&g[0],g[1]);

	if(!groupe_comparer(g[0],g[2]))
		return false;

	for(int i=0; i<2; i++){
		for(int j=0; j<NB_TESTS; j++){
			case_detruire(&tab_g[i][j]);
		}
	}

	for(int i=0; i<3; i++)
		groupe_detruire(&g[i]);

	
	return true;
}



int main(){
	srand(time(NULL));

	printf("TEST MODULE GROUPE :\n\n");
	resultat_test_fonction("groupe_initialiser",test_groupe_initialiser);
	resultat_test_fonction("groupe_ajouter",test_groupe_ajouter);
	resultat_test_fonction("groupe_supprimer",test_groupe_supprimer);
	resultat_test_fonction("groupe_fusion",test_groupe_fusion);

	return 0;
}
