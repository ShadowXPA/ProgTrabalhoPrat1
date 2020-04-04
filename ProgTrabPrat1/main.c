/*
 * Simulação da Propagação de Vírus
 *
 * Trabalho realizado por: Pedro dos Santos Alves
 * Nº: 2019112789
 * Programação
 * Licenciatura em Engenharia Informática
 * Instituto Superior de Engenharia de Coimbra
 *
 */

#include <string.h>
#include "utils.h"
#include "Place.h"

#define MAX_FILENAME 256

void prog_exiting() {
    printf("\n----------------------\nThe program will close.\n----------------------\n");
}

int main(int argc, char *argv[], char **envp) {
    atexit(prog_exiting);
    initRandom();
    char filename[MAX_FILENAME];
    ListPlace *places = NULL;

    if (argc == 2) {
        snprintf(filename, strlen(argv[1]) + 1, "%s", argv[1]);
    } else {
        // Ask for filename
        printf("Input the name of the place file:\n > ");
        scanf("%[^\n]", &filename);
    }

    // Initialize places
    printf("\n\n Initializing...\n Reading places file...\n");
    places = init_list_place(filename);
    if (places == NULL) {
        printf("The places weren't initialized with success!\n");
        exit(-1);
    }
    view_list_place(places);

    // Initialize People

    // Simulation
    printf("\n\n Initializing simulation...\n");
    free(places->place);
    free(places);
    scanf("%~[^\n]");
    return (EXIT_SUCCESS);
}