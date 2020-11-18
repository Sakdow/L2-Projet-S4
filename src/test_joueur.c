#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "joueur.h"
#include "groupe.h"
#include "case.h"
#include "sommet.h"
#include "test.h"
#include "grille.h"

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

Groupe creation_groupe(){
	Groupe g;
	Case c;
	groupe_initialiser(&g);

	for (int i = 0; i < rand()%100; ++i){
		c=creation_case();
		groupe_ajouter(&g,c);
		case_detruire(&c);
	}

	return g;
}

bool test_joueur_creer(void){
	Joueur j;
	s_couleur couleur;
	bool estIA;

	for (int i = 0; i < NB_TESTS; ++i){
		int random=rand();
		switch (random%2){			
			case 1:
			couleur=b;
			break;
			
			default:
			couleur=w;
			break;
		}
		estIA=(random%2==0) ? true : false;

		joueur_creer(&j,couleur,estIA);

		if( joueur_getNbGroupes(j)!=0 || joueur_getCouleur(j)!=couleur || joueur_estIA(j)!=estIA)
			return false;
		joueur_detruire(&j);
	}
	return true;
}

bool test_joueur_ajouterGroupe(void){
	Joueur j;
	Groupe g;
	int nbGroupes;

	joueur_creer(&j,w,true);

	for(int i=0; i<NB_TESTS; i++){
		g=creation_groupe();
		nbGroupes=joueur_getNbGroupes(j);
		joueur_ajouterGroupe(&j,g);
		groupe_detruire(&g);

		if(joueur_getNbGroupes(j)!=nbGroupes+1){
			joueur_detruire(&j);
			return false;
		}
	}
	joueur_detruire(&j);
	return true;
}

bool test_joueur_supprimerGroupe(void){
	Joueur j;
	Groupe g;
	int nbGroupes;

	joueur_creer(&j,w,true);

	for(int i=0; i<NB_TESTS; i++){
		g=creation_groupe();
		joueur_ajouterGroupe(&j,g);
		nbGroupes=joueur_getNbGroupes(j);
		joueur_supprimerGroupe(&j,g);
		groupe_detruire(&g);

		if(joueur_getNbGroupes(j)!=nbGroupes-1){
			joueur_detruire(&j);
			return false;
		}
	}
	joueur_detruire(&j);
	return true;
}

bool test_joueur_copie(void){
	Joueur j,jCopie;
	s_couleur couleur;
	bool estIA;

	for (int i = 0; i < NB_TESTS; ++i){
		int random=rand();
		switch (random%2){			
			case 1:
			couleur=b;
			break;
			
			default:
			couleur=w;
			break;
		}
		estIA=(random%2==0) ? true : false;

		joueur_creer(&j,couleur,estIA);
		jCopie=joueur_copie(j);

		if(!joueur_comparer(j,jCopie)){
			joueur_detruire(&j);
			joueur_detruire(&jCopie);
			return false;
		}
		joueur_detruire(&j);
		joueur_detruire(&jCopie);
	}
	return true;
}

bool test_joueur_recupererGroupesBords(void){
	Joueur temoinW,temoinB;
	Grille grille;
	Case bordHaut,bordBas,bordGauche,bordDroit;
	Groupe groupeBordHaut,groupeBordBas,groupeBordGauche,groupeBordDroit;

	joueur_creer(&temoinW,w,true);
	joueur_creer(&temoinB,b,false);

	grille_creer(&grille,DEFAUT);

	bordHaut=grille_caseX(grille,-1);
	bordBas=grille_caseX(grille,-3);
	bordGauche=grille_caseX(grille,-2);
	bordDroit=grille_caseX(grille,-4);

	groupe_initialiser(&groupeBordHaut);
	groupe_initialiser(&groupeBordBas);
	groupe_initialiser(&groupeBordGauche);
	groupe_initialiser(&groupeBordDroit);

	groupe_ajouter(&groupeBordHaut,bordHaut);
	groupe_ajouter(&groupeBordBas,bordBas);
	groupe_ajouter(&groupeBordGauche,bordGauche);
	groupe_ajouter(&groupeBordDroit,bordDroit);

	joueur_recupererGroupesBords(&temoinW,grille);
	joueur_recupererGroupesBords(&temoinB,grille);

	if(!joueur_appartenirGroupe(temoinW,groupeBordHaut) || !joueur_appartenirGroupe(temoinW,groupeBordBas)
	|| !joueur_appartenirGroupe(temoinB,groupeBordGauche) || !joueur_appartenirGroupe(temoinB,groupeBordDroit)){
		groupe_detruire(&groupeBordHaut);
		groupe_detruire(&groupeBordBas);
		groupe_detruire(&groupeBordGauche);
		groupe_detruire(&groupeBordDroit);
		grille_supprimer(&grille);
		joueur_detruire(&temoinW);
		joueur_detruire(&temoinB);
		return false;
	}
	groupe_detruire(&groupeBordHaut);
	groupe_detruire(&groupeBordBas);
	groupe_detruire(&groupeBordGauche);
	groupe_detruire(&groupeBordDroit);
	grille_supprimer(&grille);
	joueur_detruire(&temoinW);
	joueur_detruire(&temoinB);
	return true;
}

int main(){
	srand(time(NULL));

	printf("TEST MODULE JOUEUR :\n\n");
	resultat_test_fonction("joueur_creer",test_joueur_creer);
	resultat_test_fonction("joueur_ajouterGroupe",test_joueur_ajouterGroupe);
	resultat_test_fonction("joueur_supprimerGroupe",test_joueur_supprimerGroupe);
	resultat_test_fonction("joueur_copie",test_joueur_copie);
	resultat_test_fonction("joueur_recupererGroupesBords",test_joueur_recupererGroupesBords);

return 0;
}
