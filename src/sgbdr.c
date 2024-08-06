#include"include/sgbdr.h"

Database databases[MAX_DATABASES];
int database_count = 0;
Database *current_database = NULL;

void creer_base(char *name) {
    if (database_count >= MAX_DATABASES) {
        printf("Erreur: nombre maximal de bases de données atteint.\n");
        return;
    }
    strcpy(databases[database_count].name, name);
    databases[database_count].table_count = 0;
    database_count++;
}

Database* trouver_base(char *name) {
    for (int i = 0; i < database_count; i++) {
        if (strcmp(databases[i].name, name) == 0) {
            return &databases[i];
        }
    }
    return NULL;
}

void selectionner_base(char *name) {
    Database *db = trouver_base(name);
    if (db == NULL) {
        printf("Erreur: base de données non trouvée.\n");
        return;
    }
    current_database = db;
}

void supprimer_base(char *name) {
    int index = -1;
    for (int i = 0; i < database_count; i++) {
        if (strcmp(databases[i].name, name) == 0) {
            index = i;
            break;
        }
    }
    if (index == -1) {
        printf("Erreur: base de données non trouvée.\n");
        return;
    }
    for (int i = index; i < database_count - 1; i++) {
        databases[i] = databases[i + 1];
    }
    database_count--;
    if (current_database == &databases[index]) {
        current_database = NULL;
    }
}
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
/*
void selectionner(char *table_name, char *field_name, char *condition, char *value) {
    Table *table = trouver_table(table_name);
    if (table == NULL) {
        printf("Erreur: table non trouvée.\n");
        return;
    }
    
    int field_index = -1;
    FieldType field_type = INTEGER;
    for (int i = 0; i < table->field_count; i++) {
        if (strcmp(table->fields[i].name, field_name) == 0) {
            field_index = i;
            field_type = table->fields[i].type;
            break;
        }
    }

    if (field_index == -1) {
        printf("Erreur: champ non trouvé.\n");
        return;
    }

    for (int i = 0; i < table->row_count; i++) {
        if (field_type == INTEGER) {
            int int_value = atoi(value);
            if (compare(table->int_data[i][field_index], condition, int_value)) {
                for (int j = 0; j < table->field_count; j++) {
                    if (table->fields[j].type == INTEGER) {
                        printf("%d\t", table->int_data[i][j]);
                    } else if (table->fields[j].type == STRING) {
                        printf("%s\t", table->string_data[i][j]);
                    }
                }
                printf("\n");
            }
        } else if (field_type == STRING) {
            if (strcmp(condition, "==") == 0 && strcmp(table->string_data[i][field_index], value) == 0) {
                for (int j = 0; j < table->field_count; j++) {
                    if (table->fields[j].type == INTEGER) {
                        printf("%d\t", table->int_data[i][j]);
                    } else if (table->fields[j].type == STRING) {
                        printf("%s\t", table->string_data[i][j]);
                    }
                }
                printf("\n");
            }
        }
    }
}*/