#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sgbdr.h"

void interpret_command(char *command) {
    char *token = strtok(command, " ");
    if (strcmp(token, "CREER_TABLE") == 0) {
        // Parse and call create_table
        char *table_name = strtok(NULL, " ");
        Column columns[MAX_COLUMNS];
        int column_count = 0;
        
        while ((token = strtok(NULL, " ")) != NULL) {
            strcpy(columns[column_count].name, token);
            token = strtok(NULL, " ");
            if (strcmp(token, "INT") == 0) {
                columns[column_count].type = INT;
            } else if (strcmp(token, "STRING") == 0) {
                columns[column_count].type = STRING;
            }
            column_count++;
        }
        
        create_table(table_name, columns, column_count);
    } else if (strcmp(token, "INSERER_DONNEES") == 0) {
        // Parse and call insert_data
        char *table_name = strtok(NULL, " ");
        void *data[MAX_COLUMNS];
        int i = 0;
        
        Table *table = find_table(table_name);
        if (table == NULL) {
            printf("Erreur : Table '%s' non trouvée.\n", table_name);
            return;
        }
        
        while ((token = strtok(NULL, " ")) != NULL) {
            if (table->columns[i].type == INT) {
                int *value = (int *)malloc(sizeof(int));
                *value = atoi(token);
                data[i] = value;
            } else if (table->columns[i].type == STRING) {
                char *value = (char *)malloc(strlen(token) + 1);
                strcpy(value, token);
                data[i] = value;
            }
            i++;
        }
        
        insert_data(table_name, data);
    } else if (strcmp(token, "AFFICHER_DONNEES") == 0) {
        char *table_name = strtok(NULL, " ");
        display_data(table_name);
    } else if (strcmp(token, "SUPPRIMER_DONNEES") == 0) {
        char *table_name = strtok(NULL, " ");
        char *condition = strtok(NULL, " ");
        delete_data(table_name, condition);
    } else if (strcmp(token, "METTRE_A_JOUR_DONNEES") == 0) {
        char *table_name = strtok(NULL, " ");
        char *condition = strtok(NULL, " ");
        void *new_data[MAX_COLUMNS];
        int i = 0;
        
        Table *table = find_table(table_name);
        if (table == NULL) {
            printf("Erreur : Table '%s' non trouvée.\n", table_name);
            return;
        }
        
        while ((token = strtok(NULL, " ")) != NULL) {
            if (table->columns[i].type == INT) {
                int *value = (int *)malloc(sizeof(int));
                *value = atoi(token);
                new_data[i] = value;
            } else if (table->columns[i].type == STRING) {
                char *value = (char *)malloc(strlen(token) + 1);
                strcpy(value, token);
                new_data[i] = value;
            }
            i++;
 }
        
        update_data(table_name, condition, new_data);
    } else if (strcmp(token, "TRIER_DONNEES") == 0) {
        char *table_name = strtok(NULL, " ");
        char *column_name = strtok(NULL, " ");
        sort_data(table_name, column_name);
    } else if (strcmp(token, "JOINDRE_TABLES") == 0) {
        char *table1_name = strtok(NULL, " ");
        char *table2_name = strtok
