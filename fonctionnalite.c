#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAMPS 10
#define MAX_LIGNES 100

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

Table* creerTable(char* nom);
void ajouterChamp(Table* table, char* nomChamp, TypeChamp type);
void insererDonnee(Table* table, void* donnees[]);
void afficherDonnees(Table* table);

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

                compare = ((int)table->donnees[j][champIndex]) - ((int)table->donnees[j + 1][champIndex]);

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

        printf("Champ commun non trouvé dans l'une des tables.\n");

        return tableJointe;

    }

    for (int i = 0; i < table1->nombreLignes; i++) {

        for (int j = 0; j < table2->nombreLignes; j++) {

            int correspondance = 0;

            if (table1->champs[indexChamp1].type == ENTIER) {

                correspondance = (*((int*)table1->donnees[i][indexChamp1]) == ((int)table2->donnees[j][indexChamp2]));

            } else {

                correspondance = (strcmp((char*)table1->donnees[i][indexChamp1], (char*)table2->donnees[j][indexChamp2]) == 0);

            }

            if (correspondance) {

                for (int k = 0; k < table1->nombreChamps; k++) {

                    if (table1->champs[k].type == ENTIER) {

                        int* val = (int*)malloc(sizeof(int));

                        val = ((int*)table1->donnees[i][k]);

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

                        val = ((int*)table2->donnees[j][k]);

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