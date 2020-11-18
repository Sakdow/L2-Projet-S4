#ifndef AFFICHAGE_
#define AFFICHAGE_


#include <jni.h>

JNIEnv* create_vm(JavaVM **jvm);

int affichage_afficherGrille(JNIEnv *env,char *grille,int dimension);


int affichage_afficherMessage(JNIEnv* env,char *message);

#endif
