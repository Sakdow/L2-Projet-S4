#ifndef GRILLE_
#define GRILLE_
#include "sommet.h"
#include "case.h"
#include "graphe.h"

typedef struct s_grille *Grille;

#define DEFAUT 0
#define TAILLE_QUAND_DEFAUT 11
#define TAILLE_MIN 2
#define TAILLE_MAX 20

/** @brief permet de creer une grille de taille \ctaille
*
* @return Grille creee, NULL en cas d'erreur
* @note la grille gr peut etre modifie dans la fonction
* @note si la taille est egale a defaut ou n'est pas comprise entre \cTAILLE_MAX et \cTAILLE_MIN, la grille sera creee de taille \cTAILLE_QUAND_DEFAUT
* @note les cases seront de s_couleur ( défini dans sommet.h ) t. Les bords haut et bas sont de couleurs w et les bords gauche et droite de couleur b
*/
Grille grille_creer( Grille *gr, unsigned taille );

/** @brief permet de copier une grille
*
* @pre gr est initialisée
*/
void grille_copie( Grille * grCopie, const Grille gr );

/** @brief supprime une grille
*
* @pre la grille est initialisee
*/
void grille_supprimer( Grille *gr );

void grille_numCaseToLigneColonne( const unsigned numCase, unsigned * l, unsigned *c, const unsigned tailleGrille );

unsigned grille_ligneColonneToNumCase( unsigned l, unsigned c , unsigned tailleGrille );

#define BORD_HAUT -1
#define BORD_GAUCHE -2
#define BORD_BAS -3
#define BORD_DROIT -4
/** @brief permet d'obtenir la case correspondant au numero x de la grille
*
* @pre x est inferieur à la taille de la grille au carre, et la grille est initialisee
*/
Case grille_caseX( Grille gr, int x );

/** @brief permet de modifier la couleur d'une case numero x de la grille
*
* @pre x est inferieur à la taille de la grille au carre, et la grille est initialisee
* @note la grille gr peut etre modifie dans la fonction
*/
void grille_modifierCouleur( Grille *gr, const int x, s_couleur couleur );

/** @brief permet d'obtenir la taille de la grille
*
* @pre la grille est creee
*/
unsigned grille_obtenirTaille( Grille gr );

#define NB_ADJACENTS_MAX_PAR_CASE 6

/** @brief renvoie un tableau (de taille 6) contenant les sommets(hexagones) adjacents à la case du numero de case passée en paramètre
*
* @pre la grille est initialisée, et la case existe dans la grille
* @warnings ne pas utiliser sur les bords de la grille
*/
void grille_caseAdjacentes( Grille gr, const unsigned numCase, Case tabSommetsAdj[NB_ADJACENTS_MAX_PAR_CASE] );

#define CARACTERE_POUR_COULEUR_W 'o'
#define CARACTERE_POUR_COULEUR_B '*'
#define CARACTERE_POUR_COULEUR_T '.'

/** @brief renvoie une chaine de caractère représentant la grille
*
* @note la chaîne de caractère ne contient pas les bords de la grille
* @warning la chaine de caractère ne doit pas être initialisée, et doit être liberée avec la fnction free() de <stdlib.h> sous peine de fuites mémoire
*/
void grille_toString( Grille grille, char ** chaine );

/** @brief permet de comparer deux grilles
*
* @return 0 si elle sont identiques, 1 si les tailles sont différentes, 2 si les cases sont de couleurs différentes
*/
int grille_comparer( const Grille g1, const Grille g2 );

#endif
