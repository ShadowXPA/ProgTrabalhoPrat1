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

#include "Person.h"

#define MAX_FILENAME 256

void prog_exiting() {
    printf("\n----------------------\nThe program is closing.\n----------------------\n");
}

int main(int argc, char *argv[], char **envp) {
    atexit(prog_exiting);
    initRandom();
    char filename[MAX_FILENAME];
    char filename2[MAX_FILENAME];
    Config sim_cfg;
    ListPlace *places = NULL;
    ListPerson *people = NULL;

    if (argc == 3) {
        snprintf(filename, MAX_FILENAME, "%s", argv[1]);
        snprintf(filename2, MAX_FILENAME, "%s", argv[2]);
    } else if (argc == 2) {
        snprintf(filename, MAX_FILENAME, "%s", argv[1]);
        // Ask for filename
        printf("Input the name of the persons file:\n > ");
        scanf("%[^\n]", filename2);
    } else {
        // Ask for filename
        printf("Input the name of the places file:\n > ");
        scanf("%[^\n]", filename);
        getchar();
        printf("Input the name of the persons file:\n > ");
        scanf("%[^\n]", filename2);
    }

    // Initialize places
    printf("\n\n Initializing...\n Reading places file...\n");
    places = init_places(filename);
    if (places == NULL) {
        printf("The places weren't initialized with success!\n");
        exit(-1);
    }
    print_places(places);

    // Initialize People
    printf("\n\n Reading peoples file...\n");
    people = init_people(filename2);
    if (people == NULL) {
        printf("The people weren't initialized with success!\n");
        free_places(places);
        exit(-1);
    }
    print_people(people);

    // Distribute the people and init configuration
    printf("\n\n Initializing the simulation configuration and distributing the people.\n");
    sim_cfg.days = 0;
    sim_cfg.capacity = calloc(places->size, sizeof(Config));
    if (sim_cfg.capacity == NULL) {
        printf("Couldn't allocate memory for auxiliary data.\n");
        free_places(places);
        free_people(people);
        exit(-1);
    }
    sim_cfg.max_capacity = get_max_capacity(places);
    distribute_people(people, &sim_cfg, places);
    print_cfg(&sim_cfg, places->size);

    // Start Simulation
    printf("\n\n Initializing simulation...\n");

    free_people(people);
    free_places(places);
    scanf("%~[^\n]");
    return (EXIT_SUCCESS);
}
