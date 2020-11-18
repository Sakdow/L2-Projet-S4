#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "jeu.h"
#include "pile.h"
#include "joueur.h"
#include "groupe.h"
#include "case.h"
#include "sommet.h"
#include "test.h"
#include "grille.h"

#define NB_TESTS 100

bool test_jeu_creer(void){
	InfosJeu i;
	int taille;
	bool premierJoueurEstIA,secondJoueurEstIA;
	Grille grilleTemoin;


	int random=rand();
	int random2=rand();

	taille=(random%19)+1;
	grille_creer(&grilleTemoin,taille);

	premierJoueurEstIA=(random%2==0) ? true : false;
	secondJoueurEstIA=(random2%2==0) ? true : false;

	jeu_creer(&i,taille,premierJoueurEstIA,secondJoueurEstIA);

	if(grille_comparer(jeu_getGrille(i),grilleTemoin) 
	|| premierJoueurEstIA!=joueur_estIA(jeu_getJoueur1(i))
	|| secondJoueurEstIA!=joueur_estIA(jeu_getJoueur2(i)))
		return false;

	grille_supprimer(&grilleTemoin);
	jeu_detruire(&i);
	return true;
}

bool test_jeu_copie(void){
	InfosJeu i,iCopie;
	int taille;
	bool premierJoueurEstIA,secondJoueurEstIA;
	Grille grilleTemoin;
	for(int j=0; j<NB_TESTS; j++){
		int random=rand();
		int random2=rand();

		taille=(random%19)+1;
		grille_creer(&grilleTemoin,taille);

		premierJoueurEstIA=(random%2==0) ? true : false;
		secondJoueurEstIA=(random2%2==0) ? true : false;

		jeu_creer(&i,taille,premierJoueurEstIA,secondJoueurEstIA);
		iCopie=jeu_copie(i);
		
		if(!jeu_comparer(iCopie,i)){
			grille_supprimer(&grilleTemoin);
			jeu_detruire(&iCopie);
			jeu_detruire(&i);
			return false;
		}
		grille_supprimer(&grilleTemoin);
		jeu_detruire(&iCopie);
		jeu_detruire(&i);
	}
	return true;
}

int main(){
	srand(time(NULL));

	printf("TEST MODULE JEU :\n\n");
	resultat_test_fonction("jeu_creer",test_jeu_creer);
	resultat_test_fonction("jeu_copie",test_jeu_copie);

	return 0;
}
