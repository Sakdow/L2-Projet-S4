#ifndef SAUVEGARDE_
#define SAUVEGARDE_

#include <stdbool.h>
#include "jeu.h"

#define TAILLE_MAX_MOT 50
#define DATA "data/sauvegarde/"
#define EXTENSION ".txt"

/**
* @brief regarde si le nom de la sauvegarde existe dans le dossier
*
* @return true si le nom existe, false sinon
*/
bool sauvegarde_sauvegardeExistante(char* nomSauvegarde);

/**
* @brief sauvegarde la partie en cours dans un fichier texte
*/
void sauvegarde_sauvegarder(InfosJeu jeu, char* nomSauvegarde);

/**
* @brief charge une partie ayant était sauvegardé dans le dossier de sauvegarde
*
* @return toutes les information du jeu chargé.
*/
InfosJeu sauvegarde_chargerSauvegarde(char* nomSauvegarde);

/**
* @brief supprime un fichier de sauvegarde
*/
void sauvegarde_supprimer(char* nomSauvegarde);
#endif