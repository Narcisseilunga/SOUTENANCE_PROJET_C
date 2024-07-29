#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sgbdr.h"

Table tables[MAX_TABLES];
int table_count = 0;

void create_table(char *name, Column *columns, int column_count) {
    if (table_count >= MAX_TABLES) {
        printf("Erreur : Nombre maximum de tables atteint.\n");
        return;
    }
    
    strcpy(tables[table_count].name, name);
    tables[table_count].column_count = column_count;
    tables[table_count].row_count = 0;
    
    for (int i = 0; i < column_count; i++) {
                strcpy(tables[table_count].columns[i].name, columns[i].name);
        tables[table_count].columns[i].type = columns[i].type;
    }
    
    tables[table_count].data = (void **)malloc(MAX_COLUMNS * MAX_STRING_LENGTH * sizeof(void *));
    table_count++;
    printf("Table '%s' créée avec succès.\n", name);
}

Table* find_table(char *name) {
    for (int i = 0; i < table_count; i++) {
        if (strcmp(tables[i].name, name) == 0) {
            return &tables[i];
        }
    }
    return NULL;
}

