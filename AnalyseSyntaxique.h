

mot* avancer(mot *liste){
//	mot *tmp;
//	tmp = liste;
	liste = liste->suivant;
//	if(liste )
//	free(tmp);
	return liste;
}

mot* avancer_ligne(mot *liste){
	if(liste->suivant == NULL) return NULL;
	int current = liste->ligne;
	mot *tmp;
	while(liste != NULL && liste->ligne == current)
		liste = liste->suivant;
	if(liste == NULL) return NULL;
	return liste;
}

void signaler_erreur(mot *line, mot *m, char *str){
	FILE *err = fopen("erreur.txt", "a+");
	mot *tmp;
	tmp = line;
	int ligne;
	ligne = (strcmp(str, "manque de ;") == 0 ) || (strcmp(str, "manque de .") == 0 ) ?  m->ligne - 1 : m->ligne;
	printf("l'erreur est apres: ");
	fprintf(err, "l'erreur est apres: ");
	while(tmp->valeur != m->valeur){
		printf("%s ", tmp->valeur);
		fprintf(err, "%s ", tmp->valeur);
		tmp = tmp->suivant;
	}
	printf("*********  %s ********* Veuillez voir la ligne %d\n",  str, ligne);
	fprintf(err, "********* %s ********* Veuillez voir la ligne %d\n", str, ligne);
	fclose(err);
}

int analyse_debut(mot *line){
	mot *backup;
	backup = line;
	if(strcmp(line->valeur, "debut") == 0){
		line = avancer(line);
		if(strcmp(line->valeur, ";") == 0){
			line = avancer(line);
			return 0;
		}
		else {
			signaler_erreur(backup, line, "manque de ;");
			line = avancer(line);
			return 1;
		}
	}
}

int analyse_declaration(mot *line){
	mot *backup;
	backup = line;
	if(strcmp(line->valeur, "declaration") == 0){
		line = avancer(line);
		if(strcmp(line->valeur, ";") == 0){
			line = avancer(line);
			return 0;
		}
		else {
			signaler_erreur(backup, line, "manque de ;");
			line = avancer(line);
			return 1;
		}
	}
}

int analyse_variable(mot *line){
	mot *backup;
	backup = line;
	if(strcmp(line->valeur, "variable") == 0){
		line = avancer(line);
		if( ((line->valeur[0] >= 'a' && line->valeur[0] <= 'z') || (line->valeur[0] >= 'A' && line->valeur[0] <= 'Z')) && line->valeur[1]=='\0'){
			line = avancer(line);
			if(strcmp(line->valeur, ":") == 0){
				line = avancer(line);
				if(strcmp(line->valeur, "entier") == 0){
					line = avancer(line);
					if(strcmp(line->valeur, ";") == 0){
						line = avancer(line);
						return 0;
					} else {
						signaler_erreur(backup, line, "manque de ;");
						return 1;
					}
				} else {
					signaler_erreur(backup, line, "type de variable invalide");
					return 1;
				}
			} else {
				signaler_erreur(backup, line, "manque de :");
				return 1;
			}
		}
		else {
			signaler_erreur(backup, line, "nom de variable invalide");
			return 1;
		}
	}
}

int analyse_affectation(mot *line){
	mot *backup;
	backup = line;
	int i;
	if( ((line->valeur[0] >= 'a' && line->valeur[0] <= 'z') || (line->valeur[0] >= 'A' && line->valeur[0] <= 'Z')) && line->valeur[1]=='\0'){
		line = avancer(line);
		if(strcmp(line->valeur, ":=") == 0){
			line = avancer(line);
			for(i=0;i<strlen(line->valeur);i++){
				if(!(line->valeur[i] >= '0' && line->valeur[i] <='9')){
					signaler_erreur(backup, line, "nombre invalide");
					return 1;
				}
			}
			line = avancer(line);
			if(strcmp(line->valeur, ";") == 0){
				line = avancer(line);
				return 0;
			} else {
				signaler_erreur(backup, line, "manque de ;");
				return 1;
			}
		}
		else {
			signaler_erreur(backup, line, "manque de \":=\"");
			return 1;
		}
	}
}

int analyse_lire(mot *line){
	mot *backup;
	backup = line;
	if(strcmp(line->valeur, "lire") == 0){
		line = avancer(line);
		if(strcmp(line->valeur, "(") == 0){
			line = avancer(line);
			if( ((line->valeur[0] >= 'a' && line->valeur[0] <= 'z') || (line->valeur[0] >= 'A' && line->valeur[0] <= 'Z')) && line->valeur[1]=='\0'){
				line = avancer(line);
				if(strcmp(line->valeur, ")") == 0){
					line = avancer(line);
					if(strcmp(line->valeur, ";") == 0){
						line = avancer(line);
						return 0;
					} else {
						signaler_erreur(backup, line, "manque de ;");
						return 1;
					}
				} else {
					signaler_erreur(backup, line, "manque de )");
					return 1;
				}
			} else {
				signaler_erreur(backup, line, "nom de variable invalide");
				return 1;
			}
		}
		else {
			signaler_erreur(backup, line, "manque de (");
			return 1;
		}
	}
}

int analyse_ecrire(mot *line){
	mot *backup;
	backup = line;
	int current;
	if(strcmp(line->valeur, "ecrire") == 0){
		line = avancer(line);
		if(strcmp(line->valeur, "(") == 0){
			line = avancer(line);
			if( ((line->valeur[0] >= 'a' && line->valeur[0] <= 'z') || (line->valeur[0] >= 'A' && line->valeur[0] <= 'Z')) && line->valeur[1]=='\0'){
				line = avancer(line);
				if(strcmp(line->valeur, ")") == 0){
					line = avancer(line);
					if(strcmp(line->valeur, ";") == 0){
						return 0;
					}
					else {
						signaler_erreur(backup, line, "manque de ;");
						return 1;
					}
				} else {
					signaler_erreur(backup, line, "manque de )");
					return 1;
				}
			}
			else if(strcmp(line->valeur, "<<") == 0){
				line = avancer(line);
				current = line->ligne;
				while(1){
					if((strcmp(line->valeur, ";") == 0 || line->suivant == NULL || line->suivant->ligne != current) && strcmp(line->valeur, ">>") != 0){
						signaler_erreur(backup, line, "manque de >>");
						return 1;
					}
					else if(strcmp(line->valeur, ">>") == 0){
						line = avancer(line);
						if(strcmp(line->valeur, ")") == 0){
							line = avancer(line);
							if(strcmp(line->valeur, ";") == 0){
								return 0;
							}
							else {
								signaler_erreur(backup, line, "manque de ;");
								return 1;
							}
						} else {
							signaler_erreur(backup, line, "manque de )");
							return 1;
						}
					}
					line = avancer(line);
				}
			} else {
				signaler_erreur(backup, line, "manque de << ou d'un nom de variable");
				return 1;
			}
		}
		else {
			signaler_erreur(backup, line, "manque de (");
			return 1;
		}
	}
}

int analyse_si(mot *line){
	mot *backup;
	backup = line;
	int i;
	if(strcmp(line->valeur, "si") == 0){
		line = avancer(line);
		if(strcmp(line->valeur, "(") == 0){
			line = avancer(line);
			if( ((line->valeur[0] >= 'a' && line->valeur[0] <= 'z') || (line->valeur[0] >= 'A' && line->valeur[0] <= 'Z')) && line->valeur[1]=='\0'){
				line = avancer(line);
				if(strcmp(line->valeur, "<") == 0 || strcmp(line->valeur, ">") == 0 || strcmp(line->valeur, "==") == 0){
					line = avancer(line);
					for(i=0;i<strlen(line->valeur);i++){
						if(!(line->valeur[i] >= '0' && line->valeur[i] <='9')){
							signaler_erreur(backup, line, "nombre invalide");
							return 1;
						}
					}
					line = avancer(line);
					if(strcmp(line->valeur, ")") == 0){
						line = avancer(line);
						if(strcmp(line->valeur, "alors") == 0){
							int err=0;
							line = avancer_ligne(line);
							if( ((line->valeur[0] >= 'a' && line->valeur[0] <= 'z') || (line->valeur[0] >= 'A' && line->valeur[0] <= 'Z')) && line->valeur[1]=='\0'){
								err = analyse_affectation(line);
							}
							else if(strcmp(line->valeur, "lire") == 0){
								err = analyse_lire(line);
							}
							else if(strcmp(line->valeur, "ecrire") == 0){
								err = analyse_ecrire(line);
							} else {
								signaler_erreur(backup, line, "instruction inconnue");
								return 1;
							}
							
							if(err == 0 ){
								line = avancer_ligne(line);
								if(strcmp(line->valeur, "sinon") == 0){
									err=0;
									line = avancer_ligne(line);
									if( ((line->valeur[0] >= 'a' && line->valeur[0] <= 'z') || (line->valeur[0] >= 'A' && line->valeur[0] <= 'Z')) && line->valeur[1]=='\0'){
										err = analyse_affectation(line);
									}
									else if(strcmp(line->valeur, "lire") == 0){
										err = analyse_lire(line);
									}
									else if(strcmp(line->valeur, "ecrire") == 0){
										err = analyse_ecrire(line);
									} else {
										signaler_erreur(backup, line, "instruction inconnue");
										return 1;
									}
									if(err == 0){
										line = avancer_ligne(line);
										if(strcmp(line->valeur, "finsi") == 0){
											return analyse_finsi(line);
										}
										signaler_erreur(backup, line, "manque de finsi");
										return 1;
									}
									else
										return 1;
								} else {
									signaler_erreur(backup, line, "manque de 'sinon'");
									return 1;
								}
							} else 
								return 1;
						} else {
							signaler_erreur(backup, line, "manque de 'alors'");
							return 1;
						}
					} else {
						signaler_erreur(backup, line, "manque de )");
						return 1;
					}
				} else {
					signaler_erreur(backup, line, "manque de )");
					return 1;
				}
			} else {
				signaler_erreur(backup, line, "nom de variable invalide");
				return 1;
			}
		}
		else {
			signaler_erreur(backup, line, "manque de (");
			return 1;
		}
	}
}

int analyse_finsi(mot *line){
	mot *backup;
	backup = line;
	if(strcmp(line->valeur, "finsi") == 0){
		line = avancer(line);
		if(strcmp(line->valeur, ";") == 0){
			line = avancer(line);
			return 0;
		}
		else {
			signaler_erreur(backup, line, "manque de ;");
			line = avancer(line);
			return 1;
		}
	}
}

int analyse_fin(mot *line){
	mot *backup;
	backup = line;
	if(strcmp(line->valeur, "fin") == 0){
		
		line = avancer(line);
		if(strcmp(line->valeur, ".") == 0){
			
			return 0;
		}
		else {
			signaler_erreur(backup, line, "manque de .");
			return 1;
		}
	}
}

mot* generer_ligne(mot *debut);

int analyse_syn(mot *tete){
	int err = 0;
	while(tete != NULL){
		if(strcmp(tete->valeur, "debut") == 0){
			err = analyse_debut(tete);
		}
		if(strcmp(tete->valeur, "declaration") == 0){
			err = analyse_declaration(tete);
		}
		if(strcmp(tete->valeur, "variable") == 0){
			err = analyse_variable(tete);
		}
		if( ((tete->valeur[0] >= 'a' && tete->valeur[0] <= 'z') || (tete->valeur[0] >= 'A' && tete->valeur[0] <= 'Z')) && tete->valeur[1]=='\0'){
			err = analyse_affectation(tete);
		}
		if(strcmp(tete->valeur, "lire") == 0){
			err = analyse_lire(tete);
		}
		if(strcmp(tete->valeur, "ecrire") == 0){
			err = analyse_ecrire(tete);
		}
		if(strcmp(tete->valeur, "si") == 0){
			err = analyse_si(tete);
		}
		if(strcmp(tete->valeur, "finsi") == 0){
			err = analyse_finsi(tete);
		}
		if(strcmp(tete->valeur, "fin") == 0){
			err = analyse_fin(tete);
			
		}
		tete = avancer_ligne(tete);
		
	}
	return err;

}

mot* generer_ligne(mot *debut){
	if(debut == NULL) return NULL;
	int current = debut->ligne;
	mot *nouv = NULL, *tmp;
	while(debut->ligne == current && debut != NULL){
		if(nouv == NULL)
			nouv = debut;
		else{
			tmp = nouv;
			while(tmp->suivant != NULL)
				tmp = tmp->suivant;
			tmp->suivant = debut;
		}
		debut = debut->suivant;
	}
	return nouv;
}


