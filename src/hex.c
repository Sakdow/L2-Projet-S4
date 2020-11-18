/************************************************************
 * Jeu hex                                                  *
 *                                                          *
 *  --> Source principal du projet, gestion du jeu en       *
 *      entier                                              *
 *                                                          *
 * Roche Nathan - Dernière modification : 21/05/2017        *
 *                                                          *
 * Groupe : Damien Sarrias, Timothée Thibault               *
 ************************************************************/

#include <stdlib.h>
#include "menu.h"
#include "grille.h"
#include "IA.h"
#include "affichage.h"
#include "joueur.h"
#include "jeu.h"



void afficherInformation(InfosJeu i, JNIEnv *env){
	Grille grille=jeu_getGrille(i);
	int dim=grille_obtenirTaille(grille);
	char tourCoup[3];
	char *chaine;
	


	system("clear");
	grille_toString(grille,&chaine );
	affichage_afficherGrille(env,chaine,dim);
	free(chaine);

	affichage_afficherMessage(env,"\n\nDernier coup joué : ");
	if(!pile_vide(jeu_getPile(i))){
		jeu_toStringDernierCoup(i, &chaine);
		affichage_afficherMessage(env,chaine);
		free(chaine);
	}

	sprintf(tourCoup,"%d",jeu_getNbTours(i));
	affichage_afficherMessage(env,"\n\nTour : " );
	affichage_afficherMessage(env,tourCoup);

	sprintf(tourCoup,"%d",jeu_getNbCoups(i));
	affichage_afficherMessage(env,"\ncoup : ");
	affichage_afficherMessage(env,tourCoup );
}


void enJeu(JNIEnv *env, InfosJeu *i){
	Joueur joueurActuel;
	int choix;
	InfosJeu iCharger;
	simulationCoups sc1, sc2;
	unsigned numCaseIADoitJouer;

	if( joueur_estIA( jeu_getJoueur1(*i) ) )
		IA_creerSimulation( &sc1, (*i), jeu_getJoueur1(*i) );
	if( joueur_estIA( jeu_getJoueur2(*i) ) )
		IA_creerSimulation( &sc2, (*i), jeu_getJoueur2(*i) );
	
	for( ;!jeu_testVictoire((*i),jeu_getJoueur1((*i))) && !jeu_testVictoire((*i),jeu_getJoueur2((*i))); ){

		joueurActuel=jeu_quiDoitJouer((*i));
		afficherInformation((*i),env);
		if( joueur_estIA( joueurActuel ) ){
			unsigned ligne, colonne;
			numCaseIADoitJouer = ( joueurActuel == jeu_getJoueur1(*i) ) ? IA_jouerCase( &sc1 ) : IA_jouerCase( &sc2 );
			grille_numCaseToLigneColonne( numCaseIADoitJouer, &ligne, &colonne, grille_obtenirTaille( jeu_getGrille( *i ) ) );

			jeu_jouerCoup( i, joueurActuel, ligne, colonne );


		} else {


			do{

				affichage_afficherMessage(env,"\n\nMENU\n\n 1- Jouer \n 2- Annulation coup \n 3- Nouvelle partie \n 4- Sauvegarder \n 5- Charger \n 6- Abandonner \n\n\n");
				affichage_afficherMessage(env,"Veuillez choisir une action : ");
				scanf("%d",&choix);
			}while(choix<1 && choix>6);


			switch( choix ){
				case 1:
					menu_jouerCoup( env, i , joueurActuel );
					break;
				case 2:
					jeu_annulerDernierCoup(i);
					if(joueur_estIA(jeu_getJoueur1((*i))) || joueur_estIA(jeu_getJoueur2((*i))))
						jeu_annulerDernierCoup(i);
					break;
				case 3:
					jeu_detruire(i);
					menu_nouvellePartie(env,i);
					if( joueur_estIA( jeu_getJoueur1(*i) ) )
						IA_creerSimulation( &sc1, (*i), jeu_getJoueur1(*i) );
					if( joueur_estIA( jeu_getJoueur2(*i) ) )
						IA_creerSimulation( &sc2, (*i), jeu_getJoueur2(*i) );
					break;
				case 4:
					menu_sauvegarder(env,(*i));
					break;
				case 5:
					if(menu_charger(env,&iCharger)){
						jeu_detruire(i);
						(*i)=jeu_copie(iCharger);
						jeu_detruire(&iCharger);
					}
					break;
				default:
					menu_abandonner(env,(*i));
					jeu_detruire(i);
					return ;
					break;
			}

		}

	}
	afficherInformation(*i,env);
	if(jeu_testVictoire(*i,jeu_getJoueur1(*i))){
		affichage_afficherMessage(env,"\nC'est le joueur o qui gagne !\n");
	} else {
		affichage_afficherMessage(env,"\nC'est le joueur * qui gagne !\n");
	}

	return ;
}

int horsJeu(JNIEnv *env, InfosJeu *i){
	int choix;
	do{
		affichage_afficherMessage(env,"\n\nMENU\n\n 1- Nouvelle partie \n 2- Charger partie \n 3- Supprimer sauvegarde \n 4- Règles \n 5- Quitter\n\n\n");
		affichage_afficherMessage(env,"Veuillez choisir une action : ");
		scanf("%d",&choix);
	}while(choix<1 && choix>5);

	switch( choix ){
		case 1:
			menu_nouvellePartie (env, i);
			return 1;
			break;
		case 2:
			if(menu_charger(env,i))
				return 1;
			else
				return 2;
			break;
		case 3:
			menu_supprimerSauvegarde(env);
			return 2;
			break;
		case 4:
			menu_regles(env);
			return 2;
			break;
		default:
			return 0;
			break;
	}
}

int main(int argc, char const *argv[])
{
	JavaVM *jvm;
    JNIEnv *env;
    InfosJeu i;
    int retour;
    env = create_vm(&jvm);
    if(env == NULL)
		return 1;

    do{
    	retour=horsJeu(env,&i);
    	if(retour==1)
    		enJeu(env,&i);
    }while(retour);
	return 0;
}
