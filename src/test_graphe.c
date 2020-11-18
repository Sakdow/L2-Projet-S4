#include "graphe.h"
#include "case.h"
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include "test.h"

void afficherMessage( char *nomFonct, bool test ){
	printf("%s:",nomFonct);
	if(test){
		printf("OK\n");
	}
	else{
		printf("KO\n");
	}
}

bool test_graphe_create( void ){
	Graphe g;
	bool retour;

	retour = ( graphe_create( &g ) != NULL );

	retour = retour && graphe_estVide( g );

	graphe_destroy( &g );

	return retour;
}

void ajouterTabDansGraphe( Graphe *g, int tab[], int tailleTab, int tabRetourFonctAjout[] ){
	Case c;
	Sommet s;
	sommet_creer( &s, t );
	case_creer( &c, 0 ,s );
	int retour;

	for( size_t i = 0 ; i < tailleTab ; i++ ){
		retour = graphe_ajouterCase( g , tab[i], c );
		if( tabRetourFonctAjout != NULL )
			tabRetourFonctAjout[i] = retour;
	}

	sommet_detruire( &s );
	case_detruire( &c );
}

unsigned nbOccurAvantIndexDansTab( int tab[], size_t index, int valeur ){
	unsigned nbOcc = 0;
	for( size_t i = 0 ; i < index ; i++ )
		if( tab[i] == valeur )
			nbOcc++;
	return nbOcc;
}

#define NOMBRE_AJOUT_TEST_AJOUTER 1000
bool test_graphe_ajouterCase( void ){
	Graphe g;
	bool test1 = true;
	bool retour = true;
	int indexCasesAAjouter[NOMBRE_AJOUT_TEST_AJOUTER];
	int retourFonctAjout[NOMBRE_AJOUT_TEST_AJOUTER];

	const int modulo = 200;/*pour un meilleur texte, modulo est inferieur à NOMBRE_AJOUT_TEST_AJOUTER*/

	for( size_t i = 0 ; i < NOMBRE_AJOUT_TEST_AJOUTER ; i++ )
		indexCasesAAjouter[i] = rand()%modulo * ( -1 * (rand()%2 +1) );

	graphe_create( &g );

	ajouterTabDansGraphe( &g, indexCasesAAjouter, NOMBRE_AJOUT_TEST_AJOUTER, retourFonctAjout );

	printf("\n\tTest du retour de la fonction : ");

	for( size_t i = 0 ; i < NOMBRE_AJOUT_TEST_AJOUTER ; i++ ){
		switch( retourFonctAjout[i] ){
			case( 1 ):
				if( nbOccurAvantIndexDansTab( indexCasesAAjouter, i, indexCasesAAjouter[i] ) != 0 )
					test1 = false;
				break;
			case( 0 ):
				if( nbOccurAvantIndexDansTab( indexCasesAAjouter, i, indexCasesAAjouter[i] ) == 0 )
					test1 = false;
				break;
			default:
				test1 = false;
				break;
		}
	}
	printOkKo( test1 );

	printf("\tTest de l'ajout : ");
;
	for( size_t i = 0 ; i < NOMBRE_AJOUT_TEST_AJOUTER ; i++ ){
		if( !graphe_caseTrouvee( g, indexCasesAAjouter[i] ) )
			retour = false;
	}
	printOkKo( retour );

	graphe_destroy( &g );

	printf("\tResultat:");

	return retour && test1;
}

void ajouterLaisonsDansGraphe( Graphe *g, int tab[], int tailleTab, int tabRetourFonctLiaison[] ){

	int retour;

	for( size_t i = 0 ; i < tailleTab ; i+=2 ){
		retour = graphe_ajouterArete( g , tab[i], tab[i+1]);
		if( tabRetourFonctLiaison != NULL )
			tabRetourFonctLiaison[i/2] = retour;
	}
}

#define NOMBRE_AJOUTS_TEST_AJOUTER_ARRETE 1000
#define NOMBRE_AJOUTS_ARRETES 200
#define TAILLE_TAB_ARRETES NOMBRE_AJOUTS_ARRETES * 2
bool test_graphe_ajouterArete( void ){
	Graphe g;

	bool retour = true;
	bool trouve = true;
	bool test1 =true;

	int indexCasesAAjouter[NOMBRE_AJOUTS_TEST_AJOUTER_ARRETE];
	int indexCasesLiees[TAILLE_TAB_ARRETES];
	int retourCasesLiees[NOMBRE_AJOUTS_ARRETES];

	for( size_t i = 0 ; i < NOMBRE_AJOUTS_TEST_AJOUTER_ARRETE ; i++ )
		do{
			indexCasesAAjouter[i] = rand();
		} while( nbOccurAvantIndexDansTab( indexCasesAAjouter, i , indexCasesAAjouter[i] ) != 0 );

	graphe_create( &g );

	ajouterTabDansGraphe( &g, indexCasesAAjouter, NOMBRE_AJOUTS_TEST_AJOUTER_ARRETE, NULL );

	for( size_t i = 0 ; i < TAILLE_TAB_ARRETES ; i+=2 ){
		indexCasesLiees[i] = indexCasesAAjouter[rand()%NOMBRE_AJOUTS_TEST_AJOUTER_ARRETE];
		do {
			indexCasesLiees[i+1] = indexCasesAAjouter[rand()%NOMBRE_AJOUTS_TEST_AJOUTER_ARRETE];
		} while(indexCasesLiees[i] == indexCasesLiees[i+1]);
	}

	ajouterLaisonsDansGraphe( &g, indexCasesLiees, TAILLE_TAB_ARRETES, retourCasesLiees );

	printf("\n\tTest du retour de la fonction : ");
	for( size_t i = 0 ; i < NOMBRE_AJOUTS_ARRETES ; i++ ){
		trouve = false;
		switch( retourCasesLiees[i] ){
			case( 1 ):
				for( size_t j = 0 ;j < i*2 ; j+=2 )
					if( (indexCasesLiees[j] == indexCasesLiees[i*2] && indexCasesLiees[j+1] == indexCasesLiees[i*2 +1]) 
					|| (indexCasesLiees[j] ==indexCasesLiees[i*2 +1] && indexCasesLiees[j+1] ==indexCasesLiees[i*2]) )  
						test1 = false;
				break;
			case( 0 ):
				for( size_t j = 0 ;j < i*2 ; j++ )
					if( (indexCasesLiees[j] == indexCasesLiees[i*2] && indexCasesLiees[j+1] == indexCasesLiees[i*2 +1]) 
					|| (indexCasesLiees[j] ==indexCasesLiees[i*2 +1] && indexCasesLiees[j+1] ==indexCasesLiees[i*2]) )  
						trouve = true;
					if( !trouve )
						test1 = false;
				break;	
			default:
				test1 = false;
				break;
		}
	}

	printOkKo( test1 );

	printf("\tTest des liasons : ");

	for( size_t i = 0; i < NOMBRE_AJOUTS_TEST_AJOUTER_ARRETE ; i++ )
		for( size_t j = i+1; j < NOMBRE_AJOUTS_TEST_AJOUTER_ARRETE ; j++ )
			if( i != j ){
				for( size_t k = 0 ; k < TAILLE_TAB_ARRETES && !trouve ; k+=2 ){
					trouve = false;
					if( (indexCasesAAjouter[i] == indexCasesLiees[k] && indexCasesAAjouter[j] == indexCasesLiees[k+1]) 
					|| (indexCasesAAjouter[j] == indexCasesLiees[k] && indexCasesAAjouter[i] == indexCasesLiees[k+1]) )
						retour = retour && graphe_sontSommetsAdjactents( g, indexCasesAAjouter[i], indexCasesAAjouter[j] );
						trouve = true;
		
				}
				if( !trouve )
					retour = retour && !graphe_sontSommetsAdjactents( g, indexCasesAAjouter[i], indexCasesAAjouter[j] );
			}

	printOkKo( retour );

	graphe_destroy( &g );

	printf("\tResultat:");

	return retour && test1;

}


#define NOMBRE_AJOUTS_SUPPRIMER 1000
#define NOMBRE_SUPPRESSIONS 200 
bool test_graphe_supprimeSommet(void){
	Graphe g;
	bool retour = true;
	bool trouve = false;
	int indexCasesAAjouter[NOMBRE_AJOUTS_SUPPRIMER];
	int indexCasesSupprimees[NOMBRE_SUPPRESSIONS];

	for( size_t i = 0 ; i < NOMBRE_AJOUTS_SUPPRIMER ; i++ )
		do{
			indexCasesAAjouter[i] = rand();
		} while( nbOccurAvantIndexDansTab( indexCasesAAjouter, i , indexCasesAAjouter[i] ) != 0 );

	graphe_create( &g );

	ajouterTabDansGraphe( &g, indexCasesAAjouter, NOMBRE_AJOUTS_SUPPRIMER, NULL );

	for( size_t i = 0 ; i < NOMBRE_SUPPRESSIONS ; i++ ){
		do{
			indexCasesSupprimees[i] = indexCasesAAjouter[rand()%NOMBRE_AJOUTS_SUPPRIMER];
		}while( nbOccurAvantIndexDansTab( indexCasesSupprimees, i , indexCasesSupprimees[i] ) != 0 );
		graphe_supprimeCase( &g, indexCasesSupprimees[i] );
	}

	for( size_t i = 0 ; i < NOMBRE_AJOUTS_SUPPRIMER ; i++ ){
		trouve = false;
		for( size_t j = 0 ; j < NOMBRE_SUPPRESSIONS && !trouve ; j++ ){
			if( indexCasesAAjouter[i] == indexCasesSupprimees[j] ){
				trouve = true;
				retour = retour && !graphe_caseTrouvee( g, indexCasesAAjouter[i] );
			}
		}
		if( !trouve )
			retour = retour && graphe_caseTrouvee( g, indexCasesAAjouter[i] );

	}

	graphe_destroy( &g );

	return retour;

}



int main(int argc, char const *argv[])
{
	srand(time(NULL));
	printf("TEST MODULE GRAPHE :\n\n");
	resultat_test_fonction("test_graphe_create",test_graphe_create);
	resultat_test_fonction("test_graphe_ajouterCase",test_graphe_ajouterCase);
	resultat_test_fonction("test_graphe_ajouterArete",test_graphe_ajouterArete);
	resultat_test_fonction("test_graphe_supprimeSommet",test_graphe_supprimeSommet);
	return 0;
}


