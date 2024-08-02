#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sgbdr.h"

int main() {
    char command[256];
     printf("Bienvenue dans le SGBDR mini !\n");
    while (1) {
        printf("> ");
        if (fgets(command, sizeof(command), stdin) != NULL) {
            command[strcspn(command, "\n")] = 0; // Remove newline character
            if (strcmp(command, "EXIT") == 0) {
                break;
            }
            interpret_command(command);
        }
    }
    
    return 0;
}
