/************************************************************
 * MODULE affichage                                         *
 *                                                          *
 *  --> Interfaçage Java/c du module affichage ecrit en     *
 *      Java                                                *
 *                                                          *
 * Roche Nathan - Dernière modification : 19/05/2017        *
 *                                                          *
 * Groupe : Damien Sarrias, Timothée Thibault               *
 ************************************************************/



#include <stdio.h>
#include <jni.h>

JNIEnv* create_vm(JavaVM **jvm)
{
    JNIEnv* env;
    JavaVMInitArgs args;
    JavaVMOption options;
    args.version = JNI_VERSION_1_8;
    args.nOptions = 1;
    options.optionString = "-Djava.class.path=obj/";
    args.options = &options;
    args.ignoreUnrecognized = 0;
    JNI_CreateJavaVM(jvm, (void**)&env, &args);

    return env;
}


void affMessage( JNIEnv* env, jobject obj, jmethodID mid, char * message){
	jstring jstr = (*env)->NewStringUTF(env,message);
	(*env)->CallObjectMethod(env, obj, mid, jstr );
} 

void affGrille( JNIEnv* env, jobject obj, jmethodID mid, char * grille, int taille){
	jstring jstr = (*env)->NewStringUTF(env,grille);
	(*env)->CallStaticVoidMethod(env, obj, mid, jstr ,(jint)taille );
}




void invoke_class_afficherMessage(JNIEnv* env,char *message)
{
	jclass Affichage_class;
	jobject obj;
	jmethodID constructeur;
	jmethodID afficherMessage;
	Affichage_class = (*env)->FindClass(env, "Affichage");
	constructeur = (*env)->GetMethodID(env,Affichage_class, "<init>", "()V");
	afficherMessage = (*env)->GetMethodID(env, Affichage_class, "afficherMessage", "(Ljava/lang/String;)V");
	obj = (*env)->NewObject(env,Affichage_class, constructeur);
	affMessage( env, obj, afficherMessage, message); 
}


void invoke_class_afficherGrille(JNIEnv* env,char *grille,int dimension)
{
    jclass Affichage_class;
	jobject obj;
    jmethodID afficherGrille;
	jmethodID constructeur;
    Affichage_class = (*env)->FindClass(env, "Affichage");

	constructeur = (*env)->GetMethodID(env,Affichage_class, "<init>", "()V");
    afficherGrille = (*env)->GetMethodID(env, Affichage_class, "afficherGrille","(Ljava/lang/String;I)V");
	
	obj = (*env)->NewObject(env,Affichage_class, constructeur);

	affGrille(env, obj, afficherGrille,grille ,dimension );

}

int affichage_afficherGrille(JNIEnv *env,char *grille,int dimension)
{
    invoke_class_afficherGrille(env,grille,dimension);
    return 0;
}

int affichage_afficherMessage(JNIEnv* env,char *message)
{
    invoke_class_afficherMessage(env,message);
    return 0;
}
