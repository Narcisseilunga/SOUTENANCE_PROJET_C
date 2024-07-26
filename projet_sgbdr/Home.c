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
void afficherTable(Table* table);
void insererDonnee(Table* table, void* donnees[]);

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

int main() {
    Table* table = creerTable("Etudiants");
    ajouterChamp(table, "ID", ENTIER);
    ajouterChamp(table, "Nom", CHAINE);

    int id = 1;
    char nom[] = "Alice";
    void* donnees1[] = {&id, nom};







