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

#include "Person.h"

ListPerson *init_people(const char *filename) {
    printf("\n Initializing List of people.\n");
    ListPerson *head = calloc(1, sizeof(ListPerson));
    if (head == NULL) {
        printf("Error allocating memory for people!\n");
        return NULL;
    }
    head->next = NULL;

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening the file '%s'.\n", filename);
        free(head);
        return NULL;
    }

    Person person;
    bool first_person = true;
    char buffer[BUFFER_SIZE];
    const char s[3] = " \n";
    char *token;
    while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
        token = strtok(buffer, s);
        int i = 0;
        bool has_data = false;
        while (token != NULL) {
            has_data = true;
            switch (i) {
                case 0:
                    // ID
                    snprintf(person.id, MAX_PERSON_ID, "%s", token);
                    person.days = 0;
                    person.place = NULL;
                    break;
                case 1:
                    // Age
                    person.age = atoi(token);
                    break;
                case 2:
                    // Status
                    person.status = token[0];
                    break;
                case 3:
                    // Days
                    person.days = atoi(token);
                    break;
                default:
                    break;
            }
            token = strtok(NULL, s);
            i++;
        }
        if (has_data) {
            if (first_person) {
                head->person = person;
                first_person = false;
            } else {
                if (!add_person(head, &person)) {
                    printf("Couldn't add person...\n");
                    return NULL;
                }
            }
        }
    }

    fclose(fp);
    return head;
}

void print_people(ListPerson *people) {
    ListPerson *current = people;

    printf("\n----------------------\n People:\n----------------------\n");
    while (current != NULL) {
        printf(" Person:\n  ID: '%s'\n  Age: %d\n  Status: '%c'\n  Days sick: %d\n\n", current->person.id,
                current->person.age, current->person.status, current->person.days);
        current = current->next;
    }
    printf("----------------------\n End.\n----------------------\n");
}

bool add_person(ListPerson *people, const Person *person) {
    if (person_exists(people, person->id)) {
        printf("Person already exists!\n");
        return false;
    }
    if (!evaluate_person(person)) {
        printf("Couldn't evaluate person...\n");
        return false;
    }
    ListPerson *current = people;

    while (current->next != NULL) {
        current = current->next;
    }

    current->next = malloc(sizeof(ListPerson));
    if (current->next == NULL) {
        printf("Couldn't allocate memory for next person!\n");
        return false;
    }
    current->next->person = *person;
    current->next->next = NULL;
    return true;
}

bool person_exists(ListPerson *people, const char *person_id) {
    ListPerson *current = people;

    while (current != NULL) {
        if (strcmp(current->person.id, person_id) == 0) {
            return true;
        }
        current = current->next;
    }

    return false;
}

bool evaluate_person(const Person *person) {
    // Since it's unsigned it doesn't go below 0
    // If it's higher than 130 it might be a fake age...
    // Guinness world record is 122 years
    if (person->age > 130) {
        printf("Age is higher than 130. Seems unbelievable...\n");
        return false;
    }
    // There is only 3 status for a person...
    // There is no deceased person...
    if (person->status != HEALTHY &&
            person->status != SICK &&
            person->status != IMMUNE) {
        printf("Couldn't evaluate person's status...\n");
        return false;
    }
    // If person is sick... Then then days should be at 1...
    if (person->status == SICK && person->days == 0) {
        printf("Person's status is 'sick', yet days is 0...\n");
        return false;
    }

    return true;
}

void free_people(ListPerson *people) {
    ListPerson *tmp;

    while (people != NULL) {
        tmp = people;
        people = people->next;
        free(tmp);
    }
}


// People + Places

void distribute_people(ListPerson *people, Config *cfg, const ListPlace *places) {
    ListPerson *current = people;

    while (current != NULL && get_real_capacity(cfg, places->size) < cfg->max_capacity) {
        uint16_t i = 0;
        do {
            i = (uint16_t)intUniformRnd(0, places->size - 1);
        } while (cfg->capacity[i] >= places->place[i].capacity);
        current->person.place = &places->place[i];
        cfg->capacity[i]++;
        current = current->next;
    }
}
