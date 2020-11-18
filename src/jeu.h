#ifndef JEU
#define JEU

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "grille.h"
#include "pile.h"
#include "groupe.h"
#include "case.h"
#include "joueur.h"

#define NB_JOUEUR 2

typedef struct s_InfosJeu * InfosJeu;

/*
*@brief creer une InfosJeu i dont la dimension de la grille est taille, 
*		avec les joueurs 1 et 2 des IA selon respectivement prmeierJoueurIA et secondJoueurIA
*/
void jeu_creer(InfosJeu* i, unsigned taille, bool premierJoueurEstIA, bool secondJoueurEstIA);

/*
*@brief retourne le nombre de coups joues dans InfosJeu
*/
int jeu_getNbCoups(InfosJeu i);

/*
*@brief retourne le nombre de tours joues dans InfosJeu
*/
int jeu_getNbTours(InfosJeu i);

/*
*@brief retourne la Grille presente dans InfosJeu
*/
Grille jeu_getGrille(InfosJeu i);

/*
*@brief retourne la Pile de coups joues presente dans InfosJeu
*/
Pile jeu_getPile(InfosJeu i);

/*
*@brief retourne le premier Joueur
*/
Joueur jeu_getJoueur1(InfosJeu i);

/*
*@brief retourne le second Joueur
*/
Joueur jeu_getJoueur2(InfosJeu i);

/*
*@brief retourne le Joueur qui debute la partie
*/
Joueur jeu_getJoueurQuiCommence( InfosJeu jeu );

/*
*@brief initialise le Joueur qui debute la partie
*/
void jeu_setJoueurQuiCommence( InfosJeu * jeu, Joueur j );

/*
*@brief retourne le Joueur qui doit jouer
*/
Joueur jeu_quiDoitJouer(InfosJeu i);

/*
*@brief le Joueur j joue un coup, ligne x, colonne y dans l'InfosJeu i
*/
void jeu_jouerCoup(InfosJeu* i, Joueur j,unsigned x, unsigned y);

/*
*@brief teste si le coup, ligne x, colonne y dans InfosJeu i est legal
*@return true si legal sinon false
*/
bool jeu_coupLegal(InfosJeu i, int x, int y);

/*
*@brief teste si le Joueur j a gagne dans InfosJeu i
*@return true si victoire sinon false
*/
bool jeu_testVictoire(InfosJeu i, Joueur j);

/*
*@brief annule le denrier coup joué dans l'InfosJeu i 
*		et retablit InfosJeu i à la version precedent le dernier coup
*/
void jeu_annulerDernierCoup(InfosJeu *i);

/*
*@brief reinitialise puis recalcule les groupes des joueurs et le nombre de coups joues selon InfosJeu i
*/
void jeu_chargerGroupesEtNbCoups(InfosJeu *i);

/*
*@brief retourne sous la forme d'un String la chaine "couleur ligne colonne", le dernier coup joue
*		avec couleur une s_couleur et ligne colonne des entiers
*/
void jeu_toStringDernierCoup(InfosJeu i, char** chaine);

/*
*@brief retourne une InfosJeu, copie de InfosJeu iSource
*/
InfosJeu jeu_copie(InfosJeu iSource);

/*
*@brief teste si deux InfosJeu i1 et i2 sont identiques
*@return true si identique sinon false
*/
bool jeu_comparer(InfosJeu i1, InfosJeu i2);

/*
*@brief detruit l'InfosJeu i
*/
void jeu_detruire(InfosJeu* i);

#endif
