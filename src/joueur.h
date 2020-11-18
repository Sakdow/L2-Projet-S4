#ifndef JOUEUR
#define JOUEUR

#include <stdbool.h>
#include <stdlib.h>

#include "groupe.h"
#include "case.h"
#include "grille.h"

typedef struct s_Joueur * Joueur;

/*
* @brief creer un Joueur j, de s_couleur couleur, humain si estIA false sinon IA, avec une liste vide de Groupe
*/
void joueur_creer(Joueur* j, s_couleur couleur,bool estIA);

/*
*@brief retourne un tableau des Groupes du Joueur j
*@warning il faut liberer le tableau après utilisation via free() de <stdio.h>
*/
Groupe* joueur_getGroupesJoueur(Joueur j);

/*
*@brief retourne le nombre de groupes du joueur j
*/
int joueur_getNbGroupes(Joueur j);

/*
*@brief retourne la s_couleur du joueur j
*/
s_couleur joueur_getCouleur(Joueur j);

/*
*@brief teste si le joueur j est une IA
*@return true si c'est une IA sinon false
*/
bool joueur_estIA(Joueur j);

/*
*@brief ajoute le Groupe g au Joueur j
*/
void joueur_ajouterGroupe(Joueur* j, Groupe g);

/*
*@brief supprime le Groupe g du Joueur j
*/
void joueur_supprimerGroupe(Joueur* j, Groupe g);

/*
* @brief teste si le Groupe g appartient au Joueur j
* @return true si g appartient à j sinon false
*/
bool joueur_appartenirGroupe(Joueur j, Groupe g);

/*
*@brief integre la Case c à son bon groupe, dans les groupes du Joueur j, selon ses Case adjacents
*/
void joueur_integrerCaseDansGroupe(Joueur* j, Case c, Case* adjacents);

/*
*@brief le Joueur j recupere deux groupes composés de chacun des bords qui lui correspondent
*/
void joueur_recupererGroupesBords(Joueur *j, Grille grille);

/*
*@brief retourne un Joueur copie du Joueur joueurSource
*/
Joueur joueur_copie(const Joueur joueurSource);

/*
*@brief test si les Joueur j1 et j2 sont identiques
@return true si identique sinon false
*/
bool joueur_comparer(Joueur j1, Joueur j2);

/*
*@brief detruit le Joueur j
*/
void joueur_detruire(Joueur* j);

#endif
