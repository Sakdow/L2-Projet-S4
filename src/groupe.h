#ifndef GROUPE
#define GROUPE
#include <stdbool.h>
#include "case.h"

typedef struct s_Groupe * Groupe;

/*
 * @brief initialise un groupe g vide
*/
void groupe_initialiser(Groupe *g);

/*
* @brief teste si le groupe g est vide
* @return true si le groupe est vide sinon false
*/
bool groupe_vide(Groupe g);

/*
* @brief donne la cardinalité du groupe
* @return un entier correspondant à la taille du groupe
*/
int groupe_getTaille(Groupe g);

/*
* @brief ajoute Case el dans le groupe g seulement si n'y appartient pas déjà
*/
void groupe_ajouter(Groupe *g, Case caseAjout);

/*
* @brief supprime la Case caseASupprimer du groupe g
* @pre Case el appartient au groupe g
*/
void groupe_supprimer(Groupe *g, Case caseASupprimer);


/*
* @brief teste si Case el est dans le groupe g
* @return true si el appartient à g sinon false
*/
 bool groupe_appartenir(Groupe g, Case el);

/*
* @brief ajoute les éléments de g1 et g2 dans g1 en évitant les doublons
* @return true si il y a eu une fusion, false sinon
*/
bool groupe_fusion(Groupe *g1, Groupe g2);

/*
* @brief crée une copie groupeCopie à partir de groupeSource
*/
Groupe groupe_copie(const Groupe groupeSource);
/*
* @brief detruit la groupe g
* @return true si le groupe est vide sinon false
*/
void groupe_detruire(Groupe* g);

/*
*@brief retourne un tableau des Case du Groupe g
*@warning il faut liberer le tableau après utilisation via free() de <stdio.h>
*/
Case* groupe_getTableauCases(Groupe g);

/*
*@brief test si le groupe g1 est identique au groupe g2
*@return true si identique sinon false
*/
bool groupe_comparer(Groupe g1, Groupe g2);

#endif