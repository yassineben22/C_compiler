char* check_vars(mot *tete){
	char *vars;
	int i=0;
	vars = (char*) malloc(10*sizeof(char));
	mot *tmp;
	tmp = tete;
	while(tmp != NULL){
		if(strcmp(tmp->valeur, "variable") == 0){
			tmp = tmp->suivant;
			
		}
	}
}

int analyse_sem(mot *tete){
	
}
