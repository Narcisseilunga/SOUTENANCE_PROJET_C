#include "include/fileio.h"

void sauvegarder_tables() {
    if (current_database == NULL) {
        printf("Erreur: aucune base de données sélectionnée pour sauvegarder.\n");
        return;
    }

    FILE *file = fopen(current_database->name, "wb");
    if (file == NULL) {
        printf("Erreur: impossible d'ouvrir le fichier pour la sauvegarde.\n");
        return;
    }

    fwrite(current_database, sizeof(Database), 1, file);
    fclose(file);
}

void charger_tables() {
    FILE *file;
    for (int i = 0; i < database_count; i++) {
        file = fopen(databases[i].name, "rb");
        if (file != NULL) {
            fread(&databases[i], sizeof(Database), 1, file);
            fclose(file);
        }
    }
}
