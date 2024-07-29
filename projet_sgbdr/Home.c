#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAMPS 10
#define MAX_LIGNES 100
#define MAX_TABLES 10
typedef enum {ENTIER, CHAINE} TypeChamp;

typedef struct {
    char nom[50];
    TypeChamp type;
} Champ;

typedef struct {
    char nom[50];
    Champ champs[MAX_CHAMPS];
    int nombreChamps;
    void* donnees[MAX_LIGNES][MAX_CHAMPS];
    int nombreLignes;
} Table;
Table* tables[MAX_TABLES];
int nombreTables = 0;


Table* creerTable(char* nom){
    Table* table = (Table*)malloc(sizeof(Table));
        strcpy(table->nom, nom);
    table->nombreChamps = 0;
    table->nombreLignes = 0;
    return table;
}

void ajouterChamp(Table* table, char* nomChamp, TypeChamp type);
void afficherTable(Table* table);
void insererDonnee(Table* table, void* donnees[]);

void ajouterChamp(Table* table, char* nomChamp, TypeChamp type) {
    if (table->nombreChamps < MAX_CHAMPS) {
        strcpy(table->champs[table->nombreChamps].nom, nomChamp);
        table->champs[table->nombreChamps].type = type;
        table->nombreChamps++;
    } else {
        printf("Nombre maximum de champs atteint.\n");
    }
}
void afficherTable(Table* table) {
    printf("Table: %s\n", table->nom);
    for (int i = 0; i < table->nombreChamps; i++) {
        printf("Champ: %s, Type: %s\n", table->champs[i].nom, table->champs[i].type == ENTIER ? "Entier" : "Chaine");
    }
}



void insererDonnee(Table* table, void* donnees[]) {
    if (table->nombreLignes < MAX_LIGNES) {
        for (int i = 0; i < table->nombreChamps; i++) {
            if (table->champs[i].type == ENTIER) {
                 int* val = (int*)malloc(sizeof(int));
            *val = *((int*)donnees[i]);
                table->donnees[table->nombreLignes][i] = val;
            } else {
                char* val = (char*)malloc(50 * sizeof(char));
                strcpy(val, (char*)donnees[i]);
                table->donnees[table->nombreLignes][i] = val;
            }
        }
        table->nombreLignes++;
    } else {
        printf("Nombre maximum de lignes atteint.\n");
    }
}

void afficherDonnees(Table* table) {
    for (int i = 0; i < table->nombreLignes; i++) {
        for (int j = 0; j < table->nombreChamps; j++) {
            if (table->champs[j].type == ENTIER) {
                printf("%d ", *((int*)table->donnees[i][j]));
            } else {
                printf("%s ", (char*)table->donnees[i][j]);
            }
        }
        printf("\n");
    }
}
void supprimerDonnee(Table* table, int ligne) {
    if (ligne < 0 || ligne >= table->nombreLignes) {
        printf("Indice de ligne invalide.\n");
        return;
    }
    for (int i = ligne; i < table->nombreLignes - 1; i++) {
        for (int j = 0; j < table->nombreChamps; j++) {
            table->donnees[i][j] = table->donnees[i + 1][j];
        }
    }
    table->nombreLignes--;
}

void mettreAJourDonnee(Table* table, int ligne, int champ, void* nouvelleValeur) {
    if (ligne < 0 || ligne >= table->nombreLignes || champ < 0 || champ >= table->nombreChamps) {
        printf("Indices invalides.\n");
        return;
    }
    if (table->champs[champ].type == ENTIER) {
        *((int*)table->donnees[ligne][champ]) = *((int*)nouvelleValeur);
    } else {
        strcpy((char*)table->donnees[ligne][champ], (char*)nouvelleValeur);
    }
    }

void trierDonnees(Table* table, int champ, int ordre) {
    for (int i = 0; i < table->nombreLignes - 1; i++) {
        for (int j = i + 1; j < table->nombreLignes; j++) {
            int comparer = 0;
            if (table->champs[champ].type == ENTIER) {
                comparer = *((int*)table->donnees[i][champ]) - *((int*)table->donnees[j][champ]);
            } else {
                comparer = strcmp((char*)table->donnees[i][champ], (char*)table->donnees[j][champ]);
            }
            if ((ordre == 1 && comparer > 0) || (ordre == 0 && comparer < 0)) {
                for (int k = 0; k < table->nombreChamps; k++) {
                    void* temp = table->donnees[i][k];
                    table->donnees[i][k] = table->donnees[j][k];
                    table->donnees[j][k] = temp;
                }
            }
        }
    }
}

Table* joindreTables(Table* table1, Table* table2, char* champCommun) {
    Table* tableJointe = creerTable("Jointure");
    for (int i = 0; i < table1->nombreChamps; i++) {
        ajouterChamp(tableJointe, table1->champs[i].nom, table1->champs[i].type);
    }
    for (int i = 0; i < table2->nombreChamps; i++) {
        ajouterChamp(tableJointe, table2->champs[i].nom, table2->champs[i].type);
    }

    int indexChamp1 = -1, indexChamp2 = -1;
    for (int i = 0; i < table1->nombreChamps; i++) {
        if (strcmp(table1->champs[i].nom, champCommun) == 0) {
            indexChamp1 = i;
            break;
        }
    }
    for (int i = 0; i < table2->nombreChamps; i++) {
        if (strcmp(table2->champs[i].nom, champCommun) == 0) {
            indexChamp2 = i;
            break;
        }
    }

    if (indexChamp1 == -1 || indexChamp2 == -1) {
        printf("Champ commun non trouvé dans l'une des tables.\n");
        return tableJointe;
    }

    for (int i = 0; i < table1->nombreLignes; i++) {
        for (int j = 0; j < table2->nombreLignes; j++) {
                        int correspondance = 0;
            if (table1->champs[indexChamp1].type == ENTIER) {
                correspondance = (*((int*)table1->donnees[i][indexChamp1]) == *((int*)table2->donnees[j][indexChamp2]));
            } else {
                correspondance = (strcmp((char*)table1->donnees[i][indexChamp1], (char*)table2->donnees[j][indexChamp2]) == 0);
            }

            if (correspondance) {
                for (int k = 0; k < table1->nombreChamps; k++) {
                    if (table1->champs[k].type == ENTIER) {
                          int* val = (int*)malloc(sizeof(int));
                            *val = *((int*)table1->donnees[i][k]);
                        tableJointe->donnees[tableJointe->nombreLignes][k] = val;
                    } else {
                        char* val = (char*)malloc(50 * sizeof(char));
                        strcpy(val, (char*)table1->donnees[i][k]);
                        tableJointe->donnees[tableJointe->nombreLignes][k] = val;
                    }
                }
                for (int k = 0; k < table2->nombreChamps; k++) {
                    if (table2->champs[k].type == ENTIER) {
                        int* val = (int*)malloc(sizeof(int));
                                                *val = *((int*)table2->donnees[j][k]);
                        tableJointe->donnees[tableJointe->nombreLignes][table1->nombreChamps + k] = val;
                    } else {
                        char* val = (char*)malloc(50 * sizeof(char));
                        strcpy(val, (char*)table2->donnees[j][k]);
                        tableJointe->donnees[tableJointe->nombreLignes][table1->nombreChamps + k] = val;
                    }
                }
                tableJointe->nombreLignes++;
            }
        }
    }
    return tableJointe;
}

void interpreterCommande(char* commande) {
    char* token = strtok(commande, " ");
    if (strcmp(token, "creer_table") == 0) {
        char* nomTable = strtok(NULL, " ");
        Table* table = creerTable(nomTable);
        printf("Table '%s' créée.\n", nomTable);
    } else if (strcmp(token, "ajouter_champ") == 0) {
        char* nomTable = strtok(NULL, " ");
        char* nomChamp = strtok(NULL, " ");
        char* typeChamp = strtok(NULL, " ");
        TypeChamp type = strcmp(typeChamp, "entier") == 0 ? ENTIER : CHAINE;
        for (int i = 0; i < nombreTables; i++) {
            if (strcmp(tables[i]->nom, nomTable) == 0) {
                ajouterChamp(tables[i], nomChamp, type);
                printf("Champ '%s' de type '%s' ajouté à la table '%s'.\n", nomChamp, typeChamp, nomTable);
                return;
            }
        }
        printf("Table '%s' non trouvée.\n", nomTable);
    } else if (strcmp(token, "inserer") == 0) {
        char* nomTable = strtok(NULL, " ");
        void* donnees[MAX_CHAMPS];
        for (int i = 0; i < nombreTables; i++) {
             if (strcmp(tables[i]->nom, nomTable) == 0) {
                 for (int j = 0; j < tables[i]->nombreChamps; j++) {
                    if (tables[i]->champs[j].type == ENTIER) {
                        int* val = (int*)malloc(sizeof(int));
                        *val = atoi(strtok(NULL, " "));
                        donnees[j] = val;
                    } else {
                        char* val = (char*)malloc(50 * sizeof(char));
                        strcpy(val, strtok(NULL, " "));
                        donnees[j] = val;
                    }
                }
                insererDonnee(tables[i], donnees);
                printf("Données insérées dans la table '%s'.\n", nomTable);
                return;
            }
        }
                printf("Table '%s' non trouvée.\n", nomTable);
    } else if (strcmp(token, "afficher") == 0) {
        char* nomTable = strtok(NULL, " ");
        for (int i = 0; i < nombreTables; i++) {
            if (strcmp(tables[i]->nom, nomTable) == 0) {
                afficherDonnees(tables[i]);
                return;
            }
        }
        printf("Table '%s' non trouvée.\n", nomTable);
    } else if (strcmp(token, "supprimer") == 0) {
        char* nomTable = strtok(NULL, " ");
        int ligne = atoi(strtok(NULL, " "));
        for (int i = 0; i < nombreTables; i++) {
            if (strcmp(tables[i]->nom, nomTable) == 0) {
                supprimerDonnee(tables[i], ligne);
                            printf("Ligne %d supprimée de la table '%s'.\n", ligne, nomTable);
                return;
            }
        }
        printf("Table '%s' non trouvée.\n", nomTable);
    } else if (strcmp(token, "mettre_a_jour") == 0) {
        char* nomTable = strtok(NULL, " ");
        int ligne = atoi(strtok(NULL, " "));
        int champ = atoi(strtok(NULL, " "));
        void* nouvelleValeur;
        for (int i = 0; i < nombreTables; i++) {
            if (strcmp(tables[i]->nom, nomTable) == 0) {
                if (tables[i]->champs[champ].type == ENTIER) {
                    int* val = (int*)malloc(sizeof(int));
                    *val = atoi(strtok(NULL, " "));
                    nouvelleValeur = val;
                } else {
                    char* val = (char*)malloc(50 * sizeof(char));
                    strcpy(val, strtok(NULL, " "));
                    nouvelleValeur = val;
                }
                mettreAJourDonnee(tables[i], ligne, champ, nouvelleValeur);
                printf("Ligne %d mise à jour dans la table '%s'.\n", ligne, nomTable);
                return;
            }
        }
        printf("Table '%s' non trouvée.\n", nomTable);
    } else if (strcmp(token, "trier") == 0) {
        char* nomTable = strtok(NULL, " ");
        int champ = atoi(strtok(NULL, " "));
        int ordre = atoi(strtok(NULL, " "));
        for (int i = 0; i < nombreTables; i++) {
            if (strcmp(tables[i]->nom, nomTable) == 0) {
                trierDonnees(tables[i], champ, ordre);
                printf("Table '%s' triée par le champ %d en ordre %d.\n", nomTable, champ, ordre);
                return;
            }
        }
        printf("Table '%s' non trouvée.\n", nomTable);
    } else if (strcmp(token, "joindre") == 0) { 
        char* nomTable1 = strtok(NULL, " ");
        char* nomTable2 = strtok(NULL, " ");
        char* champCommun = strtok(NULL, " ");
        Table* table1 = NULL;
        Table* table2 = NULL;
        for (int i = 0; i < nombreTables; i++) {
            if (strcmp(tables[i]->nom, 
nomTable1) == 0) {
                table1 = tables[i];
            } else if (strcmp(tables[i]->nom, nomTable2) == 0) {
                table2 =

                table2 = tables[i];
            }
        }
        if (table1 != NULL && table2 != NULL) {
            Table* tableJointe = joindreTables(table1, table2, champCommun);
            printf("Tables '%s' et '%s' jointes par le champ '%s'.\n", nomTable1, nomTable2, champCommun);
            afficherDonnees(tableJointe);
        } else {
                    printf("Une des tables '%s' ou '%s' non trouvée.\n", nomTable1, nomTable2);
        }
    } else {
        printf("Commande inconnue : %s\n", token);
    }
}

int main() {
    char commande[256];
    while (1) {
        printf("SGBDR> ");
        if (fgets(commande, sizeof(commande), stdin) == NULL) {
            break;
        }
        commande[strcspn(commande, "\n")] = 0;  // Supprime le caractère de nouvelle ligne
        if (strcmp(commande, "exit") == 0) {
                        break;
        }
        interpreterCommande(commande);
    }
    for (int i = 0; i < nombreTables; i++) {
        free(tables[i]);
    }
    return 0;
}














