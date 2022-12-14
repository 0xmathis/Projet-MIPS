// Projet MIPS 2022-2023
// Auteurs: Cocagne_Strainovic

#include <stdio.h>
#include "fonctionModes.h"
#include "utils.h"
#include <stdlib.h>
#include <string.h>

// TODO: implémenter la distinction entre les mnémoniques de registres et les registres
// TODO: ranger les fonctions dans des fichiers spécifiques
// TODO: verifier que les erreurs soient bien checkées pour le nouveau code
// TODO: implémenter immédiats en hex
// TODO: bug programme si un caractere n'est ni une lettre, ni un nombre, ni un $ ou une ,

int main(int argc, char **argv) {
    int registers[32] = {0};

    switch (argc) {
        case 1:
            printf("* * * * * * * * * * * * Mode Interactif * * * * * * * * * * * *\n\n");
            modeInteractif(registers);
            break;
        case 3:
            printf("* * * * * * * * * * * *  Mode Pas a pas * * * * * * * * * * *\n");
            printf("Appuyez sur [ENTRER] pour passer à l'instruction suivante\n");
            printf("Appuyez sur n'importe quelle touche pour quitter l'émulateur\n\n");
            modeAutoPasAPas(argv[1], registers);
            break;
        case 4:
            printf("* * * * * * * * * * * * Mode Automatique * * * * * * * * * * * *\n\n");
            modeAuto(argv[1], argv[2], argv[3], registers);
            break;
        default:
            break;
    }

    printf("Bye émulateur MIPS!\n");

    return 0;
}
