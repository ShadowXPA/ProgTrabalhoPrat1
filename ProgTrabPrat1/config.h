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

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct config {
        int32_t *capacity;
        uint32_t max_capacity;
        uint16_t days;
    } Config;

    uint32_t get_real_capacity(Config *cfg, uint32_t size);
    void print_cfg(Config *cfg, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif /* CONFIG_H */
