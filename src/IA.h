#ifndef IA
#define IA
#include "jeu.h"
#include "joueur.h"

#define NB_COUPS_SIMULES 4

typedef struct s_simulationCoups * simulationCoups;

/** @brief crée une simulation du jeu
*
*/
void IA_creerSimulation( simulationCoups * sc, InfosJeu jeu, Joueur joueurIA );

/** @brief libère la place mémoire allouée à une simulation
*
*/
void IA_detuireSimulation( simulationCoups *sc );

/** @brief met à jour la simulation et retourne la case où l'IA veut jouer
*
*/
unsigned IA_jouerCase( simulationCoups * sc ); 

#endif
