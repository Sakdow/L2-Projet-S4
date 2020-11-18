Hex - Projet S4 - L2 INFORMATIQUE PAUL SABATIER -

Groupe 39 - Timothée Thibault - Damien Sarrias - Nathan Roche -

Compilation
===========

Pour la compilation de Hex il vous faut Java dans sa dernière version.
pour installer java en ligne de commande : 
sudo add-apt-repository -y ppa:webupd8team/java && sudo apt-get update && sudo apt-get install -y oracle-java8-installer


Ensuite il vous faut changer votre LD_LIBRARY_PATH.
Pour connaitre où se situe votre dossier java veuillez taper le commande suivante :
echo $JAVA_HOME

puis veuillez faire ceci :

export LD_LIBRARY_PATH=VOTRE_CHEMIN_JAVA/lib/amd64:VOTRE_CHEMIN_JAVA/jre/lib/amd64/server
ou
export LD_LIBRARY_PATH=VOTRE_CHEMIN_JAVA/lib/i386:VOTRE_CHEMIN_JAVA/jre/lib/i386/server

exemple :

export LD_LIBRARY_PATH=/usr/lib/jvm/java-8-oracle/lib/amd64:/usr/lib/jvm/java-8-oracle/jre/lib/amd64/server
ou
export LD_LIBRARY_PATH=/usr/lib/jvm/java-8-openjdk-i386/lib/i386:/usr/lib/jvm/java-8-openjdk-i386/jre/lib/i386/server

Dans le makefile changez la ligne JAVATOC=-L /usr/lib/jvm/java-8-oracle/jre/lib/amd64/server/ -I /usr/lib/jvm/java-8-oracle/include/ -I /usr/lib/jvm/java-8-oracle/include/linux/
	-Changez /usr/lib/jvm/java-8-oracle/jre/lib/amd64/server/ par VOTRE_CHEMIN_JAVA/jre/lib/amd64/server ou VOTRE_CHEMIN_JAVA/jre/lib/i386/server
	-Changez /usr/lib/jvm/java-8-oracle/include/ par VOTRE_CHEMIN_JAVA/include/
	-Changez /usr/lib/jvm/java-8-oracle/include/linux par VOTRE_CHEMIN_JAVA/include/linux

Les différentes options du makefile :

sommet : Test du module sommet
case   : Test du module case
graphe : Test du module graphe
grille : Test du module grille
pile   : Test du module pile
joueur : Test du module joueur
jeu    : Test du module jeu
hex    : Jeu hex

Les .o et .class se trouvent dans le dossier obj
Les fichiers sources se trouvent dans le dossier src
Les fichiers binaires se trouvent dans le dossier bin


Execution
=========

Pour executer les fichiers binaires, veuillez taper :
./bin/LE_NOM_DU_MODULE



