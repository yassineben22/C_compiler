typedef struct mot{
	char valeur[20];
	int ligne;
	struct mot *suivant;
} mot;


int check_mot(mot* tmp, mot* dict){
	mot *tmp2;
	int i;
	if( ((tmp->valeur[0] >= 'a' && tmp->valeur[0] <= 'z') || (tmp->valeur[0] >= 'A' && tmp->valeur[0] <= 'Z')) && tmp->valeur[1]=='\0')
		return 1;
	for(i=0;i<strlen(tmp->valeur);i++){
		if(!(tmp->valeur[i] >= '0' && tmp->valeur[i] <='9'))
			goto skip_nombre;
	}
	return 1;
	skip_nombre:
		tmp2=dict;
		while(tmp2!=NULL){
			if(strcmp(tmp->valeur, tmp2->valeur) == 0)
				return 1;
			tmp2=tmp2->suivant;
		}
	
	return 0;
}

mot* analyse_lex(mot *tete, mot *dict){
	mot *tmp;
	FILE *err = fopen("erreur.txt", "w");
	tmp = tete;
	int closed=1, rtn=0;
	while(tmp!=NULL){
		if(strcmp(tmp->valeur, "<<") == 0) closed = 0;
		else if(strcmp(tmp->valeur, ">>") == 0) closed = 1;
		else if(closed == 0) goto skip;
		else{
			if(check_mot(tmp, dict) == 0){
				rtn = 1;
				signaler_err(err, tmp);
			}
		}
		skip:
		tmp = tmp->suivant;
	}
	fclose(err);
	return rtn;
}

void signaler_err(FILE *ferr, mot *analyse){
	printf("\"%s\" n'appartient pas au dictionnaire || voir ligne: %d\n", analyse->valeur, analyse->ligne);
	fprintf(ferr, "probleme d'analyse lexicale! \"%s\" n'appartient pas au dictionnaire || Veuillez voir la ligne %d\n", analyse->valeur, analyse->ligne);
}
