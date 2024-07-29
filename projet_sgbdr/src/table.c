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
