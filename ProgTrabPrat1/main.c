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

/* 
 * File:   main.c
 * Author: Pedro dos Santos Alves
 *
 * Created on April 2, 2020, 4:20 PM
 */

#include <string.h>
#include <locale.h>
#include "utils.h"
#include "Local.h"

#define MAX_FILE 256

void prog_exiting() {
    printf("\n----------------------\nO programa irá terminar.\n----------------------\n");
}

int main(int argc, char *argv[], char **envp) {
    setlocale(LC_ALL, "pt_PT");
    atexit(prog_exiting);
    initRandom();
    char filename[MAX_FILE];
    ListPlace *places = NULL;

    if (argc == 2) {
        snprintf(filename, strlen(argv[1]) + 1, "%s", argv[1]);
    } else {
        // Ask for filename
        printf("Introduza o nome da pasta:\n > ");
        scanf("%[^\n]", &filename);
    }

    places = init_list_place(filename);
    if (places == NULL) {
        printf("Os locais não foram inicializados com sucesso!\n");
        exit(-1);
    }

    free(places);
    scanf("%~[^\n]");
    return (EXIT_SUCCESS);
}

