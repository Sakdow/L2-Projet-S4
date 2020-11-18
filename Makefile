.PHONY: all clean maxclean


# Les différents répertoires contenant respectivement les fichiers : Sources *.c, Objets *.o, l'exécutable

OBJDIR=obj
SRCDIR=src
BINDIR=bin

JAVATOC=-L /usr/lib/jvm/java-8-oracle/jre/lib/amd64/server/ -I /usr/lib/jvm/java-8-oracle/include/ -I /usr/lib/jvm/java-8-oracle/include/linux/
LJVM=-ljvm
CFLAGS= -Wall -std=c99
LDFLAGS=
OBJ=
EXEC=sommet case graphe pile grille sauvegarde graphe jeu joueur

all: hex

sommet: sommet.o test.o test_sommet.o
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $^
	mv *.o $(OBJDIR)/

case: sommet.o case.o test.o test_case.o
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $(OBJDIR)/$^
	mv *.o $(OBJDIR)/

graphe: sommet.o case.o graphe.o test.o test_graphe.o
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $(OBJDIR)/$^
	mv *.o $(OBJDIR)/

pile: sommet.o case.o pile.o test.o test_pile.o
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $^
	mv *.o $(OBJDIR)/

grille: sommet.o case.o graphe.o grille.o test.o test_grille.o
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $^
	mv *.o $(OBJDIR)/

sauvegarde: sommet.o case.o graphe.o grille.o groupe.o pile.o joueur.o jeu.o sauvegarde.o test.o test_sauvegarde.o
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $^
	mv *.o $(OBJDIR)/

menu : sommet.o case.o graphe.o grille.o groupe.o joueur.o pile.o jeu.o affichage.o sauvegarde.o IA.o menu.o test.o test_menu.o
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $^
	mv *.o $(OBJDIR)/
	
jeu: sommet.o case.o graphe.o grille.o groupe.o joueur.o pile.o jeu.o test.o test_jeu.o
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $^
	mv *.o $(OBJDIR)/

joueur: sommet.o case.o groupe.o graphe.o grille.o joueur.o test.o test_joueur.o
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $^
	mv *.o $(OBJDIR)/

groupe: sommet.o case.o groupe.o test.o test_groupe.o
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $^
	mv *.o $(OBJDIR)/

hex: sommet.o case.o graphe.o grille.o groupe.o joueur.o pile.o jeu.o affichage.o sauvegarde.o IA.o menu.o hex.o
	javac $(SRCDIR)/Affichage.java
	$(CC) $(CFLAGS) -o $(BINDIR)/$@ $(JAVATOC) $^ $(LJVM)
	mv $(SRCDIR)/*.class $(OBJDIR)/
	mv *.o $(OBJDIR)/

%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(JAVATOC) -c $< -o $@
	

clean:
	rm -rf $(OBJDIR)/*
	rm -rf *.o

maxclean: clean
	rm -f $(BINDIR)/*


