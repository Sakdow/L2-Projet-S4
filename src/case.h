#ifndef CASE_H
#define CASE_H
#include "sommet.h"

typedef struct s_case *Case;

/**
* @brief cree une case initialisé.
*
* @return Case cree
*/

void case_creer(Case *case1, int numeroCase, Sommet sommet);

/**
* @brief detruit une case
*/

void case_detruire(Case *case1);

/**
* @brief Retourne le numero d'une case
*/
int case_getNumeroCase(Case case1);


/**
* @brief Creer une copie de la case caseSource
*
* @return Case caseCopie initialisé
* @pre caseSource est initialisé
*/
void case_copie(Case *caseCopie, const Case caseSource);


/**
* @brief Retourne le sommet d'une case
*
* @return sommet de la case case1
*/
Sommet case_getSommet(Case case1);

/**
* @brief Compare deux cases
*
* @return true si case1 à le même numero et la même couleur que case2, false sinon
* @pre case1 et case2 sont initialisé
*/

bool case_comparer(Case case1, Case case2);
#endif