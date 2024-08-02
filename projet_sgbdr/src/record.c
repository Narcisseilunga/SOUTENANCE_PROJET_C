#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "record.h"
#include "database.h"
Record table_records[MAX_TABLES];

void insert_data(char *table_name, RecordData data[]) {
    for (int i = 0; i < table_count; i++) {
        if (strcmp(database[i].name, table_name) == 0) {
            if (table_records[i].record_count >= MAX_RECORDS) {
                printf("Limite d'enregistrements atteinte pour la table '%s'.\n", table_name);
                return;
            }
            for (int j = 0; j < database[i].field_count; j++) {
                table_records[i].records[table_records[i].record_count][j] = data[j];
            }
            table_records[i].record_count++;
            printf("Données insérées dans la table '%s'.\n", table_name);
            return;
        }
    }
    printf("Table '%s' introuvable.\n", table_name);
}

void select_data(char *table_name, char *field_name, char *condition) {
    for (int i = 0; i < table_count; i++) {
        if (strcmp(database[i].name, table_name) == 0) {
            int field_index = -1;
            for (int j = 0; j < database[i].field_count; j++) {
                if (strcmp(database[i].fields[j].name, field_name) == 0) {
                    field_index = j;
                    break;
                }
            }
            if (field_index == -1) {
                printf("Champ '%s' introuvable dans la table '%s'.\n", field_name, table_name);
                return;
            }
            printf("Sélection des données de la table '%s' où '%s' %s:\n", table_name, field_name, condition);
            for (int k = 0; k < table_records[i].record_count; k++) {
                if (database[i].fields[field_index].type == INT) {
                    int value = table_records[i].records[k][field_index].int_value;
                    if (strcmp(condition, "est supérieur à 10") == 0 && value > 10) {
                        printf("Enregistrement %d: %d\n", k + 1, value);
                    }
                } else if (database[i].fields[field_index].type == STRING) {
                    char *value = table_records[i].records[k][field_index].string_value;
                    if (strcmp(condition, "est égal à 'exemple'") == 0 && strcmp(value, "exemple") == 0) {
                        printf("Enregistrement %d: %s\n", k + 1, value);
                    }
                }
            }
            return;
        }
    }
    printf("Table '%s' introuvable.\n", table_name);
}

void delete_data(char *table_name, char *field_name, char *condition) {
    for (int i = 0; i < table_count; i++) {
        if (strcmp(database[i].name, table_name) == 0) {
            int field_index = -1;
            for (int j = 0; j < database[i].field_count; j++) {
                if (strcmp(database[i].fields[j].name, field_name) == 0) {
                    field_index = j;
                    break;
                }
            }
            if (field_index == -1) {
                printf("Champ '%s' introuvable dans la table '%s'.\n", field_name, table_name);
                return;
            }
            for (int k = 0; k < table_records[i].record_count; k++) {
                if (database[i].fields[field_index].type == INT) {
                    int value = table_records[i].records[k][field_index].int_value;
                    if (strcmp(condition, "est supérieur à 10") == 0 && value > 10) {
                        for (int l = k; l < table_records[i].record_count - 1; l++) {
                            for (int m = 0; m < database[i].field_count; m++) {
                                table_records[i].records[l][m] = table_records[i].records[l + 1][m];
                            }
                        }
                        table_records[i].record_count--;
                        k--;
                    }
                } else if (database[i].fields[field_index].type == STRING) {
                    char *value = table_records[i].records[k][field_index].string_value;
                    if (strcmp(condition, "est égal à 'exemple'") == 0 && strcmp(value, "exemple") == 0) {
                        for (int l = k; l < table_records[i].record_count - 1; l++) {
                            for (int m = 0; m < database[i].field_count; m++) {
                                table_records[i].records[l][m] = table_records[i].records[l + 1][m];
                            }
                        }
                        table_records[i].record_count--;
                        k--;
                    }
                }
            }
            printf("Données supprimées de la table '%s' où '%s' %s.\n", table_name, field_name, condition);
            return;
        }
    }
    printf("Table '%s' introuvable.\n", table_name);
}

void update_data(char *table_name, char *field_name, char *condition, RecordData new_data) {
    for (int i = 0; i < table_count; i++) {
        if (strcmp(database[i].name, table_name) == 0) {
            int field_index = -1;
            for (int j = 0; j < database[i].field_count; j++) {
                if (strcmp(database[i].fields[j].name, field_name) == 0) {
                    field_index = j;
                    break;
                }
            }
            if (field_index == -1) {
                printf("Champ '%s' introuvable dans la table '%s'.\n", field_name, table_name);
                return;
            }
            for (int k = 0; k < table_records[i].record_count; k++) {
                if (database[i].fields[field_index].type == INT) {
                    int value = table_records[i].records[k][field_index].int_value;
                    if (strcmp(condition, "est supérieur à 10") == 0 && value > 10) {
                        table_records[i].records[k][field_index] = new_data[field_index];
                        printf("Enregistrement %d mis à jour.\n", k + 1);
                    }
                } else if (database[i].fields[field_index].type == STRING) {
                    char *value = table_records[i].records[k][field_index].string_value;
                    if (strcmp(condition, "est égal à 'exemple'") == 0 && strcmp(value, "exemple") == 0) {
                        table_records[i].records[k][field_index] = new_data[field_index];
                        printf("Enregistrement %d mis à jour.\n", k + 1);
                    }
                }
            }
            return;
        }
    }
    printf("Table '%s' introuvable.\n", table_name);
}
