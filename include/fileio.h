#ifndef FILEIO_H
#define FILEIO_H

#include "sgbdr.h"

#define DATA_FILE "database.dat"

void sauvegarder_tables();
void charger_tables();

#endif // FILEIO_H
//gcc -o main.c database.c fileio.c -o home
//gcc -include main.c src/*.c -o sgbdr  https://github.com/Narcisseilunga/SOUTENANCE_PROJET_C