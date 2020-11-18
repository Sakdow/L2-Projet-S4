/************************************************************
 * TAD Grille                                               *
 *                                                          *
 *  --> Gestion du TAD Grille                               *
 *                                                          *
 * Timothée Thibault - Dernière modification : 03/05/2017   *
 *                                                          *
 * Groupe : Damien Sarrias, Nathan Roche                    *
 ************************************************************/

#include "grille.h"
#include "graphe.h"
#include "sommet.h"
#include "case.h"

#include <stdlib.h>

struct s_grille {
	Graphe g;
	unsigned taille;
};

void grille_numCaseToLigneColonne( const unsigned numCase, unsigned * l, unsigned *c, const unsigned tailleGrille ){
	*l = numCase / tailleGrille;
	*c = numCase % tailleGrille;
}

unsigned grille_ligneColonneToNumCase( unsigned l, unsigned c , unsigned tailleGrille ){
	return l*tailleGrille + c;
}

Grille grille_creer( Grille *gr, unsigned taille ){

	Case s;
	unsigned l,c;
	Sommet so;

	sommet_creer( &so, b );

	if( ((*gr) = malloc( sizeof( struct s_grille ) )) == NULL )
		return NULL;
	if( graphe_create( &(*gr)->g ) == NULL ){
		return NULL;
	}

	(*gr)->taille = ( taille == DEFAUT || taille < TAILLE_MIN || taille > TAILLE_MAX ) ? TAILLE_QUAND_DEFAUT : taille;

	/*ajout des bords*/
	for( int i = -1 ; i >= -4  ; i-- ){
		if( i%2 == 0 )
			sommet_changerCouleur( &so, b );
		else
			sommet_changerCouleur( &so, w );

		case_creer( &s, i, so );
		graphe_ajouterCase( &(*gr)->g , i, s );
		case_detruire( &s );
	}

	sommet_changerCouleur( &so, t );


	/*ajout des cases*/
	for( unsigned i = 0 ; i < (*gr)->taille * (*gr)->taille ; i++ ){

		case_creer( &s, i, so );
		graphe_ajouterCase( &(*gr)->g , (int)i, s );
		case_detruire( &s );

		grille_numCaseToLigneColonne( i, &l,&c,(*gr)->taille );

		/*haut*/
		if( l == 0 )
			graphe_ajouterArete( &(*gr)->g,(int)i , BORD_HAUT );
		else{
			graphe_ajouterArete( &(*gr)->g,(int)i , (int)grille_ligneColonneToNumCase( l - 1, c, (*gr)->taille ) );
			if( c != (*gr)->taille - 1 )
				graphe_ajouterArete( &(*gr)->g,(int)i , (int)grille_ligneColonneToNumCase( l - 1, c + 1, (*gr)->taille ) );
		}

		/*cote*/
		if( c == 0 )
			graphe_ajouterArete( &(*gr)->g,(int)i , BORD_GAUCHE );
		else
			graphe_ajouterArete( &(*gr)->g,(int)i , (int)grille_ligneColonneToNumCase( l, c - 1, (*gr)->taille ) );

		if( c == (*gr)->taille - 1 )
			graphe_ajouterArete( &(*gr)->g,(int)i , BORD_DROIT );

		/*bas*/
		if( l == (*gr)->taille - 1 )
			graphe_ajouterArete( &(*gr)->g,(int)i , BORD_BAS );
	}

	sommet_detruire( &so );

	return *gr;
}

void grille_supprimer( Grille *gr ){
	graphe_destroy( &(*gr)->g );
	free( *gr );
}

Case grille_caseX( Grille gr, int x ){
	return * graphe_caseNumeroI( gr->g, x );
}

void grille_modifierCouleur( Grille *gr, const int x, s_couleur couleur ){

	Case * aModifier;
	Sommet sAModifier;
	aModifier = graphe_caseNumeroI( (*gr)->g, x );
	sAModifier = case_getSommet(*aModifier);
	sommet_changerCouleur( &sAModifier, couleur );
}

unsigned grille_obtenirTaille( Grille gr ){
	return gr->taille;
}

void grille_caseAdjacentes( Grille gr, const unsigned numCase, Case tabSommetsAdj[NB_ADJACENTS_MAX_PAR_CASE] ){
	int * tabResultatsAdjacents = malloc( sizeof( int ) );
	size_t tailleResultatsAdj = 0;

	graphe_indexsCasesAdjacents( gr->g, numCase, &tabResultatsAdjacents, &tailleResultatsAdj );

	for( size_t i = 0 ; i < NB_ADJACENTS_MAX_PAR_CASE ; i++ ){
		if( i < tailleResultatsAdj )
			tabSommetsAdj[i] = grille_caseX( gr, tabResultatsAdjacents[i] ) ;
		else
			tabSommetsAdj[i] = NULL;
	}
	
	free( tabResultatsAdjacents );
}

void grille_copie( Grille * grCopie, const Grille gr ){
	grille_creer( grCopie, gr->taille );
	for( size_t i = 0 ; i < gr->taille * gr->taille ; i++ ){
		s_couleur couleurCase = sommet_getCouleur( case_getSommet( grille_caseX( gr, i ) ) );
		if( couleurCase != t )
			grille_modifierCouleur( grCopie, i, couleurCase );
	}
}

void grille_toString( Grille grille, char ** chaine ){

	unsigned nbCases =  grille->taille * grille->taille;

	if( (*chaine = malloc( nbCases + 1 ) ) == NULL )
		abort();

	for( unsigned i = 0 ; i < nbCases ; i++ ){
		if( sommet_getCouleur( case_getSommet( grille_caseX( grille, (int)i ) ) ) == t ) 
			(*chaine)[i] = CARACTERE_POUR_COULEUR_T;
		else if( sommet_getCouleur( case_getSommet( grille_caseX( grille, (int)i ) ) ) == w ) 
			(*chaine)[i] = CARACTERE_POUR_COULEUR_W;
		else 
			(*chaine)[i] = CARACTERE_POUR_COULEUR_B;
	}
	(*chaine)[nbCases] = '\0';
}

int grille_comparer( const Grille g1, const Grille g2 ){
	int nbCasesG1 = g1->taille * g1->taille;
	if( g1->taille != g2->taille )
		return 1;


	for( unsigned i = 0; i < nbCasesG1 ; i++ )
		if( sommet_getCouleur( case_getSommet( grille_caseX( g1, (int)i ) ) ) != sommet_getCouleur( case_getSommet( grille_caseX( g2, (int)i ) ) ) )
			return 2;

	if( ( sommet_getCouleur( case_getSommet( grille_caseX( g1, BORD_HAUT ) ) ) != sommet_getCouleur ( case_getSommet( grille_caseX( g2, BORD_HAUT ) ) ) )
	|| ( sommet_getCouleur( case_getSommet( grille_caseX( g1, BORD_BAS ) ) ) != sommet_getCouleur ( case_getSommet( grille_caseX( g2, BORD_BAS ) ) ) )
	|| ( sommet_getCouleur( case_getSommet( grille_caseX( g1, BORD_DROIT ) ) ) != sommet_getCouleur ( case_getSommet( grille_caseX( g2, BORD_DROIT ) ) ) )
	|| ( sommet_getCouleur( case_getSommet( grille_caseX( g1, BORD_GAUCHE ) ) ) != sommet_getCouleur ( case_getSommet( grille_caseX( g2, BORD_GAUCHE ) ) ) ) )
		return 2;

	return 0;
}
