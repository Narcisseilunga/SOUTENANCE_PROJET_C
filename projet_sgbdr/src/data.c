#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sgbdr.h"

void insert_data(char *table_name, void **data) {
    Table *table = find_table(table_name);
    if (table == NULL) {
        printf("Erreur : Table '%s' non trouvée.\n", table_name);
        return;
    }
    
    for (int i = 0; i < table->column_count; i++) {
        table->data[table->row_count * table->column_count + i] = data[i];
    }
    table->row_count++;
    printf("Données insérées dans la table '%s'.\n", table_name);
}

void display_data(char *table_name) {
    Table *table = find_table(table_name);
    if (table == NULL) {
        printf("Erreur : Table '%s' non trouvée.\n", table_name);
        return;
    }
    
    for (int i = 0; i < table->column_count; i++) {
        printf("%s\t", table->columns[i].name);
    }
    printf("\n");
    
    for (int i = 0; i < table->row_count; i++) {
        for (int j = 0; j < table->column_count; j++) {
            if (table->columns[j].type == INT) {
                printf("%d\t", *(int *)table->data[i * table->column_count + j]);
            } else if (table->columns[j].type == STRING) {
                printf("%s\t", (char *)table->data[i * table->column_count + j]);
            }
        }
        printf("\n");
    }
}

void delete_data(char *table_name, char *condition) {
    // Fonction à implémenter
    // Analyser et appliquer la condition pour supprimer les données
    printf("Suppression de données pour la table '%s' selon la condition '%s'.\n", table_name, condition);
}

void update_data(char *table_name, char *condition, void **new_data) {
    // Fonction à implémenter
    // Analyser et appliquer la condition pour mettre à jour les données
    printf("Mise à jour des données pour la table '%s' selon la condition '%s'.\n", table_name, condition);
}

void sort_data(char *table_name, char *column_name) {
    // Fonction à implémenter
    // Trouver la colonne et trier les données selon cette colonne
    printf("Tri des données de la table '%s' selon la colonne '%s'.\n", table_name, column_name);
}

void join_tables(char *table1_name, char *table2_name, char *join_column) {
    // Fonction à implémenter
    // Trouver les colonnes de jointure et afficher les données jointes
    printf("Jointure des tables '%s' et '%s' selon la colonne '%s'.\n", table1_name, table2_name, join_column);
}
