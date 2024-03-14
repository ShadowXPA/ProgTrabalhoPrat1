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

#include "Snapshot.h"

uint32_t get_num_snapshot(Snapshot *snap) {
    uint32_t tmp = 0;
    Snapshot *aux = snap;

    while (aux != NULL) {
        tmp++;
        aux = aux->next;
    }

    return tmp;
}

void save_snapshot(Snapshot **first, Config *cfg, ListPerson *people, ListPlace *places) {
    if (MAX_SAVED_SNAPSHOTS > 0) {
        Snapshot *tmp = *first, *last = NULL;
        ListPerson *tmp_p = people, *tmp_pF = NULL, *tmp_pI = NULL, *tmp_pA = NULL;
        uint32_t N = 0;
        // No need to get number of snapshots if we're going to loop through anyway
        //uint32_t N = get_num_snapshots(*snap);
        while (tmp != NULL) {
            N++;
            last = tmp;
            tmp = tmp->next;
        }

        tmp = malloc(sizeof (Snapshot));
        // No space, no snapshot
        if (tmp == NULL)
            return;
        tmp->prev = NULL;
        tmp->next = NULL;
        tmp->cfg.capacity = NULL;
        tmp->people = NULL;

        // Copy from one side to another
        // Copy config struct
        tmp->cfg = *cfg;
        // We don't want capacity with the same address so allocate memory
        tmp->cfg.capacity = malloc(places->size * sizeof (int32_t));
        // No space, no snapshot
        if (tmp->cfg.capacity == NULL) {
            free_snapshot(tmp);
            return;
        }
        // Copy from one array to another
        memcpy(tmp->cfg.capacity, cfg->capacity, places->size * sizeof (int32_t));

        while (tmp_p != NULL) {
            tmp_pI = malloc(sizeof (ListPerson));
            if (tmp_pI == NULL) {
                free_people(tmp_pF);
                free_snapshot(tmp);
                return;
            }
            if (tmp_pF == NULL) {
                tmp_pF = tmp_pI;
            }
            *tmp_pI = *tmp_p;
            tmp_pI->next = NULL;
            if (tmp_pA != NULL)
                tmp_pA->next = tmp_pI;

            tmp_pA = tmp_pI;
            tmp_pI = tmp_pI->next;
            tmp_p = tmp_p->next;
        }
        tmp->people = tmp_pF;

        // If snap is NULL, tmp->next will be NULL as well...
        if (*first != NULL) {
            (*first)->prev = tmp;
        }
        tmp->next = (*first);
        *first = tmp;

        // Free last one if maximum iterations saved hits the threshold
        if (N == MAX_SAVED_SNAPSHOTS) {
            if (last->prev != NULL)
                last->prev->next = NULL;
            free_snapshot(last);
        }
    }
}

void load_snapshot(Snapshot **snap, uint32_t num_iterations, ListPerson **people, Config *cfg) {
    uint32_t i = 0;
    Snapshot *tmp = *snap, *load = NULL;
    ListPerson *tmp_P = NULL;

    while (tmp != NULL && i < num_iterations) {
        i++;
        load = tmp;
        tmp = tmp->next;
    }

    if (load != NULL) {
        // Load configuration
        free(cfg->capacity);
        *cfg = load->cfg;

        // Load people
        tmp_P = *people;
        *people = load->people;
        free_people(tmp_P);

        // Free previous
        free_snapshot_reverse(load->prev);
        free(load);
        if (tmp != NULL)
            tmp->prev = NULL;
        *snap = tmp;
    }
}

void free_snapshot(Snapshot *snap) {
    Snapshot *tmp;

    while (snap != NULL) {
        tmp = snap;
        snap = snap->next;
        free_people(tmp->people);
        free(tmp->cfg.capacity);
        free(tmp);
    }
}

void free_snapshot_reverse(Snapshot *snap) {
    Snapshot *tmp;

    while (snap != NULL) {
        tmp = snap;
        snap = snap->prev;
        free_people(tmp->people);
        free(tmp->cfg.capacity);
        free(tmp);
    }
}
