-- CREATION BASE DE DONNEES

CREATE DATABASE HEX;

DROP TABLE rencontrer;
DROP TABLE jouer;
DROP TABLE placer;
DROP TABLE PARTIE;
DROP TABLE COUP;
DROP TABLE CALENDRIER;
DROP TABLE JOUEUR;


CREATE TABLE JOUEUR (
	joueurID NUMBER,
	pseudo 	VARCHAR2(30),
	adresseMail VARCHAR2(30),
	anneeNaissance NUMBER,
	CONSTRAINT pk_joueur PRIMARY KEY (joueurID)
);

CREATE TABLE PARTIE (
	partieID NUMBER,
	joueurID_a NUMBER,
	joueurID_g NUMBER,
	CONSTRAINT pk_partie PRIMARY KEY (partieID),
	CONSTRAINT fk_partie_joueur_abandonner FOREIGN key(joueurID_a) REFERENCES JOUEUR(joueurID),
	CONSTRAINT fk_partie_joueur_gagner FOREIGN key(joueurID_g) REFERENCES JOUEUR(joueurID),
	CONSTRAINT ck_partie_gagner_abandonner CHECK (joueurID_a!=joueurID_g)
);

CREATE TABLE COUP (
	numCase NUMBER,
	CONSTRAINT pk_coup PRIMARY KEY (numCase)
);

CREATE TABLE CALENDRIER (
	datePartie DATE,
	CONSTRAINT pk_date PRIMARY KEY (datepartie)
);

CREATE TABLE rencontrer (
	joueur1ID_r NUMBER,
	joueur2ID_r NUMBER,
	CONSTRAINT pk_rencontrer PRIMARY KEY (joueur1ID_r, joueur2ID_r),
	CONSTRAINT ck_joueur2ID CHECK (joueur2ID_r!=joueur1ID_r),
	CONSTRAINT fk_jouer_joueur1 FOREIGN KEY(joueur1ID_r) REFERENCES JOUEUR(joueurID),
	CONSTRAINT fk_jouer_joueur2 FOREIGN KEY(joueur2ID_r) REFERENCES JOUEUR(joueurID)
) ;

CREATE TABLE jouer (
	joueurID_j NUMBER,
	partieID_j NUMBER,
	datePartie_j DATE,
	CONSTRAINT pk_jouer PRIMARY KEY (joueurid_j,partieID_j,datePartie_j),
	CONSTRAINT fk_jouer_joueur FOREIGN KEY(joueurID_j) REFERENCES JOUEUR(joueurID),
	CONSTRAINT fk_jouer_partie FOREIGN KEY(partieID_j) REFERENCES PARTIE(partieID),
	CONSTRAINT fk_jouer_date FOREIGN KEY(datePartie_j) REFERENCES CALENDRIER(datePartie)
);

CREATE TABLE placer (
	joueurID_p NUMBER,
	partieID_p NUMBER,
	numCase_p NUMBER,
	numCoup NUMBER,
	CONSTRAINT pk_placer PRIMARY KEY (joueurid_p,partieID_p,numCase_p),
	CONSTRAINT fk_placer_joueur FOREIGN KEY(joueurID_p) REFERENCES JOUEUR(joueurID),
	CONSTRAINT fk_placer_partie FOREIGN KEY(partieID_p) REFERENCES PARTIE(partieID),
	CONSTRAINT fk_placer_coup FOREIGN KEY(numCase_p) REFERENCES COUP(numCase)
);


--SELECTION DONNEES

--parties d'un joueur donnée
SELECT DISTINCT partieID_j FROM jouer,JOUEUR WHERE jouer.joueurID_j=JOUEUR.joueurID AND JOUEUR.pseudo='nom_du_joueur_recherche';

--parties d'un joueur donnée à une date donnée
SELECT DISTINCT partieID_j FROM jouer,JOUEUR WHERE jouer.joueurID_j=JOUEUR.joueurID AND JOUEUR.pseudo='nom_du_joueur_recherche' AND jouer.datePartie_j='date/a/chercher';

--parties gagnées par un joueur
SELECT DISTINCT partieID FROM PARTIE,JOUEUR WHERE PARTIE.joueurID_g=JOUEUR.joueurID AND JOUEUR.pseudo='nom_du_joueur_recherche';

--parties abandonnées auxquelles à participer un joueur
SELECT DISTINCT partieID FROM PARTIE,JOUEUR,jouer WHERE PARTIE.joueurID_a IS NOT NULL AND JOUEUR.pseudo='nom_du_joueur_recherche' AND PARTIE.partieID=jouer.partieID_j AND JOUEUR.pseudo=jouer.joueurID_j;

--joueur rencontrées par un joueur donné
SELECT DISTINCT adverse.joueurID FROM JOUEUR recherche, JOUEUR adverse,rencontrer WHERE (rencontrer.joueur1ID_r=recherche.joueurID AND recherche.pseudo='nom_du_joueur_recherche' AND adverse.joueurID=rencontrer.joueur2ID_r) OR
(rencontrer.joueur2ID_r=recherche.joueurID AND recherche.pseudo='nom_du_joueur_recherche' AND adverse.joueurID=rencontrer.joueur1ID_r);

--parties commencées par un coup donné
SELECT DISTINCT partieID_p FROM placer p WHERE p.numCase_p='coup_recherche' AND p.numCoup=1;
