/************************************************************
 * Module joueur                                            *
 *                                                          *
 *  --> Gestion du joueur                                   *
 *                                                          *
 * Sarrias Damien - Dernière modification : 14/05/2017      *
 *                                                          *
 * Groupe : Roche Nathan, Thibault Timothée                 *
 ***********************************************************/

#include "joueur.h"
#include "grille.h"
#include "groupe.h"

#define BORD_PAR_JOUEUR 2

typedef struct s_node{
	Groupe element;
	struct s_node* suivant;
	struct s_node* precedent;

}*Node;

struct s_Joueur {
	Node sentinelle;
	int nbGroupes;
	s_couleur couleurJoueur;
	bool IA;
};

void joueur_creer(Joueur* j, s_couleur couleur,bool estIA){
	if((*j=malloc(sizeof(struct s_Joueur)))==NULL)
		abort();
	if(((*j)->sentinelle=malloc(sizeof(struct s_node)))==NULL)
		abort();
	(*j)->nbGroupes=0;
	(*j)->couleurJoueur=couleur;
	(*j)->IA=estIA;

	(*j)->sentinelle->suivant=(*j)->sentinelle;
	(*j)->sentinelle->precedent=(*j)->sentinelle;

}

Groupe* joueur_getGroupesJoueur(Joueur j){
	Groupe* groupesJoueur=malloc(sizeof(Groupe)*j->nbGroupes);
	Node cur=j->sentinelle;

	for(int i=0;i<j->nbGroupes;i++){
		cur=cur->suivant;
		groupesJoueur[i]=cur->element;
	}

	return groupesJoueur;
}

int joueur_getNbGroupes(Joueur j){
	return j->nbGroupes;
}

s_couleur joueur_getCouleur(Joueur j){
	return j->couleurJoueur;
}

bool joueur_estIA(Joueur j){
	return j->IA;
}

void joueur_ajouterGroupe(Joueur* j, Groupe g){
  Node nodeAjout;
  Groupe groupeCopie;

  if((nodeAjout=malloc(sizeof(struct s_node)))==NULL)
    abort();

  nodeAjout->suivant=(*j)->sentinelle;
  nodeAjout->precedent=(*j)->sentinelle->precedent;

  groupeCopie=groupe_copie(g);
  nodeAjout->element=groupeCopie;

  (*j)->sentinelle->precedent->suivant=nodeAjout;
  (*j)->sentinelle->precedent=nodeAjout;
  (*j)->nbGroupes++;
}

void joueur_supprimerGroupe(Joueur* j, Groupe g){
	Node cur;
	for(cur=(*j)->sentinelle->suivant; (cur != (*j)->sentinelle) && (!groupe_comparer(cur->element,g)) ;cur=cur->suivant);
		if(cur!=(*j)->sentinelle){
			cur->precedent->suivant=cur->suivant;
			cur->suivant->precedent=cur->precedent;
			groupe_detruire(&cur->element);
			free(cur);
      (*j)->nbGroupes--;
		}
}

bool joueur_appartenirGroupe(Joueur j, Groupe g){
	for(Node cur=j->sentinelle->suivant; cur!=j->sentinelle; cur=cur->suivant){
		if(groupe_comparer(cur->element,g))
			return true;
	}
	return false;
}

void joueur_integrerCaseDansGroupe(Joueur* j, Case c, Case* adjacents){
	bool adjacentAppartientGroupe,memeCouleurJoueurCase;
	bool ajouteDansUnGroupeExistant=false;
	Node groupeJoueur,groupeTemoin,groupeATest;

	for(int i=0; i<NB_ADJACENTS_MAX_PAR_CASE && adjacents[i]!=NULL; i++){
		memeCouleurJoueurCase=(sommet_getCouleur(case_getSommet(adjacents[i]))==joueur_getCouleur(*j));

		if(memeCouleurJoueurCase){
			groupeJoueur=(*j)->sentinelle->suivant;
			
			for (int k=0; k<joueur_getNbGroupes(*j) ; k++) {
				adjacentAppartientGroupe=(groupe_appartenir(groupeJoueur->element,adjacents[i]));
				
				if(adjacentAppartientGroupe){
					groupe_ajouter(&groupeJoueur->element,c);
					ajouteDansUnGroupeExistant=true;
				}
				groupeJoueur=groupeJoueur->suivant;
			}
		}
	}
	if(!ajouteDansUnGroupeExistant){
		Groupe nouveauGroupe;
		groupe_initialiser(&nouveauGroupe);
		groupe_ajouter(&nouveauGroupe,c);
		joueur_ajouterGroupe(j,nouveauGroupe);
		groupe_detruire(&nouveauGroupe);
	}
	else{
		groupeTemoin=(*j)->sentinelle->suivant;
		for(int i=0; i<(*j)->nbGroupes;i++){
			
			if (groupe_appartenir(groupeTemoin->element,c)){
				groupeATest=groupeTemoin->suivant;
				
				for(int k=i+1; k<(*j)->nbGroupes;){
					if (groupe_appartenir(groupeATest->element,c)) {
						if(groupe_fusion(&groupeTemoin->element,groupeATest->element))
							joueur_supprimerGroupe(j,groupeATest->element);
					}
					else k++;
					groupeATest=groupeATest->suivant;
				}
			}
			groupeTemoin=groupeTemoin->suivant;
		}
	}

}

void joueur_recupererGroupesBords(Joueur *j, Grille grille){
		Groupe groupesBord[BORD_PAR_JOUEUR];
		Case bordsJoueur[BORD_PAR_JOUEUR];


		bordsJoueur[0]=(joueur_getCouleur(*j)==w) ? grille_caseX(grille,BORD_HAUT) : grille_caseX(grille,BORD_GAUCHE);
		bordsJoueur[1]=(joueur_getCouleur(*j)==w) ? grille_caseX(grille,BORD_BAS) : grille_caseX(grille,BORD_DROIT);

		for(int i=0; i<BORD_PAR_JOUEUR;i++){
			groupe_initialiser(&groupesBord[i]);
			groupe_ajouter(&groupesBord[i],bordsJoueur[i]);
			joueur_ajouterGroupe(j,groupesBord[i]);
			groupe_detruire(&groupesBord[i]);
		}
}

Joueur joueur_copie(const Joueur joueurSource){
	Joueur joueurCopie;
	joueur_creer(&joueurCopie,joueur_getCouleur(joueurSource),joueur_estIA(joueurSource));

	for(Node cur=joueurSource->sentinelle->suivant; cur != joueurSource->sentinelle; cur=cur->suivant){
		joueur_ajouterGroupe(&joueurCopie,cur->element);
	}

 	return joueurCopie;
}

 bool joueur_comparer(Joueur j1, Joueur j2){
 	if(joueur_getNbGroupes(j1)!=joueur_getNbGroupes(j2) 
 		|| joueur_getCouleur(j1)!=joueur_getCouleur(j2)
 		|| joueur_estIA(j1)!=joueur_estIA(j2))
 		return false;

 	Node cur2=j2->sentinelle->suivant;
 	for(Node cur=j1->sentinelle->suivant; cur!=j1->sentinelle;){
 		if(!groupe_comparer(cur->element,cur2->element))
 			return false;
 		cur=cur->suivant;
 		cur2=cur2->suivant;
 	}
 	return true;
 }

void joueur_detruire(Joueur* j){
	Node nodeAsupp;
	for(Node cur=(*j)->sentinelle->suivant; cur != (*j)->sentinelle;){
		nodeAsupp=cur;
		cur=cur->suivant;
		joueur_supprimerGroupe(j,nodeAsupp->element);
  	}
	free((*j)->sentinelle);
	free(*j);
}

