#include <stdio.h>
#include "database.h"
#include "record.h"

int main() {
    create_table("Employees");
    add_field("Employees", "ID", INT);
    add_field("Employees", "Name", STRING);
    
    RecordData emp1[2] = { { .int_value = 1 }, { .string_value = "Alice" } };
    insert_data("Employees", emp1);
    
    RecordData emp2[2] = { { .int_value = 2 }, { .string_value = "Bob" } };
    insert_data("Employees", emp2);
    
    display_data("Employees");
    
    select_data("Employees", "ID", "est supérieur à 1");
    
    delete_data("Employees", "ID", "est supérieur à 1");
    
    display_data("Employees");
    
    RecordData new_data[2] = { { .int_value = 1 }, { .string_value = "Alice Updated" } };
    update_data("Employees", "Name", "est égal à 'Alice'", new_data);
    
    display_data("Employees");

    return 0;
}
