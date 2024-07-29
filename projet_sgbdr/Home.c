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


Table* creerTable(char* nom) {
    if (nombreTables >= MAX_TABLES) {
        printf("Nombre maximum de tables atteint.\n");
        return NULL;
    }
    Table* table = (Table*)malloc(sizeof(Table));
    if (!table) {
        printf("Erreur d'allocation de memoire pour la table.\n");
        exit(1);
    }
    strcpy(table->nom, nom);
    table->nombreChamps = 0;
    table->nombreLignes = 0;
    tables[nombreTables++] = table;
    return table;
}


void ajouterChamp(Table* table, char* nomChamp, TypeChamp type) {
    if (table->nombreChamps >= MAX_CHAMPS) {
        printf("Nombre maximum de champs atteint.\n");
        return;
    }
    strcpy(table->champs[table->nombreChamps].nom, nomChamp);
    table->champs[table->nombreChamps].type = type;
    table->nombreChamps++;
}
void afficherTable(Table* table) {
    printf("Table: %s\n", table->nom);
    for (int i = 0; i < table->nombreChamps; i++) {
        printf("Champ: %s, Type: %s\n", table->champs[i].nom, table->champs[i].type == ENTIER ? "Entier" : "Chaine");
    }
}



void insererDonnee(Table* table, void* donnees[]) {
    if (table->nombreLignes >= MAX_LIGNES) {
        printf("Nombre maximum de lignes atteint.\n");
        return;
    }
    for (int i = 0; i < table->nombreChamps; i++) {
        table->donnees[table->nombreLignes][i] = donnees[i];
    }
    table->nombreLignes++;
}

void afficherDonnees(Table* table) {
    printf("Table : %s\n", table->nom);
    for (int i = 0; i < table->nombreChamps; i++) {
        printf("%s\t", table->champs[i].nom);
    }
    printf("\n");
    for (int i = 0; i < table->nombreLignes; i++) {
        for (int j = 0; j < table->nombreChamps; j++) {
            if (table->champs[j].type == ENTIER) {
                printf("%d\t", *((int*)table->donnees[i][j]));
            } else {
                printf("%s\t", (char*)table->donnees[i][j]);
            }
        }
        printf("\n");
    }
}
void supprimerDonnee(Table* table, int index) {
    if (index < 0 || index >= table->nombreLignes) {
        printf("Index de ligne invalide.\n");
        return;
    }
    for (int i = index; i < table->nombreLignes - 1; i++) {
        for (int j = 0; j < table->nombreChamps; j++) {
            table->donnees[i][j] = table->donnees[i + 1][j];
        }
    }
    table->nombreLignes--;
}

void mettreAJourDonnee(Table* table, int index, int champIndex, void* nouvelleValeur) {
    if (index < 0 || index >= table->nombreLignes) {
        printf("Index de ligne invalide.\n");
        return;
    }
    if (champIndex < 0 || champIndex >= table->nombreChamps) {
        printf("Index de champ invalide.\n");
        return;
    }
    table->donnees[index][champIndex] = nouvelleValeur;
}

void trierDonnees(Table* table, int champIndex, int ordre) {
    if (champIndex < 0 || champIndex >= table->nombreChamps) {
        printf("Index de champ invalide.\n");
        return;

}
    for (int i = 0; i < table->nombreLignes - 1; i++) {
        for (int j = 0; j < table->nombreLignes - i - 1; j++) {
            int compare = 0;
            if (table->champs[champIndex].type == ENTIER) {
                compare = *((int*)table->donnees[j][champIndex]) - *((int*)table->donnees[j + 1][champIndex]);
            } else {
                compare = strcmp((char*)table->donnees[j][champIndex], (char*)table->donnees[j + 1][champIndex]);
            }
            if ((ordre == 0 && compare > 0) || (ordre == 1 && compare < 0)) {
                for (int k = 0; k < table->nombreChamps; k++) {
                    void* temp = table->donnees[j][k];
                    table->donnees[j][k] = table->donnees[j + 1][k];
                    table->donnees[j + 1][k] = temp;
                }
            }
        }
    }
}

Table* joindreTables(Table* table1, Table* table2, char* champCommun) {
    Table* tableJointe = creerTable("Jointure");
    if (!tableJointe) return NULL;
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
        printf("Champ commun non trouver dans l'une des tables.\n");
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
                tableJointe->nombreLignes;


                tableJointe->nombreLignes++;
            }
        }
    }
    return tableJointe;
}
void interpreterCommande(char* commande) {
    char* token = strtok(commande, " ");
    if (token == NULL) {
        printf("Commande vide.\n");
        return;
    }

    if (strcmp(token, "creer_table") == 0) {
        char* nomTable = strtok(NULL, " ");
        if (nomTable == NULL) {
            printf("Nom de table manquant.\n");
            return;
        }
        Table* table = creerTable(nomTable);
        if (table != NULL) {
            printf("Table '%s' creee.\n", nomTable);
        }
    } else if (strcmp(token, "ajouter_champ") == 0) {
        char* nomTable = strtok(NULL, " ");
        char* nomChamp = strtok(NULL, " ");
        char* typeChamp = strtok(NULL, " ");
        if (nomTable == NULL || nomChamp == NULL ||  typeChamp == NULL) {
            printf("Arguments manquants pour ajouter_champ.\n");
            return;
        }
        Table* table = NULL;
        for (int i = 0; i < nombreTables; i++) {
            if (strcmp(tables[i]->nom, nomTable) == 0) {
                table = tables[i];
                break;
            }
        }
        if (table == NULL) {
            printf("Table '%s' non trouvee.\n", nomTable);
            return;
        }
        if (strcmp(typeChamp, "entier") == 0) {
            ajouterChamp(table, nomChamp, ENTIER);
        } else if (strcmp(typeChamp, "chaine") == 0) {
            ajouterChamp(table, nomChamp, CHAINE);
        } else {
            printf("Type de champ invalide.\n");
            return;
        }
        printf("Champ '%s' de type '%s' ajouter a  la table '%s'.\n", nomChamp, typeChamp, nomTable);


    } else if (strcmp(token, "inserer") == 0) {
        char* nomTable = strtok(NULL, " ");
        if (nomTable == NULL) {
            printf("Nom de table manquant.\n");
            return;
        }
        Table* table = NULL;
        for (int i = 0; i < nombreTables; i++) {
            if (strcmp(tables[i]->nom, nomTable) == 0) {
                table = tables[i];
                break;
            }
        }
        if (table == NULL) {
            printf("Table '%s' non trouver.\n", nomTable);
            return;
        }
        void* donnees[MAX_CHAMPS];
        for (int i = 0; i < table->nombreChamps; i++) {
            char* valeur = strtok(NULL, " ");
            if (valeur == NULL) {
                printf("Valeur manquante pour le champ '%s'.\n", table->champs[i].nom);
                return;
            }
            if (table->champs[i].type == ENTIER) {
                int* val = (int*)malloc(sizeof(int));
                if (val == NULL) {
                    printf("Erreur d'allocation de memoire pour une valeur entiere.\n");
                    exit(1);
                }
                *val = atoi(valeur);
                donnees[i] = val;
            } else {
                char* val = (char*)malloc(50 * sizeof(char));
                if (val == NULL) {
                    printf("Erreur d'allocation de memoire pour une chaine.\n");
                    exit(1);
                }
                strcpy(val, valeur);
                donnees[i] = val;
            }
        }
        insererDonnee(table, donnees);
        printf("Donnees inserees dans la table '%s'.\n", nomTable);
    } else if (strcmp(token, "afficher") == 0) {
        char* nomTable = strtok(NULL, " ");
        if (nomTable == NULL) {
            printf("Nom de table manquant.\n");
            return;
        }
        Table* table = NULL;
        for (int i = 0; i < nombreTables; i++) {
            if (strcmp(tables[i]->nom, nomTable) == 0) {
                table = tables[i];
                break;
            }
        }
        if (table == NULL) {
    printf("Table '%s' non trouvee.\n", nomTable);
            return;
        }
        afficherDonnees(table);
    } else if (strcmp(token, "supprimer") == 0) {
        char* nomTable = strtok(NULL, " ");
        char* indexLigneStr = strtok(NULL, " ");
        if (nomTable == NULL || indexLigneStr == NULL) {
            printf("Arguments manquants pour supprimer.\n");
            return;
        }
        int indexLigne = atoi(indexLigneStr);
        Table* table = NULL;
        for (int i = 0; i < nombreTables; i++) {
            if (strcmp(tables[i]->nom, nomTable) == 0) {
                table = tables[i];
                break;
            }
        }
        if (table == NULL) {
            printf("Table '%s' non trouvee.\n", nomTable);
            return;
        }
        supprimerDonnee(table, indexLigne);
        printf("Ligne %d supprimee de la table '%s'.\n", indexLigne, nomTable);
    } else if (strcmp(token, "mettre_a_jour") == 0) {
        char* nomTable = strtok(NULL, " ");
        char* indexLigneStr = strtok(NULL, " ");
        char* indexChampStr = strtok(NULL, " ");
        char* nouvelleValeur = strtok(NULL, " ");
        if (nomTable == NULL  || indexLigneStr == NULL || indexChampStr == NULL || nouvelleValeur == NULL) {
            printf("Arguments manquants pour mettre_a_jour.\n");
            return;
        }
        int indexLigne = atoi(indexLigneStr);
        int indexChamp = atoi(indexChampStr);
        Table* table = NULL;
        for (int i = 0; i < nombreTables; i++) {
            if (strcmp(tables[i]->nom, nomTable) == 0) {
                table = tables[i];
                break;
            }
        }
        if (table == NULL) {
            printf("Table '%s' non trouvee.\n", nomTable);
            return;
        }
        void* valeur = NULL;
        if (table->champs[indexChamp].type == ENTIER) {
            int* val = (int*)malloc(sizeof(int));
            if (val == NULL) {
                printf("Erreur d'allocation de memoire pour une valeur entiere.\n");
                exit(1);
            }
            *val = atoi(nouvelleValeur);
            valeur = val;
        } else {
            char* val = (char*)malloc(50 * sizeof(char));
            if (val == NULL) {
                printf("Erreur d'allocation de memoire pour une chaine.\n");
                exit(1);
            }
            strcpy(val, nouvelleValeur);
            valeur = val;
        }
        mettreAJourDonnee(table, indexLigne, indexChamp, valeur);
        printf("Ligne %d mise a jour dans la table '%s'.\n", indexLigne, nomTable);
    } else if (strcmp(token, "trier") == 0) {
        char* nomTable = strtok(NULL, " ");
        char* indexChampStr = strtok(NULL, " ");
        char* ordreStr = strtok(NULL, " ");
        if (nomTable == NULL ||  indexChampStr == NULL || ordreStr == NULL) {
            printf("Arguments manquants pour trier.\n");
            return;
        }
        int indexChamp = atoi(indexChampStr);
        int ordre = atoi(ordreStr);
        Table* table = NULL;
        for (int i = 0; i < nombreTables; i++) {
            if (strcmp(tables[i]->nom, nomTable) == 0) {
                table = tables[i];
                break;
            }
        }
        if (table == NULL) {
            printf("Table '%s' non trouvee.\n", nomTable);
            return;
        }
        trierDonnees(table, indexChamp, ordre);
        printf("Table '%s' triee par le champ %d en ordre %d.\n", nomTable, indexChamp, ordre);
       } else if (strcmp(token, "joindre") == 0) {
        char* nomTable1 = strtok(NULL, " ");
        char* nomTable2 = strtok(NULL, " ");
        char* champCommun = strtok(NULL, " ");
        if (nomTable1 == NULL || nomTable2 == NULL  || champCommun == NULL) {
            printf("Arguments manquants pour joindre.\n");
        return;
        }
        Table* table1 = NULL;
        Table* table2 = NULL;
        for (int i = 0; i < nombreTables; i++) {
            if (strcmp(tables[i]->nom, nomTable1) == 0) {
                table1 = tables[i];
            }
            if (strcmp(tables[i]->nom, nomTable2) == 0) {
                table2 = tables[i];
            }
        }
        if (table1 != NULL && table2 != NULL) {
            Table* tableJointe = joindreTables(table1, table2, champCommun);
            if (tableJointe != NULL) {
                printf("Tables '%s' et '%s' jointes sur le champ '%s'.\n", nomTable1, nomTable2, champCommun);
                afficherDonnees(tableJointe);
            }
        } else {
            printf("Tables '%s' et/ou '%s' non trouvees.\n", nomTable1, nomTable2);
        }
    } else {
        printf("Commande inconnue : %s\n", token);
    }
}

int main() {
    char commande[256];
    printf("Bienvenue dans le SGBDR mini !\n");
    while (1) {
        printf("> ");
        if (fgets(commande, sizeof(commande), stdin) == NULL) {
            printf("\nFin de la session.\n");
            break;
        }
        // Remove newline character if present
        size_t len = strlen(commande);
        if (len > 0 && commande[len - 1] == '\n') {
            commande[len - 1] = '\0';
        }
        interpreterCommande(commande);
    }

    // Libérer la mémoire allouée pour les tables et les données
    for (int i = 0; i < nombreTables; i++) {
        for (int j = 0; j < tables[i]->nombreLignes; j++) {
            for (int k = 0; k < tables[i]->nombreChamps; k++) {
                free(tables[i]->donnees[j][k]);
            }
        }
        free(tables[i]);
    }

    return 0;
}















