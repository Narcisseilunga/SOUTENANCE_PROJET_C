
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef DATABASE_H
#define DATABASE_H
#define MAX_TABLES 10
#define MAX_CHAMPS 10
#define MAX_LIGNES 100
#define MAX_DATABASES 5
#define MAX_TABLES 10
#define MAX_FIELDS 10
#define MAX_ROWS 100
#define MAX_COMMAND_LENGTH 100
#define MAX_NAME_LENGTH 50


typedef enum { INTEGER, STRING } FieldType;

typedef struct {
    char name[MAX_NAME_LENGTH];
    FieldType type;
} Field;


typedef struct {
    char name[MAX_NAME_LENGTH];
    //Table table[MAX_TABLES];
    int table_count;
} Database;

extern Database databases[MAX_DATABASES];
extern int database_count;
extern Database *current_database;

typedef enum {
     ENTIER,
     FLOAT,
     CHAINE,
     DATA,
     BINAIRE
       } TypeChamp;

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
    char name[MAX_NAME_LENGTH];
    Field fields[MAX_FIELDS];
    int field_count;
    int int_data[MAX_ROWS][MAX_FIELDS];
    char string_data[MAX_ROWS][MAX_FIELDS][MAX_NAME_LENGTH];
    int row_count;
} Table;

Table* tables[MAX_TABLES];
int nombreTables = 0;


void creer_base(char *name);
Database* trouver_base(char *name);
void selectionner_base(char *name);
void supprimer_base(char *name);
void creer_table(char *name);
void ajouterChamp(Table* table, char* nomChamp, TypeChamp type);
void insererDonnee(Table* table, void* donnees[]);
void afficherDonnees(Table* table);
void supprimerDonnee(Table* table, int index);
void mettreAJourDonnee(Table* table, int index, int champIndex, void* nouvelleValeur);
void trierDonnees(Table* table, int champIndex, int ordre);
Table* joindreTables(Table* table1, Table* table2, char* champCommun);
Table* trouver_table(char *name);
void selectionner(char *table_name, char *field_name, char *condition, char *value);


#endif // DATABASE_H
