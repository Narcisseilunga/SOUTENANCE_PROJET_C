
#ifndef RECORD_H
#define RECORD_H

#include "database.h"

#define MAX_RECORDS 100

typedef struct {
    int int_value;
    char string_value[50];
} RecordData;

typedef struct {
    RecordData records[MAX_RECORDS][MAX_FIELDS];
    int record_count;
} Record;

void insert_data(char *table_name, RecordData data[]);
void select_data(char *table_name, char *field_name, char *condition);
void delete_data(char *table_name, char *field_name, char *condition);
void update_data(char *table_name, char *field_name, char *condition, RecordData new_data);

#endif // RECORD_H
