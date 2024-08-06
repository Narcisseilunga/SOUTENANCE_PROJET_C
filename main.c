#include "include/fileio.h"
#include"include/sgbdr.h"
#define MAX_NAME_LENGTH 50
void interpreterCommande(char* commande) {
    char* token = strtok(commande, " ");
    if (token == NULL) {
        printf("Commande vide.\n");
        return;
    }
    else if (strcmp(token, "creer_base") == 0) {
        char base_name[MAX_NAME_LENGTH];
        sscanf(commande + strlen(token), "%s", base_name);
        creer_base(base_name);
    } else if (strcmp(token, "selectionner_base") == 0) {
        char base_name[MAX_NAME_LENGTH];
        sscanf(commande + strlen(token), "%s", base_name);
        selectionner_base(base_name);
    } else if (strcmp(token, "supprimer_base") == 0) {
        char base_name[MAX_NAME_LENGTH];
        sscanf(commande + strlen(token), "%s", base_name);
        supprimer_base(base_name);

    }else if (strcmp(token, "creer_table") == 0) {
        char* nomTable = strtok(NULL, " ");
        if (nomTable == NULL) {
            printf("Nom de table manquant.\n");
            return;
        }
        Table* table = creerTable(nomTable);
        if (table != NULL) {
            printf("Table '%s' creee.\n", nomTable);
        }
    } else if (strcmp(token, "ajouter_champ") == 0) {
        char* nomTable = strtok(NULL, " ");
        char* nomChamp = strtok(NULL, " ");
        char* typeChamp = strtok(NULL, " ");
        if (nomTable == NULL || nomChamp == NULL ||  typeChamp == NULL) {
            printf("Arguments manquants pour ajouter_champ.\n");
            return;
        }
        Table* table = NULL;
        for (int i = 0; i < nombreTables; i++) {
            if (strcmp(tables[i]->nom, nomTable) == 0) {
                table = tables[i];
                break;
            }
        }
        if (table == NULL) {
            printf("Table '%s' non trouvee.\n", nomTable);
            return;
        }
        if (strcmp(typeChamp, "entier") == 0) {
            ajouterChamp(table, nomChamp, ENTIER);
        } else if (strcmp(typeChamp, "char") == 0) {
            ajouterChamp(table, nomChamp, CHAINE);
        } else {
            printf("Type de champ invalide.\n");
            return;
        }
        printf("Champ '%s' de type '%s' ajouter a  la table '%s'.\n", nomChamp, typeChamp, nomTable);


    } else if (strcmp(token, "inserer") == 0) {
        char* nomTable = strtok(NULL, " ");
        if (nomTable == NULL) {
            printf("Nom de table manquant.\n");
            return;
        }
        Table* table = NULL;
        for (int i = 0; i < nombreTables; i++) {
            if (strcmp(tables[i]->nom, nomTable) == 0) {
                table = tables[i];
                break;
            }
        }
        if (table == NULL) {
            printf("Table '%s' non trouver.\n", nomTable);
            return;
        }
        void* donnees[MAX_CHAMPS];
        for (int i = 0; i < table->nombreChamps; i++) {
            char* valeur = strtok(NULL, " ");
            if (valeur == NULL) {
                printf("Valeur manquante pour le champ '%s'.\n", table->champs[i].nom);
                return;
            }
            if (table->champs[i].type == ENTIER) {
                int* val = (int*)malloc(sizeof(int));
                if (val == NULL) {
                    printf("Erreur d'allocation de memoire pour une valeur entiere.\n");
                    exit(1);
                }
                *val = atoi(valeur);
                donnees[i] = val;
            } else {
                char* val = (char*)malloc(50 * sizeof(char));
                if (val == NULL) {
                    printf("Erreur d'allocation de memoire pour une chaine.\n");
                    exit(1);
                }
                strcpy(val, valeur);
                donnees[i] = val;
            }
        }
        insererDonnee(table, donnees);
        printf("Donnees inserees dans la table '%s'.\n", nomTable);
    } else if (strcmp(token, "afficher") == 0) {
        char* nomTable = strtok(NULL, " ");
        if (nomTable == NULL) {
            printf("Nom de table manquant.\n");
            return;
        }
        Table* table = NULL;
        for (int i = 0; i < nombreTables; i++) {
            if (strcmp(tables[i]->nom, nomTable) == 0) {
                table = tables[i];
                break;
            }
        }
        if (table == NULL) {
    printf("Table '%s' non trouvee.\n", nomTable);
            return;
        }
        afficherDonnees(table);
    } else if (strcmp(token, "supprimer") == 0) {
        char* nomTable = strtok(NULL, " ");
        char* indexLigneStr = strtok(NULL, " ");
        if (nomTable == NULL || indexLigneStr == NULL) {
            printf("Arguments manquants pour supprimer.\n");
            return;
        }
        int indexLigne = atoi(indexLigneStr);
        Table* table = NULL;
        for (int i = 0; i < nombreTables; i++) {
            if (strcmp(tables[i]->nom, nomTable) == 0) {
                table = tables[i];
                break;
            }
        }
        if (table == NULL) {
            printf("Table '%s' non trouvee.\n", nomTable);
            return;
        }
        supprimerDonnee(table, indexLigne);
        printf("Ligne %d supprimee de la table '%s'.\n", indexLigne, nomTable);
    } else if (strcmp(token, "mettre_a_jour") == 0) {
        char* nomTable = strtok(NULL, " ");
        char* indexLigneStr = strtok(NULL, " ");
        char* indexChampStr = strtok(NULL, " ");
        char* nouvelleValeur = strtok(NULL, " ");
        if (nomTable == NULL  || indexLigneStr == NULL || indexChampStr == NULL || nouvelleValeur == NULL) {
            printf("Arguments manquants pour mettre_a_jour.\n");
            return;
        }
        int indexLigne = atoi(indexLigneStr);
        int indexChamp = atoi(indexChampStr);
        Table* table = NULL;
        for (int i = 0; i < nombreTables; i++) {
            if (strcmp(tables[i]->nom, nomTable) == 0) {
                table = tables[i];
                break;
            }
        }
        if (table == NULL) {
            printf("Table '%s' non trouvee.\n", nomTable);
            return;
        }
        void* valeur = NULL;
        if (table->champs[indexChamp].type == ENTIER) {
            int* val = (int*)malloc(sizeof(int));
            if (val == NULL) {
                printf("Erreur d'allocation de memoire pour une valeur entiere.\n");
                exit(1);
            }
            *val = atoi(nouvelleValeur);
            valeur = val;
        } else {
            char* val = (char*)malloc(50 * sizeof(char));
            if (val == NULL) {
                printf("Erreur d'allocation de memoire pour une chaine.\n");
                exit(1);
            }
            strcpy(val, nouvelleValeur);
            valeur = val;
        }
        mettreAJourDonnee(table, indexLigne, indexChamp, valeur);
        printf("Ligne %d mise a jour dans la table '%s'.\n", indexLigne, nomTable);
    } else if (strcmp(token, "trier") == 0) {
        char* nomTable = strtok(NULL, " ");
        char* indexChampStr = strtok(NULL, " ");
        char* ordreStr = strtok(NULL, " ");
        if (nomTable == NULL ||  indexChampStr == NULL || ordreStr == NULL) {
            printf("Arguments manquants pour trier.\n");
            return;
        }
        int indexChamp = atoi(indexChampStr);
        int ordre = atoi(ordreStr);
        Table* table = NULL;
        for (int i = 0; i < nombreTables; i++) {
            if (strcmp(tables[i]->nom, nomTable) == 0) {
                table = tables[i];
                break;
            }
        }
        if (table == NULL) {
            printf("Table '%s' non trouvee.\n", nomTable);
            return;
        }
        trierDonnees(table, indexChamp, ordre);
        printf("Table '%s' triee par le champ %d en ordre %d.\n", nomTable, indexChamp, ordre);
       } else if (strcmp(token, "joindre") == 0) {
        char* nomTable1 = strtok(NULL, " ");
        char* nomTable2 = strtok(NULL, " ");
        char* champCommun = strtok(NULL, " ");
        if (nomTable1 == NULL || nomTable2 == NULL  || champCommun == NULL) {
            printf("Arguments manquants pour joindre.\n");
        return;
        }
        Table* table1 = NULL;
        Table* table2 = NULL;
        for (int i = 0; i < nombreTables; i++) {
            if (strcmp(tables[i]->nom, nomTable1) == 0) {
                table1 = tables[i];
            }
            if (strcmp(tables[i]->nom, nomTable2) == 0) {
                table2 = tables[i];
            }
        }
        if (table1 != NULL && table2 != NULL) {
            Table* tableJointe = joindreTables(table1, table2, champCommun);
            if (tableJointe != NULL) {
                printf("Tables '%s' et '%s' jointes sur le champ '%s'.\n", nomTable1, nomTable2, champCommun);
                afficherDonnees(tableJointe);
            }
        } else {
            printf("Tables '%s' et/ou '%s' non trouvees.\n", nomTable1, nomTable2);
        }
    } else {
        printf("Commande inconnue : %s\n", token);
    }
}

int main() {
    char commande[256];

    printf("Bienvenue dans le SGBDR mini !\n");
    while (1) {
        printf("> ");
        if (fgets(commande, sizeof(commande), stdin) == NULL) {
            printf("\nFin de la session.\n");
            break;
        }
        // Remove newline character if present
        size_t len = strlen(commande);
        if (len > 0 && commande[len - 1] == '\n') {
            commande[len - 1] = '\0';
        }
        interpreterCommande(commande);
    }

    // Libérer la mémoire allouée pour les tables et les données
    for (int i = 0; i < nombreTables; i++) {
        for (int j = 0; j < tables[i]->nombreLignes; j++) {
            for (int k = 0; k < tables[i]->nombreChamps; k++) {
                free(tables[i]->donnees[j][k]);
            }
        }
        free(tables[i]);
    }

    return 0;
}
