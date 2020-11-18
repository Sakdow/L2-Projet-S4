#ifndef PILE
#define PILE

#include <stdbool.h>
#include "case.h"

typedef struct s_Pile * Pile;

typedef Case Element;

/*
 * @brief initialise une Pile p vide
*/
void pile_initialiser(Pile *p);

/*
 * @brief empile l'Element el sur la Pile p
*/
void pile_empiler(Pile *p, Element el);

/*
 * @brief depile l'Element au sommet de la Pile p
 * @pre Pile non vide
*/
void pile_depiler(Pile *p);

/*
 * @brief retourne l'Element au sommet de la Pile p
 * @pre Pile non vide
 * @warning l'Element retourné doit être libéré via case_detruire() de "case.h"
*/
Element pile_sommet(Pile p);

/*
* @brief test si la Pile p est vide
* @return true si la Pile est vide sinon false
*/
bool pile_vide(Pile p);

/*
* @brief detruit la Pile p
* @return true si la Pile est vide sinon false
*/
void pile_detruire(Pile* p);

/*
* @brief cree une copie de la Pile pileSource
* @return la Pile copie de pileSource
*/
Pile pile_copie(Pile pileSource);

/*
* @brief test si les Pile p1 et p2 sont identiques
* @return true si identique sinon false
*/
bool pile_comparer(Pile p1, Pile p2);

#endif
