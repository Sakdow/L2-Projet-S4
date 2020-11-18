/************************************************************
 * TAD groupe                                               *
 *                                                          *
 *  --> Gestion du TAD groupe                               *
 *                                                          *
 * Sarrias Damien - Dernière modification : 12/05/2017      *
 *                                                          *
 * Groupe : Roche Nathan, Thibault Timothée                 *
 ***********************************************************/

#include <stdbool.h>
#include <stdlib.h>
#include "groupe.h"
#include "case.h"

typedef struct s_cell{
	struct s_cell *suivant;
	struct s_cell *precedent;
	Case element;
}*Cell;

struct s_Groupe{
  Cell sentinelle;
  int tailleGroupe;
};

void groupe_initialiser(Groupe *g){
	if((*g=malloc(sizeof(struct s_Groupe)))==NULL)
		abort();
	(*g)->tailleGroupe=0;
	if(((*g)->sentinelle=malloc(sizeof(struct s_cell)))==NULL)
		abort();
	(*g)->sentinelle->suivant=(*g)->sentinelle;
	(*g)->sentinelle->precedent=(*g)->sentinelle;
	(*g)->sentinelle->element=NULL;
}

bool groupe_vide(Groupe g){
  return(g->tailleGroupe==0);
}

int groupe_getTaille(Groupe g){
  return(g->tailleGroupe);
}

void groupe_ajouter(Groupe *g, Case caseAjout){
 	Case caseCopie;
 	if(!groupe_appartenir((*g),caseAjout)){
	 	Cell cellAjout=malloc(sizeof(struct s_cell));
	 	cellAjout->suivant=(*g)->sentinelle;
	 	cellAjout->precedent=(*g)->sentinelle->precedent;
	 	case_copie(&caseCopie,caseAjout);
	 	cellAjout->element=caseCopie;
	 	(*g)->sentinelle->precedent->suivant=cellAjout;
	 	(*g)->sentinelle->precedent=cellAjout;
		(*g)->tailleGroupe++;
	}
}

void groupe_supprimer(Groupe *g, Case caseASupprimer){
	Cell cur;
	for(cur=(*g)->sentinelle->suivant; (!case_comparer(cur->element,caseASupprimer)) && (cur != (*g)->sentinelle) ;cur=cur->suivant);
	if(cur!=(*g)->sentinelle){
		cur->precedent->suivant=cur->suivant;
		cur->suivant->precedent=cur->precedent;
		case_detruire(&cur->element);
		(*g)->tailleGroupe--;
		free(cur);
	}
}

bool groupe_appartenir(Groupe g, Case el){
	for (Cell cur=g->sentinelle->suivant; cur!=g->sentinelle;cur=cur->suivant){
		if (case_comparer(cur->element,el))
	 		return true;
	}
   	return false;
}

bool groupe_fusion(Groupe *g1, Groupe g2){
	bool fusionner=false;
	for (Cell cur=g2->sentinelle->suivant; cur!=g2->sentinelle;cur=cur->suivant){
		if (!groupe_appartenir((*g1),cur->element)){
			groupe_ajouter(g1,cur->element);
			fusionner=true;
		}
	}
	return(fusionner);
}

Groupe groupe_copie(const Groupe groupeSource){
	Groupe groupeCopie;
	groupe_initialiser(&groupeCopie);

	for(Cell cur=groupeSource->sentinelle->suivant; cur != groupeSource->sentinelle; cur=cur->suivant){
		groupe_ajouter(&groupeCopie,cur->element);
	}
	return groupeCopie;
}

void groupe_detruire(Groupe* g){
	Cell cellAsupp;
	for(Cell cur=(*g)->sentinelle->suivant;cur != (*g)->sentinelle;){
		cellAsupp=cur;
		cur=cur->suivant;
		groupe_supprimer(g,cellAsupp->element);
  	}
	free((*g)->sentinelle);
	free(*g);
}

Case* groupe_getTableauCases(Groupe g){
	Case *tabCase=malloc(sizeof(Case)*g->tailleGroupe);
	Cell cur=g->sentinelle;
	int i = 0;
	for(cur = g->sentinelle->suivant ; cur != g->sentinelle ; cur = cur->suivant){
		tabCase[i++]=cur->element;
	}
	return tabCase;
}

bool groupe_comparer(Groupe g1, Groupe g2){
	if(groupe_getTaille(g1)!=groupe_getTaille(g2))
		return false;

	Case* tabCase2=groupe_getTableauCases(g2);

	for(int i=0; i<groupe_getTaille(g2);i++)
		if(!groupe_appartenir(g1,tabCase2[i])){
			free(tabCase2);
			return false;
		}
	free(tabCase2);
	return true;
}
