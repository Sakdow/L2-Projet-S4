#ifndef SOMMET_H
#define SOMMET_H
#include <stdbool.h>
typedef enum {w, b, t} s_couleur;

typedef struct s_sommet *Sommet;


/**
* @brief cree une sommet initialisé.
*
* @return Sommet cree
*/

void sommet_creer(Sommet *sommet, s_couleur couleur);

/**
* @brief detruit un sommet
*/

void sommet_detruire(Sommet *sommet);


/**
* @brief Change la couleur du sommet;
*
* @return sommet avec la couleur passé en parametre
*/

void sommet_changerCouleur(Sommet *sommet, s_couleur couleur);


/**
* @brief Retourne la couleur du sommet
*/

s_couleur sommet_getCouleur(Sommet sommet);

/**
* @brief Creer une copie de la case sommetSource
*
* @return Case sommetCopie initialisé
* @pre sommetSource est initialisé
*/

void sommet_copie(Sommet *sommetCopie, const Sommet sommetSource);


/**
* @brief Compare deux cases
*
* @return true si sommet1 à la même couleur que sommet2, false sinon
* @pre sommet1 et sommet2 sont initialisé
*/

bool sommet_comparerCouleur(Sommet sommet1, Sommet sommet2);


#endif
