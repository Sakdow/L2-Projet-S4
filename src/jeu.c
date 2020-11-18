/************************************************************
 * Module jeu                                               *
 *                                                          *
 *  --> Gestion du jeu                                      *
 *                                                          *
 * Sarrias Damien - Dernière modification : 15/05/2017      *
 *                                                          *
 * Groupe : Roche Nathan, Thibault Timothée                 *
 ***********************************************************/


#include "jeu.h"
#include "grille.h"
#include "pile.h"
#include "sommet.h"
#include "case.h"
#include "joueur.h"

#include <assert.h>
#include <stdlib.h>

struct s_InfosJeu{
    int nbCoups;
    Grille grilleJeu;
    Pile coupsJoues;
    Joueur joueurs[NB_JOUEUR];
    Joueur joueurQuiCommence;
};

static void _chargerGroupesJoueur( InfosJeu *i, Joueur j, int *nbCoupsAjout );

void jeu_creer(InfosJeu* i, unsigned taille, bool premierJoueurEstIA, bool secondJoueurEstIA){
    Pile p;
    Grille g;
    Joueur j1,j2;

    if((*i=malloc(sizeof(struct s_InfosJeu)))==NULL)
        abort();
    grille_creer(&g, taille);
    pile_initialiser(&p);

    (*i)->grilleJeu=g;
    (*i)->coupsJoues=p;
    joueur_creer(&j1,w,premierJoueurEstIA);
    joueur_creer(&j2,b,secondJoueurEstIA);
    (*i)->joueurs[0]=j1;
    (*i)->joueurs[1]=j2;

    (*i)->joueurQuiCommence=jeu_getJoueur1(*i);

    jeu_chargerGroupesEtNbCoups(i);
}

int jeu_getNbCoups(InfosJeu i){
    return (i->nbCoups);
}

int jeu_getNbTours(InfosJeu i){
    return ((i->nbCoups/2)+1);
}

Grille jeu_getGrille(InfosJeu i){
    return (i->grilleJeu);
}

Pile jeu_getPile(InfosJeu i){
    return (i->coupsJoues);
}

Joueur jeu_getJoueur1(InfosJeu i){
    return ((i)->joueurs[0]);
}

Joueur jeu_getJoueur2(InfosJeu i){
    return ((i)->joueurs[1]);
}

Joueur jeu_getJoueurQuiCommence( InfosJeu jeu ){
    return jeu->joueurQuiCommence;
}

void jeu_setJoueurQuiCommence( InfosJeu * jeu, Joueur j ){
    (*jeu)->joueurQuiCommence = j;
}

Joueur jeu_quiDoitJouer(InfosJeu i){
    Pile p=jeu_getPile(i);
    if( !pile_vide( p ) ){
        Case dernierCoup=pile_sommet(p);
        Joueur joueurQuiDoitJouer=(sommet_getCouleur(case_getSommet(dernierCoup))==w) ? jeu_getJoueur2(i) :jeu_getJoueur1(i);
        case_detruire(&dernierCoup);
        return joueurQuiDoitJouer;
    }
    return i->joueurQuiCommence;
}

void jeu_jouerCoup(InfosJeu* i, Joueur j,unsigned x, unsigned y){
    Case adjacentsCaseAModifier[NB_ADJACENTS_MAX_PAR_CASE];
    int numCase=grille_ligneColonneToNumCase(x,y,grille_obtenirTaille(jeu_getGrille(*i)));
    Case caseAModifier=grille_caseX(jeu_getGrille(*i),numCase);
    Sommet sommetAModifier=case_getSommet(caseAModifier);
    Pile p=jeu_getPile(*i);

    sommet_changerCouleur(&sommetAModifier,joueur_getCouleur(j));
    grille_caseAdjacentes(jeu_getGrille(*i),numCase,adjacentsCaseAModifier);
    joueur_integrerCaseDansGroupe(&j,caseAModifier,adjacentsCaseAModifier);

    pile_empiler(&p,caseAModifier);
    (*i)->nbCoups++;
}

bool jeu_coupLegal(InfosJeu i, int x, int y){
    Grille grille=jeu_getGrille(i);
    unsigned tailleGrille = grille_obtenirTaille(grille);
    int numCase=grille_ligneColonneToNumCase(x,y,tailleGrille);


    return (x>=0 && x>=0 && x<tailleGrille
            && y>=0 && y<tailleGrille
            && sommet_getCouleur(case_getSommet(grille_caseX(grille, numCase)))==t);
}

bool jeu_testVictoire(InfosJeu i, Joueur j){
    Grille grille=jeu_getGrille(i);
    Groupe* groupesJoueur=joueur_getGroupesJoueur(j);

    Case premierBord=(joueur_getCouleur(j)==w) ? grille_caseX(grille,BORD_HAUT) : grille_caseX(grille,BORD_GAUCHE);
    Case secondBord=(joueur_getCouleur(j)==w) ? grille_caseX(grille,BORD_BAS) : grille_caseX(grille,BORD_DROIT);

    for (int i = 0; i < joueur_getNbGroupes(j); i++)
        if(groupe_appartenir(groupesJoueur[i],premierBord) && groupe_appartenir(groupesJoueur[i],secondBord))
            return true;
    free(groupesJoueur);
    return false;
}

void jeu_annulerDernierCoup(InfosJeu *i){
    Grille grilleJeu=jeu_getGrille(*i);
    Pile pileCoupsJoues=jeu_getPile(*i);
    int nbGroupe;
    if(!pile_vide(pileCoupsJoues)){
        Case dernierCoup=pile_sommet(pileCoupsJoues);
        pile_depiler(&pileCoupsJoues);
        Case caseAModifier=grille_caseX(grilleJeu,case_getNumeroCase(dernierCoup));
        Sommet sommetAModifier=(case_getSommet(caseAModifier));
        s_couleur ancienneCouleur=sommet_getCouleur(sommetAModifier);
        sommet_changerCouleur(&sommetAModifier,t);

        Joueur joueur=(ancienneCouleur==w) ? jeu_getJoueur1(*i) :jeu_getJoueur2(*i);
        Groupe* groupesJoueur=joueur_getGroupesJoueur(joueur);
        nbGroupe = joueur_getNbGroupes(joueur);
        for (int ind=0;ind < nbGroupe;ind++)
            joueur_supprimerGroupe(&joueur,groupesJoueur[ind]);

        assert( joueur_getNbGroupes(joueur) == 0  );
        _chargerGroupesJoueur( i,joueur, NULL );
        (*i)->nbCoups--;
    }
}

static void _chargerGroupesJoueur( InfosJeu *i, Joueur j, int *nbCoupsAjout ){
    Case nouvelleCaseGroupe;
    Case caseAdjacentes[NB_ADJACENTS_MAX_PAR_CASE];
    Pile pileCoupsJoues=pile_copie(jeu_getPile(*i));
    s_couleur couleurJoueur = joueur_getCouleur( j );

    joueur_recupererGroupesBords(&j,(*i)->grilleJeu);

    for (;!pile_vide(pileCoupsJoues);) {
        nouvelleCaseGroupe=pile_sommet(pileCoupsJoues);
        pile_depiler(&pileCoupsJoues);

        if(sommet_getCouleur(case_getSommet(nouvelleCaseGroupe))==couleurJoueur ){
            grille_caseAdjacentes(jeu_getGrille(*i),case_getNumeroCase(nouvelleCaseGroupe),caseAdjacentes);
            joueur_integrerCaseDansGroupe(&j,nouvelleCaseGroupe,caseAdjacentes);
            if( nbCoupsAjout != NULL )
                (*nbCoupsAjout)++;
        }
        case_detruire(&nouvelleCaseGroupe);
    }
    pile_detruire(&pileCoupsJoues);
}

void jeu_chargerGroupesEtNbCoups(InfosJeu *i){
    Joueur tabJoueur[NB_JOUEUR];

    int nbGroupe;

    tabJoueur[0]=jeu_getJoueur1(*i);
    tabJoueur[1]=jeu_getJoueur2(*i);

    (*i)->nbCoups=0;
    for( size_t ind = 0 ; ind < NB_JOUEUR ; ind++ ){
        Groupe* groupesJoueur=joueur_getGroupesJoueur(tabJoueur[ind]);
        nbGroupe = joueur_getNbGroupes(tabJoueur[ind]);
        for (size_t j = 0; j < nbGroupe; j++)
            joueur_supprimerGroupe(&tabJoueur[ind],groupesJoueur[j]);

        _chargerGroupesJoueur( i, tabJoueur[ind], &(*i)->nbCoups );
        free(groupesJoueur);
    }

}

void jeu_toStringDernierCoup(InfosJeu i, char** chaine){
    Pile pile=jeu_getPile(i);
    unsigned l,c;
    
    if(!pile_vide(pile)){
        Case dernierCoup=pile_sommet( pile );
        if((*chaine=malloc(6))==NULL)
            abort();

        if( sommet_getCouleur( case_getSommet( dernierCoup ) ) == t ) 
            (*chaine)[0] = CARACTERE_POUR_COULEUR_T;
        else if( sommet_getCouleur( case_getSommet( dernierCoup ) ) == w ) 
            (*chaine)[0] = CARACTERE_POUR_COULEUR_W;
        else 
            (*chaine)[0] = CARACTERE_POUR_COULEUR_B;

        (*chaine)[1]=(*chaine)[3]=' ';

        Grille grille=jeu_getGrille(i);
        grille_numCaseToLigneColonne( case_getNumeroCase(dernierCoup), &l, &c, grille_obtenirTaille(grille) );
        
        char ligne[1];
        char colonne[1];
        sprintf(ligne,"%d",l);
        sprintf(colonne,"%d",(char)c);
        (*chaine)[2]=ligne[0];
        (*chaine)[4]=colonne[0];
        (*chaine)[5]='\0';
    }
}

InfosJeu jeu_copie(InfosJeu iSource){
    InfosJeu iCopie;
    if((iCopie=malloc(sizeof( struct s_InfosJeu)))==NULL)
        abort();

    iCopie->nbCoups=jeu_getNbCoups(iSource);
    grille_copie(&iCopie->grilleJeu, jeu_getGrille(iSource));
    iCopie->coupsJoues=pile_copie(jeu_getPile(iSource));
    iCopie->joueurs[0]=joueur_copie(jeu_getJoueur1(iSource));
    iCopie->joueurs[1]=joueur_copie(jeu_getJoueur2(iSource));
    if(jeu_getJoueur1(iSource)==jeu_getJoueurQuiCommence(iSource))
        iCopie->joueurQuiCommence=jeu_getJoueur1(iCopie);
    else
        iCopie->joueurQuiCommence=jeu_getJoueur2(iCopie);
    return iCopie;
}

bool jeu_comparer(InfosJeu i1, InfosJeu i2){
    if( !pile_comparer(jeu_getPile(i1),jeu_getPile(i2))
    || jeu_getNbCoups(i1)!=jeu_getNbCoups(i2) 
    || !joueur_comparer(jeu_getJoueur1(i1),jeu_getJoueur1(i2)) 
    || !joueur_comparer(jeu_getJoueur2(i1),jeu_getJoueur2(i2))
    || !joueur_comparer(jeu_getJoueurQuiCommence(i1),jeu_getJoueurQuiCommence(i2))
    || grille_comparer(jeu_getGrille(i1),jeu_getGrille(i2)))
        return false;
    return true;
}

void jeu_detruire(InfosJeu* i){
    grille_supprimer(&(*i)->grilleJeu);
    pile_detruire(&(*i)->coupsJoues);
    joueur_detruire(&(*i)->joueurs[0]);
    joueur_detruire(&(*i)->joueurs[1]);
    free(*i);
}
