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





