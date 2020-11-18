#ifndef MENU_
#define MENU_


#include <jni.h>
#include <stdbool.h>

#include "joueur.h"
#include "jeu.h"
#define TAILLE_NOM_SAUVEGARDE 20

/**
* @brief Demande au joueur avec une saisie, la case où il veut jouer (ligne colonne). 
* @brief La validité du coup est vérifié
*
* @retrurn une InfosJeu actualisé avec le coup placé
*/
void menu_jouerCoup(JNIEnv *env, InfosJeu *i,Joueur joueurActuel);

/**
* @brief Demande au joueur le nom de la sauvegarde à sauvegarder, de taille TAILLE_NOM_SAUVEGARDE
* @note L'existance ou non de ce nom dans le dossier de sauvegarde est vérifié.
*/
void menu_sauvegarder(JNIEnv *env, InfosJeu i);

/**
* @brief Demande au joueur le nom de la sauvegarde à charger, de taille TAILLE_NOM_SAUVEGARDE
* @note L'existance ou non de ce nom dans le dossier de sauvegarde est vérifié
*
* @return une InfosJeu actualisé avec la nouvelle sauvegarde.
* @return true si le chargement d'une sauvegarde à été effectué, false sinon
*/
bool menu_charger(JNIEnv *env, InfosJeu *i);

/**
* @brief Lance une nouvelle partie, avec sasie de la taille, du type de partie et du joueur qui commence
* 
* @return une InfosJeu initialisé.
* @waning InfosJeu ne doit pas être initialisé
*/
void menu_nouvellePartie(JNIEnv *env,InfosJeu *i);

/**
* @brief Demande au joueur le nom de la sauvegarde à supprimer, de taille TAILLE_NOM_SAUVEGARDE
* @note  L'existance ou non de ce nom dans le dossier de sauvegarde est vérifié
*/
void menu_supprimerSauvegarde(JNIEnv *env);


/**
* @brief Affiche les règles du jeu
*/
void menu_regles(JNIEnv *env);

/**
* @brief Abandonne une partie
* @brief Demande si le joueur veut sauvegarder
*/
void menu_abandonner(JNIEnv *env, InfosJeu i);

#endif