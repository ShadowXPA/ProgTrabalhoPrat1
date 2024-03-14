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

#ifndef PLACE_H
#define PLACE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_CONNECTIONS 3

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct place {
        int32_t id;
        int32_t capacity;
        int32_t connection[MAX_CONNECTIONS];
    } Place;

    typedef struct arr_place {
        Place *place;
        uint32_t size;
    } ListPlace;

    ListPlace *init_places(const char *dirname);
    bool resize_places(ListPlace *places, const size_t new_size);
    void print_places(const ListPlace *places);
    void print_places___(const ListPlace *places, FILE *stream, const char *title);
    void free_places(ListPlace *places);
    bool evaluate_places(const ListPlace *places);
    bool _evaluate_list_id(const ListPlace *places);
    bool _evaluate_list_connection(const ListPlace *places);
    bool _evaluate_list_connection_2(const ListPlace *places);
    uint32_t get_max_capacity(const ListPlace *places);
    int32_t get_place_by_id(ListPlace *places, const int32_t id, Place **place);

#ifdef __cplusplus
}
#endif

#endif /* PLACE_H */
