#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "jeu.h"
#include "sauvegarde.h"
#include "test.h"

#define NB_TEST 10

char* chaineAleatoire(int nbChar){
	char *chaine=malloc(sizeof(char)*(nbChar+1));
	int nombreMystere;
	for(int i=0;i<nbChar;i++){
		nombreMystere = (rand() % (122 - 97 + 1)) + 97;
		chaine[i]=(unsigned char)nombreMystere;
	}
	return chaine;
}

bool test_sauvegardeExistante_sauvegarde(void){
	bool retour=false;
	InfosJeu i;
	char* nom;
	int dim = (rand() % 21);
	jeu_creer(&i,dim,false,false);
	nom=chaineAleatoire(20);
	sauvegarde_sauvegarder(i,nom);
	if(sauvegarde_sauvegardeExistante(nom))
		retour=true;
	sauvegarde_supprimer(nom);
	free(nom);
	jeu_detruire(&i);
	return retour;
}


bool test_supprimer_sauvegarde(void){
	bool retour=true;
	InfosJeu i;
	char* nom;
	for(int k=0;k<NB_TEST;k++){
		int dim = (rand() % 21);
		nom=chaineAleatoire(20);
		jeu_creer(&i,dim,false,false);
		sauvegarde_sauvegarder(i,nom);
		sauvegarde_supprimer(nom);
		if(sauvegarde_sauvegardeExistante(nom))
			retour=false;
		free(nom);
		jeu_detruire(&i);
	}
	return retour;
}



bool test_sauvegarderEtCharger_sauvegarde(void){
	bool retour=true;
	InfosJeu i,i2;
	int numeroCase,nbCoup;
	unsigned ligne,colonne;
	char* nom;
	for(int k=0;k<NB_TEST;k++){
		int dim = (rand() % (20-2+1) + 2);
		jeu_creer(&i,dim,false,false);
		nbCoup=(rand()%(dim*dim));
		for(int m=0;m<nbCoup;m++){
			numeroCase=(rand()%(dim*dim));
			grille_numCaseToLigneColonne(numeroCase, &ligne, &colonne, dim );
			if(jeu_coupLegal(i,ligne,colonne)){
				jeu_jouerCoup(&i,jeu_quiDoitJouer(i),ligne,colonne);
			}
		}
		jeu_chargerGroupesEtNbCoups(&i);
		nom=chaineAleatoire(20);
		sauvegarde_sauvegarder(i,nom);
		if(!sauvegarde_sauvegardeExistante(nom))
			retour=false;
		else{
			i2=sauvegarde_chargerSauvegarde(nom);
			if(!jeu_comparer(i,i2))
				retour=false;
			sauvegarde_supprimer(nom);
			jeu_detruire(&i2);
		}
		jeu_detruire(&i);
		free(nom);
	}
	return retour;
}



int main(int argc, char const *argv[])
{
	srand(time(NULL));
	printf("TEST MODULE SAUVEGARDE :\n\n");
	resultat_test_fonction("test_sauvegardeExistante_sauvegarde",test_sauvegardeExistante_sauvegarde);
	resultat_test_fonction("test_supprimer_sauvegarde",test_supprimer_sauvegarde);
	resultat_test_fonction("test_sauvegarderEtCharger_sauvegarde",test_sauvegarderEtCharger_sauvegarde);

}