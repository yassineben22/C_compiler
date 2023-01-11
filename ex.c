#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AnalyseLexicale.h"
#include "AnalyseSyntaxique.h"

mot* initialiser_mot(){
	mot *m = (mot*) malloc(sizeof(mot));
	m->suivant = NULL;
	return m;
}

mot* ajouter_fin(mot *tete, mot *m){
	if(tete == NULL)
		tete = m;
	else {
		mot *tmp;
		tmp = tete;
		while(tmp->suivant != NULL)
			tmp = tmp->suivant;
		tmp->suivant = m;
	}
	return tete;
}



//void afficher_liste(mot *tete, mot *dict){
//	int i=0;
//	mot *tmp;
//	tmp = tete;
//	while(tmp!=NULL){
//		printf("%s ==> %d", tmp->valeur, tmp->ligne);
//		if(tmp->suivant != NULL) printf(" | ");
//		tmp = tmp->suivant;
//		i++;
//	}
//}

mot* MotParMot(FILE *f, mot *tete){
	char c, i=0;
	int ligne_fichier = 1;
	mot *tmp;
	
	while((c = (char) fgetc(f)) != EOF){
		
		if(isspace(c)){
			if(c=='\n') ligne_fichier++;
			else continue;
		}
		tmp=initialiser_mot();
		while(!isspace(c)){
			tmp->valeur[i] = c;
			i++;
			if((c = (char) fgetc(f)) == EOF) break;
		}
		tmp->ligne = ligne_fichier;
		if(c=='\n') ligne_fichier++;
		tmp->valeur[i] = '\0';
 		i=0;
		tete = ajouter_fin(tete, tmp);
	}
	
	return tete;
}

int main(){
	FILE *fprogramme = fopen("programme.txt", "r"), *fdict = fopen("dictionnaire.txt", "r");
	mot *programme = NULL, *dict = NULL;
	programme = MotParMot(fprogramme, programme);
	dict = MotParMot(fdict, dict);
	int lex;
	printf("### Analyse Lexicale ###\n\n");
	lex = analyse_lex(programme, dict);
	if(lex == 0)
		printf("Pas d'erreurs...\n");
	printf("\n### Analyse Syntaxique ###\n\n");
	int a = analyse_syn(programme);
	fclose(fprogramme);
	fclose(fdict);
	return 0;
}
