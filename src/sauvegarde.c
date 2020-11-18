/************************************************************
 * MODULE sauvegarde                                        *
 *                                                          *
 *  --> Gestion de la sauvegarde (charger, sauvegarder,     *
 *      supprimer)                                          *
 *                                                          *
 * Roche Nathan - Dernière modification : 16/04/2017        *
 *                                                          *
 * Groupe : Damien Sarrias, Timothée Thibault               *
 ************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include "grille.h"
#include "pile.h"
#include "sommet.h"
#include "jeu.h"
#include "sauvegarde.h"
#include "case.h"

/**
* @brief Concatène deux chaines. 
*
* @return une chaine de charactère composé des deux chaines passé en paramètre
* @warning la chaine retourné doit être free
*/
static char * _concat(char * chaine1, char *chaine2);

/**
* @brief Sauvegarde la couleur du sommet dans un fichier
*/
static void _sauvegarde_sauvegarderCouleurJoueur(FILE *f,Sommet sommet);

/**
* @brief Charge la grille de jeu
*
* @return une grille de dimension dimGrille
*/

static void _sauvegarde_chargerGrille(FILE* f,int dimGrille, Grille *grille);

/**
* @brief Charge la pile de jeu
*/
static void _sauvegarde_chargerPile(FILE* f,int dimGrille, Pile *pile);




static char * _concat(char * chaine1, char *chaine2){
	size_t taille = strlen(chaine1);
	char * chaine = malloc( taille + strlen(chaine2) + 1 );
	memcpy(chaine,chaine1,taille + 1);
	strcat(chaine,chaine2);
	return chaine;
}


static void _sauvegarde_sauvegarderCouleurJoueur(FILE *f,Sommet sommet){
		switch(sommet_getCouleur(sommet)){
			case w:
				fprintf(f, "o ");
				break;
			case b:
				fprintf(f, "* ");
				break;
			case t:
				fprintf(f, ". ");
				break;
		}
}

static void _sauvegarde_chargerGrille(FILE* f,int dimGrille, Grille *grille){
	char ch[TAILLE_MAX_MOT];
	s_couleur couleur;
	int i=0;


	fscanf(f,"%s",ch);
	for(;strcmp(ch,"\\endboard") && i<dimGrille*dimGrille;){
		if(!strcmp(ch,"o"))
			couleur=w;
		else if(!strcmp(ch,"*"))
			couleur=b;
		else if(!strcmp(ch,"."))
			couleur=t;
		grille_modifierCouleur(grille,i,couleur);
		fscanf(f,"%s",ch);
		i++;
	}

}

static void _sauvegarde_chargerPile( FILE* f, int dimGrille, Pile *pile ){
	char ch[TAILLE_MAX_MOT];
	Case c;
	s_couleur couleur;
	Sommet s1;
	unsigned int ligne, colonne;


	for( ; strcmp( ch,"\\endgame" ) ; ){
		fscanf( f,"%s",ch );
		if( !strcmp( ch,"\\play" ) ){
			fscanf( f,"%s",ch );
			if( !strcmp(ch, "o" ) )
				couleur=w;
			else if( !strcmp( ch, "*" ) )
				couleur = b;
			else if( !strcmp(ch, "." ) )
				couleur = t;
			fscanf( f, "%d", &ligne );
			fscanf( f,"%d", &colonne );
			sommet_creer(&s1,couleur);
			case_creer(&c,grille_ligneColonneToNumCase( ligne,colonne,dimGrille ),s1 );
			pile_empiler( pile, c );
			sommet_detruire(&s1);
			case_detruire(&c);
		}
	}
}


bool sauvegarde_sauvegardeExistante( char* nomSauvegarde )
{
	DIR *rep;
	struct dirent* curfile;
	struct stat infos;
	char *repFile;
	char *nomSauvegardeExtension;

	nomSauvegardeExtension=_concat(nomSauvegarde,EXTENSION);
	if( (rep=opendir( DATA ) )==NULL ){
		perror( DATA );
		exit( 1 );
	}

	for(;( curfile=readdir( rep ) )!=NULL;){
		repFile=_concat( DATA,curfile->d_name );
		if( ( stat( repFile,&infos ) )==-1 )
			perror( curfile->d_name );
		if( !S_ISDIR( infos.st_mode ) ){
			if(!strcmp(curfile->d_name,nomSauvegardeExtension)){
				closedir(rep);
				free(nomSauvegardeExtension);
				free(repFile);
				return true;
			}
		}
		free( repFile );
	}

	closedir( rep );
	free( nomSauvegardeExtension );
	return false;
}


void sauvegarde_sauvegarder( InfosJeu jeu, char* nomSauvegarde )
{
	FILE *f;
	Case c;
	int numCase=0;
	char *nomSauvegardeComplet;
	char *nomSauvegardeExtension;
	unsigned int ligne,colonne;
	Sommet sommet;
	Grille grille=jeu_getGrille(jeu);
	Pile p=jeu_getPile(jeu);
	Pile pInverser;

	pile_initialiser(&pInverser);
	int dimGrille=grille_obtenirTaille(grille);
	nomSauvegardeExtension=_concat(nomSauvegarde,EXTENSION);
	nomSauvegardeComplet=_concat(DATA,nomSauvegardeExtension);
	if((f=fopen(nomSauvegardeComplet,"w+"))==NULL){
		perror(nomSauvegardeComplet);
		exit(1);
	}

	fprintf(f, "\\hex\n");
	fprintf(f, "\\dim %d\n", dimGrille);
	fprintf(f, "\\board\n");

	for (;numCase<dimGrille*dimGrille;numCase++){
		c=grille_caseX( grille, numCase );
		sommet=case_getSommet(c);
		_sauvegarde_sauvegarderCouleurJoueur(f,sommet);
		if(numCase%dimGrille==dimGrille-1){
			fprintf(f,"\n");
		}
	}

	fprintf(f, "\\endboard\n" );
	fprintf(f, "\\game\n" );

	for(;!pile_vide(p);){
		c=pile_sommet(p);
		pile_empiler(&pInverser,c);
		pile_depiler(&p);
		case_detruire(&c);
	}

	for(;!pile_vide(pInverser);){
		c=pile_sommet(pInverser);
		numCase=case_getNumeroCase(c);
		sommet=case_getSommet(c);
		fprintf(f, "\\play " );
		_sauvegarde_sauvegarderCouleurJoueur(f,sommet);
		grille_numCaseToLigneColonne(numCase,&ligne,&colonne,dimGrille);
		fprintf(f, "%d %d\n",ligne, colonne );
		pile_empiler(&p,c);
		case_detruire(&c);
		pile_depiler(&pInverser);
	}

	fprintf(f, "\\endgame\n" );
	fprintf(f, "\\endhex\n" );

	if(fclose(f)==EOF){
		perror(nomSauvegardeComplet);
		exit(2);
	}
	pile_detruire(&pInverser);
	free(nomSauvegardeExtension);
	free(nomSauvegardeComplet);
}




void sauvegarde_supprimer(char* nomSauvegarde)
{
	char *nomSauvegardeComplet;
	char *nomSauvegardeExtension;

	nomSauvegardeExtension=_concat(nomSauvegarde,EXTENSION);
	nomSauvegardeComplet=_concat(DATA,nomSauvegardeExtension);

    remove(nomSauvegardeComplet);
    free(nomSauvegardeExtension);
    free(nomSauvegardeComplet);
}



InfosJeu sauvegarde_chargerSauvegarde(char* nomSauvegarde)
{
	FILE *f;
	InfosJeu i;
	int dimGrille;
	char ch[TAILLE_MAX_MOT];
	Grille grille;
	Pile pile;
	char *nomSauvegardeComplet;
	char *nomSauvegardeExtension;

	nomSauvegardeExtension=_concat(nomSauvegarde,EXTENSION);
	nomSauvegardeComplet=_concat(DATA,nomSauvegardeExtension);

	if((f=fopen(nomSauvegardeComplet,"r"))==NULL){
		perror(nomSauvegardeComplet);
		exit(1);
	}
	fscanf(f,"%s",ch);

	for(;!strcmp(ch,"\\hex")&& !feof(f);){
		fscanf(f,"%s",ch);
	}

	for(;strcmp(ch,"\\endhex")&& !feof(f);){
		if(!strcmp(ch,"\\dim")){
			fscanf(f,"%d",&dimGrille);
			jeu_creer(&i,dimGrille,false,false);
			grille=jeu_getGrille(i);
			pile=jeu_getPile(i);
		}else if(!strcmp(ch,"\\board")){
			_sauvegarde_chargerGrille(f,dimGrille,&grille);
		}
		else if (!strcmp(ch,"\\game")){
			_sauvegarde_chargerPile(f,dimGrille,&pile);
		}
		fscanf(f,"%s",ch);
	}
	jeu_chargerGroupesEtNbCoups(&i);
	if(fclose(f)==EOF){
		perror(nomSauvegardeComplet);
		exit(2);
	}

	free(nomSauvegardeExtension);
	free(nomSauvegardeComplet);

	return i;
}