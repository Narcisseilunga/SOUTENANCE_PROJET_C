#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sgbdr.h"

Table *database = NULL;

void create_table(char *name, Column *columns, int column_count) {
    Table *new_table = (Table *)malloc(sizeof(Table));
    strncpy(new_table->name, name, MAX_NAME_LENGTH);
    new_table->columns = (Column *)malloc(sizeof(Column) * column_count);
    memcpy(new_table->columns, columns, sizeof(Column) * column_count);
    new_table->column_count = column_count;
    new_table->rows = NULL;
    new_table->next = database;
    database = new_table;
    printf("Table %s créée avec succès.\n", name);
}

void insert_data(char *table_name, void **values) {
    Table *table = find_table(table_name);
    if (table == NULL) {
        printf("Erreur : Table %s non trouvée.\n", table_name);
        return;
    }
    Row *new_row = (Row *)malloc(sizeof(Row));
    new_row->values = (void **)malloc(sizeof(void *) * table->column_count);
    memcpy(new_row->values, values, sizeof(void *) * table->column_count);
    new_row->next = table->rows;
    table->rows = new_row;
    printf("Données insérées avec succès dans la table %s.\n", table_name);
}

void display_data(char *table_name) {
    Table *table = find_table(table_name);
    if (table == NULL) {
        printf("Erreur : Table %s non trouvée.\n", table_name);
        return;
    }
    for (int i = 0; i < table->column_count; i++) {
        printf("%s\t", table->columns[i].name);
    }
    printf("\n");
    Row *row = table->rows;
    while (row != NULL) {
        for (int i = 0; i < table->column_count; i++) {
            if (table->columns[i].type == INT) {
                printf("%d\t", *((int *)row->values[i]));
            } else if (table->columns[i].type == STRING || table->columns[i].type == DATE) {
                printf("%s\t", (char *)row->values[i]);
            }
        }
        printf("\n");
        row = row->next;
    }
}

void delete_data(char *table_name, char *condition) {
    Table *table = find_table(table_name);
    if (table == NULL) {
        printf("Erreur : Table '%s' non trouvée.\n", table_name);
        return;
    }

    char column_name[MAX_COLUMN_NAME_LENGTH];
    char value[MAX_STRING_LENGTH];
    sscanf(condition, "%[^=]=%s", column_name, value);

    int column_index = -1;
    for (int i = 0; i < table->column_count; i++) {
        if (strcmp(table->columns[i].name, column_name) == 0) {
            column_index = i;
            break;
        }
    }

    if (column_index == -1) {
        printf("Erreur : Colonne '%s' non trouvée dans la table '%s'.\n", column_name, table_name);
        return;
    }

    int new_row_count = 0;
    for (int i = 0; i < table->row_count; i++) {
        int match = 0;
        if (table->columns[column_index].type == INT) {
            match = (*(int *)table->data[i * table->column_count + column_index] == atoi(value));
        } else if (table->columns[column_index].type == STRING) {
            match = (strcmp((char *)table->data[i * table->column_count + column_index], value) == 0);
        }
        if (!match) {
            for (int j = 0; j < table->column_count; j++) {
                table->data[new_row_count * table->column_count + j] = table->data[i * table->column_count + j];
            }
            new_row_count++;
        }
    }
    table->row_count = new_row_count;
    printf("Données supprimées de la table '%s'.\n", table_name);
}

void update_data(char *table_name, char *condition, void **new_data) {
    Table *table = find_table(table_name);
    if (table == NULL) {
        printf("Erreur : Table '%s' non trouvée.\n", table_name);
        return;
    }

    char column_name[MAX_COLUMN_NAME_LENGTH];
    char value[MAX_STRING_LENGTH];
    sscanf(condition, "%[^=]=%s", column_name, value);

    int column_index = -1;
    for (int i = 0; i < table->column_count; i++) {
        if (strcmp(table->columns[i].name, column_name) == 0) {
            column_index = i;
            break;
        }
    }

    if (column_index == -1) {
        printf("Erreur : Colonne '%s' non trouvée dans la table '%s'.\n", column_name, table_name);
        return;
    }

    for (int i = 0; i < table->row_count; i++) {
        int match = 0;
        if (table->columns[column_index].type == INT) {
            match = (*(int *)table->data[i * table->column_count + column_index] == atoi(value));
        } else if (table->columns[column_index].type == STRING) {
            match = (strcmp((char *)table->data[i * table->column_count + column_index], value) == 0);
        }
        if (match) {
            for (int j = 0; j < table->column_count; j++) {
                table->data[i * table->column_count + j] = new_data[j];
            }
        }
    }
    printf("Données mises à jour dans la table '%s'.\n", table_name);
}


void sort_data(char *table_name, char *column_name) {
    Table *table = find_table(table_name);
    if (table == NULL) {
        printf("Erreur : Table %s non trouvée.\n", table_name);
        return;
    }

    // Find the column index for the specified column name
    int column_index = -1;
    for (int i = 0; i < table->column_count; i++) {
        if (strcmp(table->columns[i].name, column_name) == 0) {
            column_index = i;
            break;
        }
    }
    if (column_index == -1) {
        printf("Erreur : Colonne %s non trouvée dans la table %s.\n", column_name, table_name);
        return;
    }

    // Simple bubble sort for sorting the rows
    int swapped;
    Row *row;
    Row *last = NULL;
    if (table->rows == NULL) {
        return;
    }
    do {
        swapped = 0;
        row = table->rows;
        while (row->next != last) {
            if (compare_values(row->values[column_index], row->next->values[column_index], table->columns[column_index].type) > 0) {
                void **temp = row->values;
                row->values = row->next->values;
                row->next->values = temp;
                swapped = 1;
            }
            row = row->next;
        }
        last = row;
    } while (swapped);

    printf("Table %s triée par %s.\n", table_name, column_name);
}
int compare_values(void *a, void *b, ColumnType type) {
    if (type == INT) {
        return (*(int *)a - *(int *)b);
    } else if (type == STRING || type == DATE) {
        return strcmp((char *)a, (char *)b);
    }
    return 0;
}

void join_tables(char *table1_name, char *table2_name, char *join_column) {
    Table *table1 = find_table(table1_name);
    Table *table2 = find_table(table2_name);
    if (table1 == NULL || table2 == NULL) {
        printf("Erreur : Une des tables de jointure non trouvée.\n");
        return;
    }

    // Find the column indexes for the specified join column
    int column_index1 = -1;
    int column_index2 = -1;
    for (int i = 0; i < table1->column_count; i++) {
        if (strcmp(table1->columns[i].name, join_column) == 0) {
            column_index1 = i;
            break;
        }
    }
    for (int i = 0; i < table2->column_count; i++) {
        if (strcmp(table2->columns[i].name, join_column) == 0) {
            column_index2 = i;
            break;
        }
    }
    if (column_index1 == -1 || column_index2 == -1) {
        printf("Erreur : Colonne %s non trouvée dans une des tables.\n", join_column);
        return;
    }

    // Print headers
    for (int i = 0; i < table1->column_count; i++) {
        printf("%s\t", table1->columns[i].name);
    }
    for (int i = 0; i < table2->column_count; i++) {
        if (i != column_index2) {
            printf("%s\t", table2->columns[i].name);
        }
    }
    printf("\n");

    // Perform the join
    Row *row1 = table1->rows;
    while (row1 != NULL) {
        Row *row2 = table2->rows;
        while (row2 != NULL) {
            if (compare_values(row1->values[column_index1], row2->values[column_index2], table1->columns[column_index1].type) == 0) {
                for (int i = 0; i < table1->column_count; i++) {
                    if (table1->columns[i].type == INT) {
                        printf("%d\t", *((int *)row1->values[i]));
                    } else {
                        printf("%s\t", (char *)row1->values[i]);
                    }
                }
                for (int i = 0; i < table2->column_count; i++) {
                    if (i != column_index2) {
                        if (table2->columns[i].type == INT) {
                            printf("%d\t", *((int *)row2->values[i]));
                        } else {
                            printf("%s\t", (char *)row2->values[i]);
                        }
                    }
                }
                printf("\n");
            }
            row2 = row2->next;
        }
        row1 = row1->next;
    }
}


Table *find_table(char *name) {
    Table *current_table = database;
    while (current_table != NULL) {
        if (strcmp(current_table->name, name) == 0) {
            return current_table;
        }
        current_table = current_table->next;
    }
    return NULL;
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
