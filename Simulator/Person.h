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

#ifndef PERSON_H
#define PERSON_H

#include "utils.h"
#include <string.h>
#include "Place.h"
#include "config.h"

#define MAX_PERSON_ID 30
#define BUFFER_SIZE 512
#define PERSON_MAX_AGE 130

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum status {
        HEALTHY = 'S',
        SICK = 'D',
        IMMUNE = 'I',
    } Status;

    typedef struct person {
        char id[MAX_PERSON_ID];
        uint8_t age;
        Status status;
        uint16_t days;
        Place *place;
    } Person;

    typedef struct arr_person {
        Person person;
        struct arr_person *next;
    } ListPerson;

    ListPerson *init_people(const char *filename);
    void print_people(ListPerson *people);
    void print_people___(ListPerson *people, FILE *stream, const char *title);
    bool add_person(ListPerson *people, const Person *person);
    void free_people(ListPerson *people);
    bool person_exists(ListPerson *people, const char *person_id);
    bool evaluate_person(const Person *person);

    ListPerson *distribute_people(ListPerson *people, Config *cfg, const ListPlace *places);
    int32_t get_total___(ListPerson *people, const Status HIS);
    int32_t get_total_healthy(ListPerson *people);
    int32_t get_total_immune(ListPerson *people);
    int32_t get_total_sick(ListPerson *people);
    int32_t get_total_people_room(ListPerson *people, const Place *place);
    int32_t get_total_sick_room(ListPerson *people, const Place *place);
    Person **get_people_room(ListPerson *people, const Place *place, const int32_t size);

    int get_avg_age_room(ListPerson *people, const Place *place);
    
#ifdef __cplusplus
}
#endif

#endif /* PERSON_H */
