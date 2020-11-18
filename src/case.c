/************************************************************
 * MODULE case                                              *
 *                                                          *
 *  --> Gestion du TAD case                                 *
 *                                                          *
 * Roche Nathan - Dernière modification : 05/05/2017        *
 *                                                          *
 * Groupe : Damien Sarrias, Timothée Thibault               *
 ************************************************************/


#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include "case.h"
#include "sommet.h"


struct s_case{
	Sommet sommet;
	int numeroCase;
};

void case_creer(Case *case1, int numeroCase, Sommet sommet){
	Sommet s2;
	(*case1)=malloc(sizeof(struct s_case));
	if((*case1)==NULL)
		abort();
	sommet_copie(&s2,sommet);
	(*case1)->sommet=s2;
	(*case1)->numeroCase=numeroCase;
}

void case_detruire(Case *case1){
	sommet_detruire(&(*case1)->sommet);
	free((*case1));
}

void case_copie(Case *caseCopie, const Case caseSource){
	case_creer(caseCopie,caseSource->numeroCase,caseSource->sommet);
}

int case_getNumeroCase(Case case1){
	return case1->numeroCase;
}

Sommet case_getSommet(Case case1){
	return case1->sommet;
}

bool case_comparer(Case case1, Case case2){
	return sommet_comparerCouleur(case1->sommet,case2->sommet) && case1->numeroCase == case2->numeroCase;
}