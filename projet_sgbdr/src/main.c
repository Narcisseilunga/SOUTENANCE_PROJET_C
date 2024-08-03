#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sgbdr.h"

void interpret_command(char *command);
int main() {
    char command[256];
     printf("Bienvenue dans le SGBDR mini !\n");
    while (1) {
        printf("> ");
        if (fgets(command, sizeof(command), stdin) != NULL) {
            command[strcspn(command, "\n")] = 0; // Remove newline character
            if (strcmp(command, "EXIT") == 0) {
                break;
            }
            interpret_command( command);
        }
    }
    
    return 0;
}

void interpret_command(char *command) {
    if (strncmp(command, "CREER TABLE", 11) == 0) {
        char *token = strtok(command + 12, " (");
        char *table_name = token;
        Column columns[MAX_COLUMNS];
        int column_count = 0;
        token = strtok(NULL, ", )");
        while (token != NULL) {
            char *column_name = token;
            token = strtok(NULL, " ");
            char *column_type = token;
            strncpy(columns[column_count].name, column_name, MAX_NAME_LENGTH);
            if (strncmp(column_type, "INT", 3) == 0) {
                columns[column_count].type = INT;
            } else if (strncmp(column_type, "VARCHAR", 7) == 0) {
                columns[column_count].type = STRING;
            } else if (strncmp(column_type, "DATE", 4) == 0) {
                columns[column_count].type = DATE;
            }
            column_count++;
            token = strtok(NULL, ", )");
        }
        create_table(table_name, columns, column_count);
    } else if (strncmp(command, "INSERER DANS", 12) == 0) {
        // Parse insertion command here
    } else if (strncmp(command, "AFFICHER", 8) == 0) {
        char *table_name = strtok(command + 9, " ");
        display_data(table_name);
    } else if (strncmp(command, "SUPPRIMER DE", 12) == 0) {
        // Parse deletion command here
    } else if (strncmp(command, "METTRE A JOUR", 13) == 0) {
        // Parse update command here
    } else if (strncmp(command, "TRIER", 5) == 0) {
        // Parse sorting command here
    } else if (strncmp(command, "JOINDRE", 7) == 0) {
        // Parse join command here
    } else {
        printf("Commande non reconnue.\n");
    }
}
