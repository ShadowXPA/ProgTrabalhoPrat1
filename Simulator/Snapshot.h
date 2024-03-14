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

#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include "Person.h"
/* Number of maximum iterations to save
 */
#define MAX_SAVED_SNAPSHOTS 3

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct snap {
        Config cfg;
        ListPerson *people;
        struct snap *prev;
        struct snap *next;
    } Snapshot;

    uint32_t get_num_snapshot(Snapshot *snap);
    void save_snapshot(Snapshot **snap, Config *cfg, ListPerson *people, ListPlace *places);
    void load_snapshot(Snapshot **snap, uint32_t num_iterations, ListPerson **people, Config *cfg);
    void free_snapshot(Snapshot *snap);
    void free_snapshot_reverse(Snapshot *snap);

#ifdef __cplusplus
}
#endif

#endif /* SNAPSHOT_H */

