
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
        printf("Erreur : Table '%s' non trouvée.\n", table_name);
        return;
    }

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

    for (int i = 0; i < table->row_count - 1; i++) {
        for (int j = 0; j < table->row_count - i - 1; j++) {
            int swap = 0;
            if (table->columns[column_index].type == INT) {
                if (*(int *)table->data[j * table->column_count + column_index] > *(int *)table->data[(j + 1) * table->column_count + column_index]) {
                    swap = 1;
                }
            } else if (table->columns[column_index].type == STRING) {
                if (strcmp((char *)table->data[j * table->column_count + column_index], (char *)table->data[(j + 1) * table->column_count + column_index]) > 0) {
                    swap = 1;
                }
            }
            if (swap) {
                for (int k = 0; k < table->column_count; k++) {
                    void *temp = table->data[j * table->column_count + k];
                    table->data[j * table->column_count + k] = table->data[(j + 1) * table->column_count + k];
                    table->data[(j + 1) * table->column_count + k] = temp;
                }
            }
        }
    }
    printf("Données triées dans la table '%s' par la colonne '%s'.\n", table_name, column_name);
}

void join_tables(char *table1_name, char *table2_name, char *join_column) {
    Table *table1 = find_table(table1_name);
    Table *table2 = find_table(table2_name);
    if (table1 == NULL || table2 == NULL) {
        printf("Erreur : Une ou les deux tables n'ont pas été trouvées.\n");
        return;
    }

    int join_index1 = -1, join_index2 = -1;
    for (int i = 0; i < table1->column_count; i++) {
        if (strcmp(table1->columns[i].name, join_column) == 0) {
            join_index1 = i;
            break;
        }
    }
    for (int i = 0; i < table2->column_count; i++) {
        if (strcmp(table2->columns[i].name, join_column) == 0) {
            join_index2 = i;
            break;
        }
    }

    if (join_index1 == -1 || join_index2 == -1) {
        printf("Erreur : Colonne de jointure '%s' non trouvée dans les tables.\n", join_column);
        return;
    }

    for (int i = 0; i < table1->row_count; i++) {
        for (int j = 0; j < table2->row_count; j++) {
            int match = 0;
            if (table1->columns[join_index1].type == INT && table2->columns[join_index2].type == INT) {
          match = (*(int *)table1->data[i * table1->column_count + join_index1] == *(int *)table2->data[j * table2->column_count + join_index2]);
        } else if (table1->columns[join_index1].type == STRING && table2->columns[join_index2].type == STRING) {
            match = (strcmp((char *)table1->data[i * table1->column_count + join_index1], (char *)table2->data[j * table2->column_count + join_index2]) == 0);
        }

        if (match) {
            for (int k = 0; k < table1->column_count; k++) {
                if (table1->columns[k].type == INT) {
                    printf("%d\t", *(int *)table1->data[i * table1->column_count + k]);
                } else if (table1->columns[k].type == STRING) {
                    printf("%s\t", (char *)table1->data[i * table1->column_count + k]);
                }
            }
            for (int k = 0; k < table2->column_count; k++) {
                if (k != join_index2) {
                    if (table2->columns[k].type == INT) {
                        printf("%d\t", *(int *)table2->data[j * table2->column_count + k]);
                    } else if (table2->columns[k].type == STRING) {
                        printf("%s\t", (char *)table2->data[j * table2->column_count + k]);
                    }
                }
            }
            printf("\n");
        }
    }
}
}

void select_data(char *table_name, char *condition) {
    Table *table = find_table(table_name);
    if (table == NULL) {
        printf("Erreur : Table '%s' non trouvée.\n", table_name);
        return;
    }

    char column_name[MAX_COLUMN_NAME_LENGTH];
    char operator;
    char value[MAX_STRING_LENGTH];
    sscanf(condition, "%[^<>!=]=%c%s", column_name, &operator, value);

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

    for (int i = 0; i < table->column_count; i++) {
        printf("%s\t", table->columns[i].name);
    }
    printf("\n");

    for (int i = 0; i < table->row_count; i++) {
        int match = 0;
        if (table->columns[column_index].type == INT) {
            int int_value = atoi(value);
            int data_value = *(int *)table->data[i * table->column_count + column_index];
            switch (operator) {
                case '=': match = (data_value == int_value); break;
                case '<': match = (data_value < int_value); break;
                case '>': match = (data_value > int_value); break;
                case '!': match = (data_value != int_value); break;
            }
        } else if (table->columns[column_index].type == STRING) {
            char *data_value = (char *)table->data[i * table->column_count + column_index];
            switch (operator) {
                case '=': match = (strcmp(data_value, value) == 0); break;
                case '!': match = (strcmp(data_value, value) != 0); break;
                // For strings, < and > are not usually used for comparison
            }
        }

        if (match) {
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
}
