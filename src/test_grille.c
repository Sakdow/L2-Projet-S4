#include "sommet.h"
#include "graphe.h"
#include "grille.h"
#include "test.h"

#include <stdio.h>
#include <stdbool.h>
#include <time.h>


bool test_grille_creer(void){
	Grille gr1, gr2, gr3, gr4;
	bool retour;
	bool testC = true;

	int tailleAlea = rand()%(TAILLE_MAX - TAILLE_MIN) + TAILLE_MIN;
	int nbCases;

	grille_creer( &gr1, DEFAUT );
	grille_creer( &gr2, tailleAlea );
	grille_creer( &gr3, TAILLE_MAX + 1 );
	grille_creer( &gr4, TAILLE_MIN - 1 );

	printf("\n\tTest de la taille lors de la creation : ");
 	
	retour = grille_obtenirTaille( gr1 ) == TAILLE_QUAND_DEFAUT
		     && grille_obtenirTaille( gr2 ) == tailleAlea
		     && grille_obtenirTaille( gr3 ) == TAILLE_QUAND_DEFAUT
		     && grille_obtenirTaille( gr4 ) == TAILLE_QUAND_DEFAUT;

	printOkKo( retour );

	grille_supprimer( &gr1 );
	grille_supprimer( &gr2 );
	grille_supprimer( &gr3 );
	grille_supprimer( &gr4 );

	printf("\tTest de la couleur des cases : ");

	grille_creer( &gr1, rand()%TAILLE_MAX + TAILLE_MIN );

	nbCases = grille_obtenirTaille( gr1 ) * grille_obtenirTaille( gr1 );
	for( unsigned i = 0 ; i < nbCases ; i++ ){
		if( sommet_getCouleur( case_getSommet( grille_caseX( gr1, (int)i ) ) ) != t )
			testC = false;
	}
	if( sommet_getCouleur( case_getSommet( grille_caseX( gr1, BORD_HAUT ) ) ) != sommet_getCouleur( case_getSommet( grille_caseX( gr1, BORD_BAS ) ) )
	|| sommet_getCouleur( case_getSommet( grille_caseX( gr1, BORD_HAUT ) ) ) != w ){
		testC = false;
	}
	if( sommet_getCouleur( case_getSommet( grille_caseX( gr1, BORD_GAUCHE ) ) ) != sommet_getCouleur( case_getSommet( grille_caseX( gr1, BORD_DROIT ) ) )
	|| sommet_getCouleur( case_getSommet( grille_caseX( gr1, BORD_GAUCHE ) ) ) != b )
		testC = false;

	printOkKo( testC );

	printf("\tResultat : ");

	grille_supprimer( &gr1 );

	return retour && testC;

}

bool test_grille_modifierCouleur( void ){
	Grille grille;
	bool retour = true;
	int tailleG = rand()%(TAILLE_MAX - TAILLE_MIN) + TAILLE_MIN;
	const unsigned nbModifications = tailleG - TAILLE_MIN - 1;
	s_couleur couleurChangement;
	int caseChangement;

	grille_creer( &grille, tailleG );

	for( unsigned i = 0 ; i < nbModifications ; i++ ){
		caseChangement = rand()%( tailleG * tailleG + 4 ) - 4;
		switch( rand()%3 ){
			case 0:
				couleurChangement = t;
				break;
			case 1:
				couleurChangement = w;
				break;
			default:
				couleurChangement = b;
				break;
		}

		grille_modifierCouleur( &grille, caseChangement, couleurChangement );

		if( sommet_getCouleur( case_getSommet( grille_caseX( grille, caseChangement ) ) ) != couleurChangement )
			retour = false;
	}

	grille_supprimer( &grille );

	return retour;

}

bool test_grille_copie( void ){
	Grille grille, grilleCopie;
	bool retour = true;
	int tailleG = rand()%(TAILLE_MAX - TAILLE_MIN) + TAILLE_MIN;
	const unsigned nbModifications = tailleG - TAILLE_MIN - 1;
	s_couleur couleurChangement;
	int caseChangement;

	grille_creer( &grille, tailleG );

	for( unsigned i = 0 ; i < nbModifications ; i++ ){
		caseChangement = rand()%( tailleG * tailleG + 4 ) - 4;
		switch( rand()%3 ){
			case 0:
				couleurChangement = t;
				break;
			case 1:
				couleurChangement = w;
				break;
			default:
				couleurChangement = b;
				break;
		}

		grille_modifierCouleur( &grille, caseChangement, couleurChangement );
	}

	grille_copie( &grilleCopie, grille );

	retour = ( grille_comparer( grille, grilleCopie ) == 0 );

	grille_supprimer( &grille );
	grille_supprimer( &grilleCopie );

	return retour;
}



int main(int argc, char const *argv[])
{
	srand(time(NULL));
	printf("TEST MODULE GRILLE :\n\n");
	resultat_test_fonction("test_grille_creer",test_grille_creer);
	resultat_test_fonction("test_grille_modifierCouleur",test_grille_modifierCouleur);
	resultat_test_fonction("test_grille_copie",test_grille_copie);

	return 0;
}
