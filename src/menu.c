/************************************************************
 * MODULE menu                                              *
 *                                                          *
 *  --> Gestion des tous les menus du jeu                   *
 *                                                          *
 * Roche Nathan - Dernière modification : 21/05/2017        *
 *                                                          *
 * Groupe : Damien Sarrias, Timothée Thibault               *
 ************************************************************/



#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "grille.h"
#include "sauvegarde.h"
#include "menu.h"
#include "jeu.h"
#include "joueur.h"
#include "affichage.h"


/**
* @brief Vide le buffer 
*/
static void _vider_buffer();

/**
* @brief Saisie de caractère, o pour oui et n pour non
*
* @return renvoie true si o, false sinon 
*/
static bool _menu_confirmation();

/**
* @brief Saisie d'une chaîne de caractères, de taille TAILLE_NOM_SAUVEGARDE
*
* @return Une chaine de caractère statique
*/
static void _menu_saisirNom(char *nomSauvegarde);





static void _vider_buffer(){
	int c=0;

	for(;c!='\n' && c!= EOF;)
		c=getchar();
}



static bool _menu_confirmation(){
	char confirmation;

	do{
		confirmation=getchar();
	}while (confirmation!='o' && confirmation!='n');

	if(confirmation=='o')
		return true;
	return false;
}




static void _menu_saisirNom(char *nomSauvegarde){
	char *positionEntree;

	if (fgets(nomSauvegarde, TAILLE_NOM_SAUVEGARDE , stdin) != NULL){
        positionEntree = strchr(nomSauvegarde, '\n');
        if (positionEntree != NULL)
            *positionEntree = '\0';
    }
}



void menu_jouerCoup(JNIEnv *env, InfosJeu *i,Joueur joueurActuel){
	char c;
	char chaineJ[2];
	int ligne,colonne;

	if(joueur_getCouleur(joueurActuel)==w)
		c='o';
	else
		c='*';

	do{
		chaineJ[0] = c;
		chaineJ[1] = '\0';
		affichage_afficherMessage(env,"C'est au joueur " );
		affichage_afficherMessage(env,chaineJ );
		affichage_afficherMessage(env, " !\nOù voulez-vous placer votre pion (ligne colonne) : " );
		scanf("%u",&ligne);
		scanf("%u",&colonne);
	}while(!jeu_coupLegal((*i),ligne,colonne));


	printf("\n");
	jeu_jouerCoup(i,joueurActuel,ligne,colonne);
}

void menu_sauvegarder(JNIEnv *env, InfosJeu i){
	char nomSauvegarde[TAILLE_NOM_SAUVEGARDE];
	_vider_buffer();
	affichage_afficherMessage(env,"Veuillez saisir le nom de la sauvegarde : " );
	_menu_saisirNom(nomSauvegarde);

	if(nomSauvegarde==NULL)
		return ;
	if(sauvegarde_sauvegardeExistante(nomSauvegarde)){
	affichage_afficherMessage(env,"La sauvegarde existe deja, voulez-vous l'ecraser ? (o/n) ");
		if(!_menu_confirmation())
			return ;
	}
	sauvegarde_sauvegarder(i,nomSauvegarde);
}




bool menu_charger(JNIEnv *env, InfosJeu *i){
	char nomSauvegarde[TAILLE_NOM_SAUVEGARDE];

	affichage_afficherMessage(env,"Veuillez saisir le nom de la sauvegarde à charger: " );
	_vider_buffer();
	_menu_saisirNom(nomSauvegarde);

	if(nomSauvegarde==NULL)
		return false;


	if(!sauvegarde_sauvegardeExistante(nomSauvegarde)){

		affichage_afficherMessage(env,"La sauvegarde n'existe pas, voulez-vous réessayer ? (o/n) ");
		if(_menu_confirmation()){
			return menu_charger(env,i);
		}
		else
			return false;

	}else{
		(*i)=sauvegarde_chargerSauvegarde(nomSauvegarde);
	}


	return true;
}




void menu_nouvellePartie(JNIEnv *env,InfosJeu *i){
	int dim,choix;
	char JoueurQuiCommence;
	do{
		affichage_afficherMessage(env,"Veuillez choisir la dimension du la grille (min 2, max 20) : ");
		scanf("%d",&dim);
	}while(dim<2 || dim>20);


	affichage_afficherMessage(env,"Voulez-vous :\n 1- Jouer contre un joueur\n 2- Jouer contre un ordinateur\n 3- Regarder une partie ordinateur contre ordinateur\n");
	do{
		affichage_afficherMessage(env,"Veuillez choisir une action : ");
		scanf("%d",&choix);
	}while(choix<1 || choix>3);


	switch(choix){
		case 1:
			jeu_creer(i,dim,false,false);
			break;
		case 2:
			jeu_creer(i,dim,false,true);
			break;
		default:
			jeu_creer(i,dim,true,true);
			break;
	}

	do{
		_vider_buffer();
		affichage_afficherMessage(env,"Veuillez choisir le joueur qui commence (w/b) : ");
		JoueurQuiCommence=getc(stdin);
	}while(JoueurQuiCommence!='w' && JoueurQuiCommence!='b');


	if(JoueurQuiCommence=='w'){
		jeu_setJoueurQuiCommence(i,jeu_getJoueur1((*i)));
	}else{
		jeu_setJoueurQuiCommence(i,jeu_getJoueur2((*i)));
	}
}



void menu_supprimerSauvegarde(JNIEnv *env){
	char nomSauvegarde[TAILLE_NOM_SAUVEGARDE];

	affichage_afficherMessage(env,"Veuillez saisir le nom de la sauvegarde à supprimer : " );
	_vider_buffer();
	_menu_saisirNom(nomSauvegarde);


	if(nomSauvegarde==NULL)
		return ;


	if(!sauvegarde_sauvegardeExistante(nomSauvegarde)){
		affichage_afficherMessage(env,"La sauvegarde n'existe pas, voulez-vous réessayer ? (o/n) ");
		if(_menu_confirmation())
			menu_supprimerSauvegarde(env);
	}else{
		sauvegarde_supprimer(nomSauvegarde);
	}
}


void menu_regles(JNIEnv *env){
	affichage_afficherMessage(env,"\n- Les joueurs jouent chacun leur tour\n");
	affichage_afficherMessage(env,"- À chaque tour, le joueur place un pion de sa couleur sur une case libre\n");
	affichage_afficherMessage(env,"- Le premier joueur qui réussit à relier ses deux bords par un chemin de pions contigus de sa couleur a gagné.\n");
	affichage_afficherMessage(env,"\n\n\nRevenir au menu principal ? (o/n) ");

	if(_menu_confirmation()){
		return ;
	}
	menu_regles(env);
}




void menu_abandonner(JNIEnv *env, InfosJeu i){
	affichage_afficherMessage(env,"Voulez-vous sauvegarder avant d'abandonner ? (o/n) ");
	if(_menu_confirmation())
		menu_sauvegarder(env,i);

}
