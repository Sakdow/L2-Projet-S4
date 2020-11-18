#ifndef GRAPHE_
#define GRAPHE_

#include "case.h"
#include <stdbool.h>
#include <stdlib.h>

typedef struct s_graphe * Graphe;

/**
* @brief cree un graphe vide
*
* @return Graphe cree, NULL en cas d'erreur
* @note le graphe g peut etre modifie dans la fonction
*/
Graphe graphe_create( Graphe * g );

/**
* @brief detruit un graphe
*
* @note le graphe g peut etre modifie dans la fonction
*/
void graphe_destroy( Graphe *g );

/** @brief renvoie si un graphe est vide
*
* @return \ctrue si le graphe est vide, \c sinon
* @pre le graaphe est créé
*/
bool graphe_estVide( Graphe g );

/**
* @brief permet d'ajouter une case
*
* @return 1 si la case a ete ajoute, 0 si une case de meme num existait deja,-1 si erreur
* @note le num est unique. La case ne sera ajoute que si aucune case du graphe n'a ce num
* @note le graphe g peut etre modifie dans la fonction
*/ 
int graphe_ajouterCase( Graphe *g, const int num, const Case s );

/**
* @brief permet de creer une arete entre deux case
*
* @return 1 si l'arete a ete ajoutee, 0 si elle existait deja, -1 en cas d'erreur
* @pre les deux cases existent dans le graphe et num1 est different de num2
* @note le graphe g peut etre modifie dans la fonction
*/
int graphe_ajouterArete( Graphe *g,const int num1, const int num2 );

/**
* @brief trouve la case correspondant au numero passe en parametre
*
* @return l'adresse de la case correspondant s'il a ete trouve, \cNULL sinon
*/
Case * graphe_caseNumeroI( Graphe g, const int num );

/**
* @brief cherche si une case est dans le graphe
*
* @return \ctrue si la case a ete trouve , \cfalse sinon
*/
bool graphe_caseTrouvee( Graphe g, const int num );

/**
* @brief supprime un Sommet ainsi que ses aretes
*
* @pre la case existe dans le graphe
* @note le graphe g peut etre modifie dans la fonction
*/
void graphe_supprimeCase( Graphe *g, const int num );

/** @brief cherche si deux cases sont adjacents
*
* @pre les deux cases existent dans le graphe et numS1 et numS2 sont differents
*return \ctrue si les cases sont adjacents , \cfalse sinon
*/
bool graphe_sontSommetsAdjactents( Graphe g, const int numS1, const int numS2 );

/** @brief renvoie un tableau contenant les indexs des cases adjacents au Sommet passé en paramètre
*
* @warnings le tableau devra avoir une place allouée dynamiquement en mémoire
*/
void graphe_indexsCasesAdjacents( Graphe g, const int numCase, int * tabResultat[], size_t * tailleTabResultat );


#endif
