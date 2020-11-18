/************************************************************
 * TAD Graphe                                               *
 *                                                          *
 *  --> Gestion du TAD Graphe                               *
 *                                                          *
 * Timothée Thibault - Dernière modification : 16/04/2017   *
 *                                                          *
 * Groupe : Damien Sarrias, Nathan Roche                    *
 ************************************************************/

#include <stdlib.h>
#include "case.h"
#include "graphe.h"

#define TAILLE_TAB_TABLE_HACHAGE 100

typedef struct s_listeChainee{
	struct s_listeChainee * precedent;
	struct s_listeChainee * suivant;
	void * el;
} * listeChainee;

typedef struct s_element{
	listeChainee ensembleE;
	int valeur;
	Case s;
} * element;

typedef struct s_tableHachage{
	listeChainee tabHachage[TAILLE_TAB_TABLE_HACHAGE];
	size_t (*fonctionHachage)( const int );
} *tableHachage;

struct s_graphe{
	tableHachage ensembleV;
};

/** @brief fonction de hachage, utilisant un modulo
*
*/
static size_t _fonctHachModulo( const int i );

/** @brief premet de créer la table de hachage
*
* @warning th ne doit pas être initialisée, risque de fuites de mémoire sinon
*/
static void _creerTableHachage( tableHachage * th, size_t (*fonctionHachage)( const int ) );

/** @brief crée un pointeur sur une structure s_element
*
*/
static element _creerElement( element * el, int valeur,const Case s );

/** @brief permet de creer un noeud d'une liste chainée. Ce noeud boucle sur lui même
*
*/
static listeChainee _creerNoeudListeChainee( listeChainee * lst, void * el );

/** @brief libère la place allouée à un noeud d'une liste chaînée
*
*/
static void _libererNoeudListe( listeChainee *noeud );

/** @brief libère la place allouée à une liste chainée 
*
*/
static void _libererListeChainee( listeChainee * lst );

/** @brief permet de libérer la place allouée à un pointeur vers une structure s_element
*
*/
static void _libererEl( element *el );






static size_t _fonctHachModulo( const int i ){
	return (size_t) i % TAILLE_TAB_TABLE_HACHAGE;
}

static void _creerTableHachage( tableHachage * th, size_t (*fonctionHachage)( const int ) ){
	if( (*th = malloc( sizeof(struct s_tableHachage) )) == NULL  )
		abort();

	(*th)->fonctionHachage = fonctionHachage;
	for( size_t i = 0 ; i < TAILLE_TAB_TABLE_HACHAGE ; i++ )
		(*th)->tabHachage[i] = NULL; 

}

Graphe graphe_create( Graphe * g ){
	if( ((*g) = malloc( sizeof(struct s_graphe ) )) == NULL )
		return NULL;
	_creerTableHachage( &(*g)->ensembleV, _fonctHachModulo );
	return *g;
}

static element _creerElement( element * el, int valeur,const Case s ){
	if( ( *el = malloc( sizeof(struct s_element) )) == NULL )
		return NULL;
	(*el)->ensembleE = NULL;
	(*el)->valeur = valeur;
	case_copie(&(*el)->s, s);
	return *el;
}

static listeChainee _creerNoeudListeChainee( listeChainee * lst, void * el ){
	if( (*lst = malloc( sizeof(struct s_listeChainee) )) == NULL )
		return NULL;
	(*lst)->precedent = *lst;
	(*lst)->suivant = *lst;
	(*lst)->el = el;
	return *lst;
}

static void _libererNoeudListe( listeChainee *noeud ){
	listeChainee noeudProv;
	if( (*noeud)->suivant == (*noeud) ){
		free(*noeud);
		*noeud = NULL;
	} else {
		noeudProv = (*noeud)->precedent->suivant = (*noeud)->suivant;
		(*noeud)->suivant->precedent = (*noeud)->precedent;
		free(*noeud);
		*noeud = noeudProv;
	} 
}

static void _libererListeChainee( listeChainee * lst ){
	if( *lst != NULL ){
		listeChainee depart, noeudSuiv, noeudActu;
		depart    = *lst;
		noeudActu = depart;

		for( ; ; ){
			noeudSuiv = noeudActu->suivant;
			free( noeudActu );
			if( noeudSuiv == depart )
				return;
			noeudActu = noeudSuiv;
		}
	}
}

static void _libererEl( element *el ){
	_libererListeChainee( &(*el)->ensembleE );
	case_detruire( &(*el)->s );
	free( *el );
}

int graphe_ajouterCase( Graphe *g, const int num, const Case s ){
	size_t indexHach;
	element elAAjouter, elRecherche;
	listeChainee noeudDeListeAAjouter, caseITablehachage;

	if( _creerElement( &elAAjouter, num, s ) == NULL )
		return -1;

	if( _creerNoeudListeChainee( &noeudDeListeAAjouter, (void *)elAAjouter ) == NULL ){
		_libererEl( &elAAjouter );
		return -1;
	}

	indexHach = (*g)->ensembleV->fonctionHachage( num );
	caseITablehachage = (*g)->ensembleV->tabHachage[indexHach];

	if( caseITablehachage == NULL ){
		(*g)->ensembleV->tabHachage[indexHach] = noeudDeListeAAjouter;
		return 1;
	}

	for( listeChainee lst = caseITablehachage ; ; ){
		elRecherche = (element)lst->el;
		if( elRecherche->valeur == num ){
			_libererEl( &elAAjouter );
			_libererListeChainee( &noeudDeListeAAjouter );
			return 0;
		}

		if( ( lst = lst->suivant ) == caseITablehachage ) break;
	}

	caseITablehachage->precedent->suivant = noeudDeListeAAjouter;
	noeudDeListeAAjouter->precedent       = caseITablehachage->precedent;
	caseITablehachage->precedent          = noeudDeListeAAjouter;
	noeudDeListeAAjouter->suivant         = caseITablehachage;

	return 1;

}


int graphe_ajouterArete( Graphe *g, const int num1, const int num2 ){
	element el1, el2;
	size_t hachEl1, hachEl2;
	listeChainee caseEl1TableHach, caseEl2TableHach, lst, noeudAAjouter;

	hachEl1 = (*g)->ensembleV->fonctionHachage( num1 );
	hachEl2 = (*g)->ensembleV->fonctionHachage( num2 );

	caseEl1TableHach = (*g)->ensembleV->tabHachage[hachEl1];
	caseEl2TableHach = (*g)->ensembleV->tabHachage[hachEl2];

	for( lst = caseEl1TableHach ; ((element)(lst->el))->valeur != num1 ; lst = lst->suivant );
	el1 = (element)lst->el;
	
	for( lst = caseEl2TableHach ; ((element)lst->el)->valeur != num2 ; lst = lst->suivant );
	el2 = (element)lst->el;

	for( lst = el1->ensembleE ; lst != NULL ; ){
		if( (element)(lst->el) == el2 ) return 0;
		if( (lst = lst->suivant) == el1->ensembleE ) break;
	}

	_creerNoeudListeChainee( &noeudAAjouter, (void *)el2 );
	if( el1->ensembleE == NULL )
		el1->ensembleE = noeudAAjouter;
	else {
		el1->ensembleE->precedent->suivant = noeudAAjouter;
		noeudAAjouter->precedent           = el1->ensembleE->precedent;
		el1->ensembleE->precedent          = noeudAAjouter;
		noeudAAjouter->suivant             = el1->ensembleE;
	}

	_creerNoeudListeChainee( &noeudAAjouter, (void *)el1 );
	if( el2->ensembleE == NULL )
		el2->ensembleE = noeudAAjouter;
	else {
		el2->ensembleE->precedent->suivant = noeudAAjouter;
		noeudAAjouter->precedent           = el2->ensembleE->precedent;
		el2->ensembleE->precedent          = noeudAAjouter;
		noeudAAjouter->suivant             = el2->ensembleE;
	}

	return 1;
}

Case * graphe_caseNumeroI( Graphe g, const int num ){
	size_t hachNum;

	hachNum = g->ensembleV->fonctionHachage( num );

	if( g->ensembleV->tabHachage[hachNum] == NULL ) return NULL;

	for( listeChainee lst = g->ensembleV->tabHachage[hachNum] ; ; ){
		if( ((element)( lst->el ))->valeur == num ) return &((element)( lst->el ))->s;
		if( ( lst = lst->suivant ) == g->ensembleV->tabHachage[hachNum] ) return NULL;
	}
}

bool graphe_caseTrouvee( Graphe g, const int num ){
	size_t hachNum;

	hachNum = g->ensembleV->fonctionHachage( num );

	if( g->ensembleV->tabHachage[hachNum] == NULL ) return false;

	for( listeChainee lst = g->ensembleV->tabHachage[hachNum] ; ; ){
		if( ((element)( lst->el ))->valeur == num ) return true;
		if( ( lst = lst->suivant ) == g->ensembleV->tabHachage[hachNum] ) return false;
	}
}

bool graphe_sontSommetsAdjactents( Graphe g, const int numS1, const int numS2 ){
	size_t hachNum;
	element el;
	listeChainee caseITablehachage, lst;

	hachNum = g->ensembleV->fonctionHachage( numS1 );

	caseITablehachage = g->ensembleV->tabHachage[hachNum];

	for( lst = caseITablehachage ; ((element)( lst->el ))->valeur != numS1 ; lst = lst->suivant );
	el = (element)( lst->el );

	if( el->ensembleE == NULL ) return false;

	for( lst = el->ensembleE ; ; ){
		if( ((element)( lst->el ))->valeur == numS2 ) return true;
		if( (lst = lst->suivant) == el->ensembleE ) return false;
	}
}

void graphe_supprimeCase( Graphe *g, const int num ){
	size_t hachNum;
	element el, elAModifier;
	listeChainee caseITablehachage, lst, l;

	hachNum = (*g)->ensembleV->fonctionHachage( num );

	caseITablehachage = (*g)->ensembleV->tabHachage[hachNum];

	for( lst = caseITablehachage ; ((element)( lst->el ))->valeur != num ; lst = lst->suivant );
	el = (element)( lst->el );
	_libererNoeudListe( &lst );
	(*g)->ensembleV->tabHachage[hachNum] = lst;

	if( el->ensembleE != NULL ){
		for( lst = el->ensembleE ; ; ){
			elAModifier = (element)lst->el;
			for( l = elAModifier->ensembleE ; (element)(l->el) != el ; l = l->suivant );
			_libererNoeudListe( &l );
			if( ( lst = lst->suivant ) == el->ensembleE ) break;
		}
	}
	_libererEl( &el );	
}

void graphe_destroy( Graphe *g ){
	element elASupprmer;
	for( size_t i = 0 ; i < TAILLE_TAB_TABLE_HACHAGE ; i++ ){
		for( listeChainee lst = (*g)->ensembleV->tabHachage[i] ; lst != NULL ; _libererNoeudListe( &lst ) ){
			elASupprmer = (element)( lst->el );
			_libererEl( &elASupprmer );
		}
	}
	free( (*g)->ensembleV );
	free(*g);
}

void graphe_indexsCasesAdjacents( Graphe g, const int numSommet, int * tabResultat[], size_t * tailleTabResultat ){
	size_t hachNum;
	element el;
	listeChainee caseITablehachage, lst;

	*tailleTabResultat = 0;

	hachNum = g->ensembleV->fonctionHachage( numSommet );

	caseITablehachage = g->ensembleV->tabHachage[hachNum];

	for( lst = caseITablehachage ; ((element)( lst->el ))->valeur != numSommet ; lst = lst->suivant );
	el = (element)( lst->el );

	if( el->ensembleE == NULL ) return;

	for( lst = el->ensembleE ; ; ){
		*tabResultat = realloc( *tabResultat, sizeof( int ) * ++(*tailleTabResultat) );
		(*tabResultat)[ *tailleTabResultat - 1 ] = ((element)( lst->el ))->valeur;
		if( (lst = lst->suivant) == el->ensembleE ) return;
	}
}

bool graphe_estVide( Graphe g ){
	for( size_t i = 0; i <TAILLE_TAB_TABLE_HACHAGE ; i++ ){
		if( g->ensembleV->tabHachage[i] != NULL )
			return false;
	}

	return true;
}
