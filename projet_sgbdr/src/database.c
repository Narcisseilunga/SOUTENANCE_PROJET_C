#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/database.h"


Table database[MAX_TABLES];
int table_count = 0;

void create_table(char *table_name) {
    if (table_count >= MAX_TABLES) {
        printf("Limite de tables atteinte.\n");
        return;
    }
    strcpy(database[table_count].name, table_name);
    database[table_count].field_count = 0;
    table_count++;
    printf("Table '%s' créée.\n", table_name);
}

void add_field(char *table_name, char *field_name, FieldType type) {
    for (int i = 0; i < table_count; i++) {
        if (strcmp(database[i].name, table_name) == 0) {
            if (database[i].field_count >= MAX_FIELDS) {
                printf("Limite de champs atteinte pour la table '%s'.\n", table_name);
                return;
            }
            strcpy(database[i].fields[database[i].field_count].name, field_name);
            database[i].fields[database[i].field_count].type = type;
            database[i].field_count++;
            printf("Champ '%s' ajoute à la table '%s'.\n", field_name, table_name);
            return;
        }
    }
    printf("Table '%s' introuvable.\n", table_name);
}

void display_data(char *table_name) {
    for (int i = 0; i < table_count; i++) {
        if (strcmp(database[i].name, table_name) == 0) {
            printf("Affichage des donnees de la table '%s':\n", table_name);
            for (int k = 0; k < database[i].field_count; k++) {
                if (database[i].fields[k].type == INT) {
                    printf("%s: INT\n", database[i].fields[k].name);
                } else if (database[i].fields[k].type == STRING) {
                    printf("%s: STRING\n", database[i].fields[k].name);
                }
            }
            return;
        }
    }
    printf("Table '%s' introuvable.\n", table_name);
}
