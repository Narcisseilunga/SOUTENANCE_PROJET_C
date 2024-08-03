#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"
#include "sgbdr.h"

Table *database = NULL;
database = malloc(sizeof(Database));
//Table database[MAX_TABLES];
