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

#ifndef LOCAL_H
#define LOCAL_H

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

    ListPlace *init_list_place(const char *dirname);
    bool resize_list_place(ListPlace *list_place, const size_t new_size);
    void view_list_place(const ListPlace *list_place);
    bool evaluate_list_place(const ListPlace *list_place);
    bool _evaluate_list_id(const ListPlace *list_place);
    bool _evaluate_list_connection(const ListPlace *list_place);
    bool _evaluate_list_connection_2(const ListPlace *list_place);


#ifdef __cplusplus
}
#endif

#endif /* LOCAL_H */

