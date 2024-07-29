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
