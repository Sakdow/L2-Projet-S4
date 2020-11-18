/************************************************************
 * TAD IA                                                   *
 *                                                          *
 *  --> Gestion du TAD IA                                   *
 *                                                          *
 * Timothée Thibault - Dernière modification : 21/05/2017   *
 *                                                          *
 * Groupe : Damien Sarrias, Nathan Roche                    *
 ************************************************************/


#include "IA.h"
#include "grille.h"
#include "pile.h"
#include "jeu.h"
#include "case.h"
#include "joueur.h"
#include "sommet.h"
#include "groupe.h"
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

typedef struct s_feuilleArbre {
	unsigned caseModifiee;
	int poids;
	unsigned nbFils;
	unsigned nbCoupsJoueesDansSim;
	bool estJoueParIA;
	struct s_feuilleArbre * parent;
	struct s_feuilleArbre ** fils;
} * feuilleArbre;

struct s_simulationCoups{
	int nbCoupsJoues;
	InfosJeu jeuActu;
	feuilleArbre arbreJeu;
	Joueur joueurRobot;
};

/** @brief retourne si la case est un bord
*
*/
static bool _caseEstUnBord( Case c );

#define PREMIER_APPEL 1
#define APPEL_RECURSIF 0

/** @brief premet de calculer la distance minimale entre deux cases ( = le nombre de coups qu'il faudrait pour les relier ), en ajoutant que des cases de la couleur de /cc1
*
* @return -1 si joindre les deux cases et impossible par ce chemin , la distance entre les deux cases sinon
* @warning Utiliser /cPREMIER_APPEL lorsque cette fonction est appelée en dehors d'elle-même, /cAPPEL_RECURSIF sinon.
*/
static int _distanceEntreDeuxCases( Grille grille, Case c1, Case c2, bool * casesVisitees, int taille, int appel );

/** @brief permet de calculer la distance minimale entre deux groupes
*
*/
static int _distanceEntreGroupes( Grille grille, Groupe groupe1, Groupe groupe2 );

/** @brief permet de calculer lenonmbre de coup minimal qu'il faut à un joueur pour gagner ( si on considèrequ'il joue tout le temps )
*
*/
static int _distance_victoire( InfosJeu jeu, Joueur j );

/** @brief permet de calculer lepoids d'une case dans une simulation
*
*/
static int _calculPoidsCase( InfosJeu jeu, Joueur j );

/** @brief permet de faire une simulation ( i.e : remplir un arbre de jeu )
*
*/
static void _simuler( simulationCoups sc, feuilleArbre * feuille, InfosJeu jeuProv );

#define CASE_MODIFIEE_NULLE 0xffffffff
/** @brief permet de créer un pointeur vers un arbre de jeu représentant le jeu dans l'état qu'il est actuellement
*
* @note le champ caseModifié sera initialisé à \cCASE_MODIFIEE_NULLE
*/
static feuilleArbre _creerArbreJeu( InfosJeu jeu );

/** @brief permet de trouver le fils de la feuille passée en paramètre qui modifie la case \cnumCase 
*
* @return le fils s'il a été trouvé, \cNULL sinon
*/
static feuilleArbre _filsModifiantCaseX( feuilleArbre feuille, unsigned numCase );

/** @brief permet de libérer la mémoire allouée à une feuille de l'arbre de jeu
*
* @warning son parent et ses fils ne sont pas mis à jour lors de la suppression
*/
static void _supprimerFeuille( feuilleArbre * feuille );

/** @brief permet de supprimer un sous-arbre en ne gardant que le sous-sous-arbre \cfilsAGarder
*
* @param si \cfilsAGarder est \cNULL, supprime tout le sous-arbre
* @note le parent de filsAGarder est mis à jour
* @warning le fils du parent du sous-arbre correspondant au sous-arbre \cparent n'est pas mis à jour
*/
static void _supprimerParentEtBranchesFillesSaufUne( feuilleArbre *parent, feuilleArbre filsAGarder );

/** @brief permet de mettre à jour une simulation
*
*/
static void _miseAJourArbreJeu( simulationCoups *sc );

/** @brief permet de calculer le coefficient minimax d'un arbre de jeu
*
* @param filsFaisantCeMiniMax pointera vers le fils de la racine du sous-arbre faisant ce coefficient minimax
*/
static int miniMax( feuilleArbre feuille, feuilleArbre * filsFaisantCeMiniMax );




static bool _caseEstUnBord( Case c ){
	return case_getNumeroCase( c ) == BORD_HAUT    || 
		   case_getNumeroCase( c ) == BORD_BAS     || 
		   case_getNumeroCase( c ) == BORD_GAUCHE  || 
		   case_getNumeroCase( c ) == BORD_DROIT;
}


static int _distanceEntreDeuxCases( Grille grille, Case c1, Case c2, bool * casesVisitees, int taille, int appel ){

	Case adjacentes[NB_ADJACENTS_MAX_PAR_CASE];
	int distance = -1;
	int distanceRetournee = -1;
	s_couleur couleurC1 =sommet_getCouleur( case_getSommet( c1 ) );

	bool *caseVisitees;

	if( ( caseVisitees = malloc( sizeof( bool ) * taille ) ) == NULL )
			abort();

	if( appel == PREMIER_APPEL ){

		for( int i = 0; i < taille ; i++ )
			caseVisitees[i] = false;
	} else {
		for( int i = 0; i < taille ; i++ )
			caseVisitees[i] = casesVisitees[i];
	}

	if( !_caseEstUnBord( c1 ) ){

		grille_caseAdjacentes( grille, case_getNumeroCase( c1 ), adjacentes );
		caseVisitees[case_getNumeroCase( c1 )] = true;

		for( int i = 0 ; i < NB_ADJACENTS_MAX_PAR_CASE && adjacentes[i] != NULL ; i++ ){

			if( case_getNumeroCase(adjacentes[i]) == case_getNumeroCase( c2 ) ){
				free( caseVisitees );
				return 0;
			}
			else if ( !_caseEstUnBord( adjacentes[i] ) && !caseVisitees[case_getNumeroCase( adjacentes[i] )] ){

				if( sommet_getCouleur( case_getSommet( adjacentes[i] ) ) == t )
					distanceRetournee = 1 + _distanceEntreDeuxCases( grille, adjacentes[i], c2, caseVisitees, taille, APPEL_RECURSIF );
				else if( sommet_getCouleur( case_getSommet( adjacentes[i] ) ) == couleurC1)
					distanceRetournee = _distanceEntreDeuxCases( grille, adjacentes[i], c2, caseVisitees, taille, APPEL_RECURSIF );
				else
					distanceRetournee = -1;
			} else {
				distanceRetournee = -1;
			}

			distance = ( distance == -1 || ( distanceRetournee != -1 && distanceRetournee < distance ) ) ? distanceRetournee : distance;
		}
	}
	free( caseVisitees );
	return distance;
}

static int _distanceEntreGroupes( Grille grille, Groupe groupe1, Groupe groupe2 ){
	int min = -1;
	int distanceCases;

	Case * casesGroupe1, *casesGroupe2;
	casesGroupe1 = groupe_getTableauCases( groupe1 );
	casesGroupe2 = groupe_getTableauCases( groupe2 );

	bool * casesVisitees;
	int nbCases = grille_obtenirTaille( grille ) * grille_obtenirTaille( grille );

	for( int iGroupe1 = 0 ; iGroupe1 < groupe_getTaille( groupe1 ) ; iGroupe1++ ){
		for( int iGroupe2 = 0 ; iGroupe2 < groupe_getTaille( groupe2 ) ; iGroupe2++ ){
			if( ( casesVisitees = malloc( sizeof( bool ) * nbCases ) ) == NULL )
				abort();
			distanceCases = _distanceEntreDeuxCases( grille, casesGroupe1[iGroupe1], casesGroupe2[iGroupe2], casesVisitees, nbCases, PREMIER_APPEL );
			if( min == -1 || distanceCases < min )
				min = distanceCases;
			free(casesVisitees);
		}
	}


	return min;
}

static int _distance_victoire( InfosJeu jeu, Joueur j ){ 
	Grille grille = jeu_getGrille( jeu );
	Case bord1 = ( joueur_getCouleur( j ) == w )  ? grille_caseX( grille, BORD_HAUT ) : grille_caseX( grille, BORD_GAUCHE );
	Case bord2 = ( joueur_getCouleur( j ) == w )  ? grille_caseX( grille, BORD_BAS ) : grille_caseX( grille, BORD_DROIT );

	Groupe groupeBord1;
	Groupe groupeBord2;
	Groupe * groupesJoueur;
	groupesJoueur = joueur_getGroupesJoueur( j );

	bool gB1Trouve, gB2Trouve;
	gB2Trouve = gB1Trouve = false;

	int min = -1, distance = -1;

	for( int i = 0 ; !gB1Trouve || !gB2Trouve ; i++ ){
		if( !gB1Trouve && groupe_appartenir( groupesJoueur[i], bord1 ) ){
			gB1Trouve   = true;
			groupeBord1 = groupesJoueur[i];
		}
		if( !gB2Trouve && groupe_appartenir( groupesJoueur[i], bord2 ) ){
			gB2Trouve   = true;
			groupeBord2 = groupesJoueur[i];
		}
	}

	if( groupe_getTaille( groupeBord1 ) == 1 ){
		if( groupe_getTaille( groupeBord2 ) > 1 )
			return _distanceEntreGroupes( grille, groupeBord2, groupeBord1 );
		if( joueur_getNbGroupes( j ) == 2 )
			return grille_obtenirTaille( jeu_getGrille( jeu ) );
		for( int i = 0 ; i < joueur_getNbGroupes( j ) ; i++ ){
			if( !groupe_appartenir( groupesJoueur[i], bord1 )&& !groupe_appartenir( groupesJoueur[i], bord2 ) ){
				distance = _distanceEntreGroupes(grille, groupesJoueur[i], groupeBord1 ) + _distanceEntreGroupes(grille, groupesJoueur[i], groupeBord2 );
				if( min == -1 || distance < min )
					min = distance;
			}

		}
		return distance;
	}
	return _distanceEntreGroupes( grille, groupeBord1, groupeBord2 );
}

static int _calculPoidsCase( InfosJeu jeu, Joueur j ){
	int tailleGrille = (int)grille_obtenirTaille( jeu_getGrille( jeu ) ) * grille_obtenirTaille( jeu_getGrille( jeu ) );
	int distanceJ, distanceAdv;
	Joueur joueurAdverse = ( j == jeu_getJoueur1( jeu ) ) ? jeu_getJoueur2( jeu ) : jeu_getJoueur1( jeu );

	if( jeu_testVictoire( jeu, j ) )
		return 1;
	if( jeu_testVictoire( jeu, joueurAdverse ) )
		return -1;
	distanceJ = _distance_victoire( jeu, j );
	distanceAdv = _distance_victoire( jeu, joueurAdverse );

	return tailleGrille - distanceJ + distanceAdv ; 
}

static void _simuler( simulationCoups sc, feuilleArbre * feuille, InfosJeu jeuProv ){

	InfosJeu jeuSimul;
	Grille grilleProv = jeu_getGrille( jeuProv );
	int nbCasesGrille = (int) ( grille_obtenirTaille( grilleProv ) * grille_obtenirTaille( grilleProv ) );
	int numCase = -1;
	unsigned ligne,colonne;
	Joueur joueurSimule;

	if( (*feuille)->nbFils == 0 )
		return;

	if( (int)(*feuille)->nbCoupsJoueesDansSim - NB_COUPS_SIMULES < jeu_getNbCoups(sc->jeuActu) ){

		if( (*feuille)->fils == NULL ){
			if( ((*feuille)->fils = malloc( sizeof( feuilleArbre ) * (*feuille)->nbFils )) == NULL )
				abort();

			for( unsigned numFils = 0 ; numFils < (*feuille)->nbFils ; numFils++ ){

				if( ((*feuille)->fils[numFils] = malloc( sizeof( struct s_feuilleArbre ) )) == NULL )
					abort();

				for( numCase++ ; sommet_getCouleur( case_getSommet( grille_caseX( jeu_getGrille( jeuProv ), numCase ) ) ) != t ; numCase++ );
				assert( numCase >= 0 && numCase < nbCasesGrille );
				(*feuille)->fils[numFils]->caseModifiee         = (unsigned)numCase;
				(*feuille)->fils[numFils]->nbFils               = (*feuille)->nbFils - 1;
				(*feuille)->fils[numFils]->nbCoupsJoueesDansSim = (*feuille)->nbCoupsJoueesDansSim + 1;
				(*feuille)->fils[numFils]->estJoueParIA         = !(*feuille)->estJoueParIA;
				(*feuille)->fils[numFils]->parent               = (*feuille);
				(*feuille)->fils[numFils]->fils                 = NULL;
				
				jeuSimul                                        = jeu_copie( jeuProv );
				grille_numCaseToLigneColonne( numCase, &ligne, &colonne, grille_obtenirTaille( grilleProv ) );
				joueurSimule                                    = jeu_quiDoitJouer( jeuSimul );
				jeu_jouerCoup( &jeuSimul,joueurSimule, ligne, colonne );
				(*feuille)->fils[numFils]->poids                = _calculPoidsCase( jeuSimul, joueurSimule );

				if( !(*feuille)->fils[numFils]->estJoueParIA )
					(*feuille)->fils[numFils]->poids            = -(*feuille)->fils[numFils]->poids;
				if( (*feuille)->fils[numFils]->poids != -1 && (*feuille)->fils[numFils]->poids != 1 )
					_simuler( sc, &(*feuille)->fils[numFils], jeuSimul );
				else
					(*feuille)->fils[numFils]->nbFils           = 0;

				jeu_detruire( &jeuSimul );

			}
		} else {
			for( unsigned numFils = 0 ; numFils < (*feuille)->nbFils ; numFils++ ){
				jeuSimul                                        = jeu_copie( jeuProv );
				grille_numCaseToLigneColonne( (*feuille)->fils[numFils]->caseModifiee, &ligne, &colonne, grille_obtenirTaille( grilleProv ) );
				joueurSimule                                    = jeu_quiDoitJouer( jeuSimul );
				jeu_jouerCoup( &jeuSimul,joueurSimule, ligne, colonne );
				_simuler( sc, &(*feuille)->fils[numFils], jeuSimul );
			}
		}
	}
}
static feuilleArbre _creerArbreJeu( InfosJeu jeu ){
	feuilleArbre arbreJeu;
	if( (arbreJeu = malloc( sizeof( struct s_feuilleArbre ) )) == NULL )
		abort();
	arbreJeu->estJoueParIA         = !joueur_estIA( jeu_quiDoitJouer( jeu ) );
	arbreJeu->caseModifiee         = CASE_MODIFIEE_NULLE;
	arbreJeu->poids                = 0;
	arbreJeu->parent               = NULL;
	arbreJeu->nbFils               = grille_obtenirTaille( jeu_getGrille( jeu ) ) * grille_obtenirTaille( jeu_getGrille( jeu ) ) - jeu_getNbCoups(jeu);
	arbreJeu->nbCoupsJoueesDansSim = jeu_getNbCoups(jeu);
	arbreJeu->fils                 = NULL;

	return arbreJeu;
}

void IA_creerSimulation( simulationCoups * sc, InfosJeu jeu, Joueur joueurIA ){
	if( (*sc = malloc( sizeof( struct s_simulationCoups ) )) == NULL )
		abort();

	(*sc)->jeuActu      = jeu;
	(*sc)->joueurRobot  = joueurIA;
	(*sc)->nbCoupsJoues = 0;
	(*sc)->arbreJeu     = _creerArbreJeu( jeu );
	_simuler( *sc, &(*sc)->arbreJeu, (*sc)->jeuActu );
}

static feuilleArbre _filsModifiantCaseX( feuilleArbre feuille, unsigned numCase ){

	if( feuille->fils != NULL ){
		for( unsigned numFils = 0 ; numFils < feuille->nbFils ; numFils++ )
			if( feuille->fils[numFils]->caseModifiee == numCase )
				return feuille->fils[numFils];
	}

	return NULL;
}

static void _supprimerFeuille( feuilleArbre * feuille ){
	if( (*feuille)->fils != NULL )
		free( (*feuille)->fils );
	free( (*feuille) );
}

static void _supprimerParentEtBranchesFillesSaufUne( feuilleArbre *parent, feuilleArbre filsAGarder ){
	if( filsAGarder != NULL ){
		filsAGarder->parent = (*parent)->parent;
	}
	if( (*parent)->fils != NULL && (*parent)->nbFils != 0 )
		for( unsigned i = 0; i < (*parent)->nbFils ; i++ )
			if( (*parent)->fils[i] != filsAGarder )
				_supprimerParentEtBranchesFillesSaufUne( &(*parent)->fils[i], NULL );

	_supprimerFeuille( parent );
}

void _supprimerArbreJeu( feuilleArbre * feuille ){
	_supprimerParentEtBranchesFillesSaufUne( feuille, NULL );
}

static void _miseAJourArbreJeu( simulationCoups *sc ){
	feuilleArbre f;

	Pile pInverseeCoupsJouees, pCopie;
	Case dernierCoup;

	int nbCoupsDifferences = jeu_getNbCoups( (*sc)->jeuActu ) - (*sc)->nbCoupsJoues;
	unsigned caseAModifier;

	InfosJeu i = (*sc)->jeuActu;
	if( !pile_vide( jeu_getPile( (*sc)->jeuActu ) ) )
		dernierCoup = pile_sommet( jeu_getPile( (*sc)->jeuActu ) );

	/*Si un coup a été anulé*/
	if( nbCoupsDifferences < 0 
		|| ( !pile_vide( jeu_getPile( (*sc)->jeuActu ) ) && (*sc)->arbreJeu->caseModifiee != CASE_MODIFIEE_NULLE && case_getNumeroCase( dernierCoup ) != (*sc)->arbreJeu->caseModifiee ) ){
		_supprimerArbreJeu( &(*sc)->arbreJeu );
		(*sc)->arbreJeu = _creerArbreJeu( i );
		(*sc)->nbCoupsJoues = jeu_getNbCoups( (*sc)->jeuActu );
		_miseAJourArbreJeu( sc );

	} else {

		pile_initialiser( &pInverseeCoupsJouees );
		pCopie = pile_copie( jeu_getPile( (*sc)->jeuActu ) );

		for( int i = 0 ; i < nbCoupsDifferences ; i++ ){
			pile_empiler( &pInverseeCoupsJouees, pile_sommet(pCopie) );
			pile_depiler( &pCopie );
		}
		pile_detruire( &pCopie );

		for( ; !pile_vide( pInverseeCoupsJouees ) ; ){

			caseAModifier = (unsigned)case_getNumeroCase( pile_sommet( pInverseeCoupsJouees ));
			f = _filsModifiantCaseX((*sc)->arbreJeu,caseAModifier);
			if( f == NULL ){
				if( (f = malloc( sizeof( struct s_feuilleArbre ) ) ) == NULL )
					abort();
				f->caseModifiee         = caseAModifier;
				f->nbFils               = (*sc)->arbreJeu->nbFils - 1;
				f->nbCoupsJoueesDansSim = (*sc)->arbreJeu->nbCoupsJoueesDansSim + 1;
				f->estJoueParIA         = !(*sc)->arbreJeu->estJoueParIA;
				f->parent               = NULL;
				f->fils                 = NULL;
				_supprimerParentEtBranchesFillesSaufUne( &(*sc)->arbreJeu, NULL );

			} else {
				_supprimerParentEtBranchesFillesSaufUne( &(*sc)->arbreJeu, f );
			}	
			(*sc)->arbreJeu = f;
			(*sc)->nbCoupsJoues++;
			pile_depiler( &pInverseeCoupsJouees );
		}

		_simuler( *sc, &(*sc)->arbreJeu, (*sc)->jeuActu );
	}
}

static int miniMax( feuilleArbre feuille, feuilleArbre * filsFaisantCeMiniMax ){
	int retour, minMaxRec;
	if( feuille->fils == NULL || feuille->nbFils == 0 ){
		*filsFaisantCeMiniMax = feuille;
		return feuille->poids;
	}

	retour = miniMax( feuille->fils[0], filsFaisantCeMiniMax );
	*filsFaisantCeMiniMax = feuille->fils[0];

	for( unsigned i = 1 ; i < feuille->nbFils ; i++ ){
		if( ( feuille->estJoueParIA && ( minMaxRec = miniMax( feuille->fils[i], filsFaisantCeMiniMax ) ) > retour )
			|| ( !feuille->estJoueParIA && ( minMaxRec = miniMax( feuille->fils[i], filsFaisantCeMiniMax ) ) < retour ) ){

				retour = minMaxRec;
				*filsFaisantCeMiniMax = feuille->fils[i];
		}
	}

	return retour;
}

unsigned IA_jouerCase( simulationCoups * sc ){
	feuilleArbre filsChoix;
	_miseAJourArbreJeu( sc );
	miniMax( (*sc)->arbreJeu, &filsChoix );
	return filsChoix->caseModifiee;
}

void IA_detuireSimulation( simulationCoups *sc ){
	assert( sc != NULL );
	_supprimerArbreJeu( &(*sc)->arbreJeu );
	free( *sc );
}

