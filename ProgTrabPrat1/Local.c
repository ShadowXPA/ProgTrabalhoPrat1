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

#include "Local.h"

ListPlace *init_list_place(const char *filename) {
    ListPlace *tmp = calloc(1, sizeof(ListPlace));

    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Erro ao abrir o ficheiro.\n");
        exit(-1);
    }

    Place tmp_place;
    uint32_t i = 0;
    size_t read = 0;
    bool keep_reading = true;
    while (keep_reading) {
        if ((read = fread(&tmp_place, sizeof(Place), 1, fp)) == 0) {
            // EOF
            keep_reading = false;
        } else {
            if (!resize_list_place(tmp, (i + 1) * sizeof(Place))) {
                printf("Erro ao alocar memória para os locais!\n");
                exit(-1);
            }
            tmp->place[i] = tmp_place;
            tmp->size = ++i;//(i + 1);
            //i++;
        }
    }

    fclose(fp);
    return tmp;
}

bool resize_list_place(ListPlace *list_place, size_t new_size) {
    Place *tmp = NULL;

    tmp = realloc(list_place->place, new_size);
    if (tmp == NULL)
        return false;

    list_place->place = tmp;
    return true;
}