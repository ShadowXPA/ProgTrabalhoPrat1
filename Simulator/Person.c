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
    ListPerson *head = NULL;
    /*ListPerson *head = calloc(1, sizeof(ListPerson));
    if (head == NULL) {
        printf("Error allocating memory for people!\n");
        return NULL;
    }
    head->next = NULL;*/

    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening the file '%s'.\n", filename);
        return NULL;
    }

    Person person;
    bool first_person = true;
    char buffer[BUFFER_SIZE];
    const char s[4] = " \t\n";
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
                head = calloc(1, sizeof(ListPerson));
                if (head == NULL) {
                    printf("Error allocating memory for people!\n");
                    fclose(fp);
                    return NULL;
                }
                if (!evaluate_person(&person)) {
                    printf("Couldn't add person...\n");
                    free_people(head);
                    fclose(fp);
                    return NULL;
                }
                head->person = person;
                head->next = NULL;
                first_person = false;
            } else {
                if (!add_person(head, &person)) {
                    printf("Couldn't add person...\n");
                    free_people(head);
                    fclose(fp);
                    return NULL;
                }
            }
        }
    }

    fclose(fp);
    return head;
}

void print_people(ListPerson *people) {
    print_people___(people, stdout, "People");
}

void print_people___(ListPerson *people, FILE *stream, const char *title) {
    ListPerson *current = people;

    fprintf(stream, "\n----------------------\n %s:\n----------------------\n", title);
    while (current != NULL) {
        fprintf(stream , " Person:\n  ID: '%s'\n  Age: %d\n  Status: '%c'\n  Days sick: %d", current->person.id,
                current->person.age, current->person.status, current->person.days);
        if (current->person.place != NULL) {
            fprintf(stream, "\n    Place ID: %d\n\n", current->person.place->id);
        } else {
            fprintf(stream, "\n\n");
        }
        current = current->next;
    }
    fprintf(stream, "----------------------\n End.\n----------------------\n");
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
    if (strcmp(person->id, "") == 0) {
        printf("Person ID is empty...\n");
        return false;
    }
    if (person->age == 0) {
        printf("Age is 0. Age needs to be 1 or higher.\n");
        return false;
    }
    // Since it's unsigned it doesn't go below 0
    // If it's higher than 130 it might be a fake age...
    // Guinness world record is 122 years
    if (person->age > PERSON_MAX_AGE) {
        printf("Age is higher than %d. Seems unbelievable...\n", PERSON_MAX_AGE);
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

ListPerson *distribute_people(ListPerson *people, Config *cfg, const ListPlace *places) {
    ListPerson *current = people, *simulation_people = people;

    while (current != NULL && get_real_capacity(cfg, places->size) < cfg->max_capacity) {
        uint16_t i = 0;
        do {
            i = (uint16_t)intUniformRnd(0, places->size - 1);
        } while (cfg->capacity[i] >= places->place[i].capacity);
        current->person.place = &places->place[i];
        cfg->capacity[i]++;
        simulation_people = current;
        current = current->next;
    }
    simulation_people->next = NULL;
    cfg->real_capacity = get_real_capacity(cfg, places->size);
    cfg->day_0_s = UINT16_MAX;
    cfg->day_peak = 0;
    cfg->peak_sick = 0;

    return current;
}

int32_t get_total___(ListPerson *people, const Status HIS) {
    int32_t sum = 0;
    ListPerson *current = people;

    while (current != NULL) {
        if (current->person.status == HIS)
            sum++;
        current = current->next;
    }

    return sum;
}

int32_t get_total_healthy(ListPerson *people) {
    return get_total___(people, HEALTHY);
}

int32_t get_total_immune(ListPerson *people) {
    return get_total___(people, IMMUNE);
}

int32_t get_total_sick(ListPerson *people) {
    return get_total___(people, SICK);
}

int32_t get_total_people_room(ListPerson *people, const Place *place) {
    int32_t sum = 0;
    ListPerson *current = people;

    while (current != NULL) {
        if (current->person.place == place) {
            sum++;
        }
        current = current->next;
    }

    return sum;
}

int32_t get_total_sick_room(ListPerson *people, const Place *place) {
    int32_t sum = 0;
    ListPerson *current = people;

    while (current != NULL) {
        if (current->person.place == place && current->person.status == SICK) {
            sum++;
        }
        current = current->next;
    }

    return sum;
}

int get_avg_age_room(ListPerson *people, const Place *place) {
    int sum = 0, total = 0;
    ListPerson *current = people;   
    
    while (current != NULL) {
        if (current->person.place == place) {
            sum += current->person.age;
            total++;
        }
        current = current->next;
    }

    if (!total)
        return 0;

    return sum/total;
}

Person **get_people_room(ListPerson *people, const Place *place, const int32_t size) {
    Person **tmp = malloc(size * sizeof(Person*));
    ListPerson *current = people;
    int32_t i = 0;
    if (tmp == NULL) {
        return NULL;
    }

    while (current != NULL) {
        if (current->person.place == place) {
            tmp[i] = &current->person;
            i++;
        }
        current = current->next;
    }

    return tmp;
}
