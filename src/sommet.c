/************************************************************
 * MODULE sommet                                            *
 *                                                          *
 *  --> Gestion du TAD sommet                               *
 *                                                          *
 * Roche Nathan - Dernière modification : 04/04/2017        *
 *                                                          *
 * Groupe : Damien Sarrias, Timothée Thibault               *
 ************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "sommet.h"

struct s_sommet{
	s_couleur couleur;
};


void sommet_creer(Sommet *sommet, s_couleur couleur){
	(*sommet)=malloc(sizeof(struct s_sommet));
	if ((*sommet) == NULL)
		abort();
	(*sommet)->couleur=couleur;
}

void sommet_detruire(Sommet *sommet){
	free((*sommet));
}

void sommet_changerCouleur(Sommet *sommet, s_couleur couleur){
	(*sommet)->couleur=couleur;
}

s_couleur sommet_getCouleur(Sommet sommet){
	return sommet->couleur;
}

void sommet_copie(Sommet *sommetCopie, const Sommet sommetSource){
	sommet_creer(sommetCopie,sommetSource->couleur);
}

bool sommet_comparerCouleur(Sommet sommet1, Sommet sommet2){
	return sommet1->couleur == sommet2->couleur;
}
