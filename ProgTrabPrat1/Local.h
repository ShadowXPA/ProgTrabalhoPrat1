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
 * File:   Local.h
 * Author: Pedro dos Santos Alves
 *
 * Created on April 3, 2020, 5:08 PM
 */

#ifndef LOCAL_H
#define LOCAL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct place {
        int32_t id;
        int32_t capacity;
        int32_t connection[3];
    } Place;

    typedef struct arr_place {
        Place *place;
        uint32_t size;
    } ListPlace;

    ListPlace *init_list_place(const char *dirname);
    bool resize_list_place(ListPlace *list_place, size_t new_size);


#ifdef __cplusplus
}
#endif

#endif /* LOCAL_H */

