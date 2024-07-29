#ifndef SGBDR_H
#define SGBDR_H

#define MAX_TABLES 100
#define MAX_COLUMNS 100
#define MAX_COLUMN_NAME_LENGTH 50
#define MAX_TABLE_NAME_LENGTH 50
#define MAX_STRING_LENGTH 100

typedef enum {INT, STRING} ColumnType;

typedef struct {
    char name[MAX_COLUMN_NAME_LENGTH];
    ColumnType type;
} Column;
typedef struct {
    char name[MAX_TABLE_NAME_LENGTH];
    Column columns[MAX_COLUMNS];
    int column_count;
    int row_count;
    void **data;
} Table;

void create_table(char *name, Column *columns, int column_count);
void insert_data(char *table_name, void **data);
void select_data(char *table_name, char *condition);
void display_data(char *table_name);
void delete_data(char *table_name, char *condition);
void update_data(char *table_name, char *condition, void **new_data);
void sort_data(char *table_name, char *column_name);
void join_tables(char *table1_name, char *table2_name, char *join_column);
void interpret_command(char *command);

#endif
