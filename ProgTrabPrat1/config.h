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

#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <stdbool.h>

/* Spread rate: rate at which the virus will spread
 */
#define SPREAD_RATE 0.05f
/* Probability of recovery: chance of recovering on each iteration
 */
#define PROB_RECOVERY(age) ((float)(1.0f/age))
/* Maximum duration of infection: number of days (iterations)
 * in which a person is sick
 */
#define INF_MAX_DUR(age) (5+((int)(age/10)))
/* Immunity rate: chance to become immune after maximum duration
 * of infection has passed or recovery rate
 */
#define IMMUNITY_RATE 0.2f

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct config {
        int32_t *capacity;
        uint32_t max_capacity;
        uint32_t real_capacity;
        uint16_t days;
        bool console_mode;
        uint16_t day_0_s;
        uint16_t day_peak;
        uint32_t peak_sick;
    } Config;

    uint32_t get_real_capacity(Config *cfg, uint32_t size);
    void print_cfg(Config *cfg, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif /* CONFIG_H */
