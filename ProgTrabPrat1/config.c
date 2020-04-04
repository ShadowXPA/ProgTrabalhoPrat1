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

#include <stdio.h>
#include <stdint.h>
#include "config.h"

uint32_t get_real_capacity(Config *cfg, uint32_t size) {
    uint32_t tmp = 0;

    for (uint32_t i = 0; i < size; i++) {
        tmp += cfg->capacity[i];
    }

    return tmp;
}

void print_cfg(Config *cfg, uint32_t size) {
    printf("\n----------------------\n Simulation Configuration:\n----------------------\n");
    printf(" Day: %d\n Maximum capacity: %d\n Current capacity: %d\n",
            cfg->days, cfg->max_capacity, get_real_capacity(cfg, size));
    printf("----------------------\n End.\n----------------------\n");
}
