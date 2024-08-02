#ifndef DATABASE_H
#define DATABASE_H

#define MAX_TABLES 10
#define MAX_FIELDS 10

typedef enum { 
    INT, 
    STRING,
    DECIMAL,
    BINAIRE,
    DATE } FieldType;

typedef struct {
    char name[30];
    FieldType type;
} Field;

typedef struct {
    char name[30];
    Field fields[MAX_FIELDS];
    int field_count;
} Table;

void create_table(char *table_name);
void add_field(char *table_name, char *field_name, FieldType type);
void display_data(char *table_name);

#endif // DATABASE_H
