/************************************************************
 * TAD Pile                                                 *
 *                                                          *
 *  --> Gestion du TAD Pile                                 *
 *                                                          *
 * Sarrias Damien - Dernière modification : 16/04/2017      *
 *                                                          *
 * Groupe : Roche Nathan, Thibault Timothée                 *
 ***********************************************************/

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include "pile.h"
#include <stdio.h>


typedef struct s_cell {
	Element elem;
	struct s_cell* previous;
}*cell;

struct s_Pile{
	cell head;
};

void pile_initialiser(Pile *p){
	if((*p=malloc(sizeof(struct s_Pile)))==NULL)
		abort();
	(*p)->head=NULL;
}


void pile_empiler(Pile *p, Element el){
	Case copie;
	case_copie(&copie,el);
	cell newCell;
	newCell=malloc(sizeof(struct s_cell));
	newCell->elem=copie;
	newCell->previous=(*p)->head;
	(*p)->head=newCell;
}


void pile_depiler(Pile *p){
	assert(!pile_vide(*p));
	cell aSupprimmer=(*p)->head;
	(*p)->head=aSupprimmer->previous;
	case_detruire(&aSupprimmer->elem);
	free(aSupprimmer);
}

bool pile_vide(Pile p){
	return (p->head==NULL);
}

Element pile_sommet(Pile p){
	assert(!pile_vide(p));
	Case copie;
	case_copie(&copie,p->head->elem);
	return(copie);
}

Pile pile_copie(Pile pileSource){
	Pile pileCopie,pileSwap;
	Element el;

	pile_initialiser(&pileCopie);
	pile_initialiser(&pileSwap);

	for(;!pile_vide(pileSource);){
		el=pile_sommet(pileSource);
		pile_empiler(&pileSwap,el);
		pile_depiler(&pileSource);
		case_detruire(&el);
	}

	for(;!pile_vide(pileSwap);){
		el=pile_sommet(pileSwap);

		pile_empiler(&pileCopie,el);
		pile_empiler(&pileSource,el);

		case_detruire(&el);
		pile_depiler(&pileSwap);
	}
	pile_detruire(&pileSwap);

	return pileCopie;
}

bool pile_comparer(Pile p1, Pile p2){
	cell cur2=p2->head;
	for(cell cur1=p1->head;cur1!=NULL;cur1=cur1->previous){
		if (cur2==NULL)
			return false;
		if(!case_comparer(cur1->elem,cur2->elem))
			return false;

		cur2=cur2->previous;
	}
	if (cur2!=NULL)
		return false;

	return true;
}

void pile_detruire(Pile* p){
	while(!pile_vide(*p)){
		pile_depiler(p);
	}
	free(*p);
}
