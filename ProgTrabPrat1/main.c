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

#define MAX_FILENAME 256

bool simulating(ListPlace *places, Config *cfg, ListPerson **people, ListPerson *discarded_people, Snapshot **snap);
void show_min_stats(ListPlace *places, Config *cfg, ListPerson *people);
void show_max_stats(ListPlace *places, Config *cfg, ListPerson *people);
void show_max_stats___(ListPlace *places, Config *cfg, ListPerson *people, FILE *stream, const char *title);
void show_bar(char *buf, int32_t percent);
uint8_t print_menu();
uint8_t console_mode(ListPlace *places, Config *cfg, ListPerson **people, Snapshot **snap);
void generate_reports(ListPlace *places, Config *cfg, ListPerson *people, ListPerson *discarded_people);
void show_cmds();
void step(ListPlace *places, Config *cfg, ListPerson *people);
void spread_virus(ListPlace *places, ListPerson *people);
void probability_recovery(ListPerson *people, Config *cfg);
//void evaluate_if_recovered(ListPerson *people);
void adding_people(ListPlace *places, Config *cfg, ListPerson *people);
void transfering_people(ListPlace *places, Config *cfg, ListPerson *people);

bool try_add_person(ListPlace *places, Config *cfg, ListPerson *people, Person *p, const int32_t place_id);
bool try_move_people(ListPlace *places, Config *cfg, ListPerson *people, const int32_t N, const int32_t place_id_src, const int32_t place_id_dest);

void prog_exiting() {
    printf("\n----------------------\nThe program is closing.\n----------------------\n");
}

int main(int argc, char *argv[], char **envp) {
    atexit(prog_exiting);
    initRandom();
    char filename[MAX_FILENAME];
    char filename2[MAX_FILENAME];
    Config sim_cfg;
    ListPlace *places = NULL;
    ListPerson *people = NULL;
    Snapshot *snap = NULL;

    // Get filenames
    if (argc == 3) {
        snprintf(filename, MAX_FILENAME, "%s", argv[1]);
        snprintf(filename2, MAX_FILENAME, "%s", argv[2]);
    } else if (argc == 2) {
        snprintf(filename, MAX_FILENAME, "%s", argv[1]);
        // Ask for filename
        printf("Input the name of the persons file:\n > ");
        scanf("%[^\n]", filename2);
        getchar();
    } else {
        // Ask for filename
        printf("Input the name of the places file:\n > ");
        scanf("%[^\n]", filename);
        getchar();
        printf("Input the name of the persons file:\n > ");
        scanf("%[^\n]", filename2);
        getchar();
    }

    // Initialize places
    printf("\n\n Initializing...\n Reading places file...\n");
    places = init_places(filename);
    if (places == NULL) {
        printf("The places weren't initialized with success!\n");
        exit(-1);
    }
    print_places(places);

    // Initialize People
    printf("\n\n Reading peoples file...\n");
    people = init_people(filename2);
    if (people == NULL) {
        printf("The people weren't initialized with success!\n");
        free_places(places);
        exit(-1);
    }
    //print_people(people);

    // Distribute the people and init configuration
    printf("\n\n Initializing the simulation configuration and distributing the people.\n");
    sim_cfg.days = 0;
    sim_cfg.console_mode = false;
    sim_cfg.capacity = calloc(places->size, sizeof (int32_t));
    if (sim_cfg.capacity == NULL) {
        printf("Couldn't allocate memory for auxiliary data.\n");
        free_places(places);
        free_people(people);
        exit(-1);
    }
    sim_cfg.max_capacity = get_max_capacity(places);
    ListPerson *people_discarded = distribute_people(people, &sim_cfg, places);
    print_cfg(&sim_cfg, places->size);
    if (people_discarded == people) {
        people_discarded = NULL;
    }
    printf("\n\n People who belong in the simulation:\n");
    print_people(people);
    if (people_discarded != NULL) {
        printf("\n\n People who DO NOT belong in the simulation:\n");
        print_people(people_discarded);
    }

    // Start Simulation
    printf("\n\n Initializing simulation...\n");
    printf("\n\n\n----------------------\n  Simulation start!\n----------------------\n");
    while (simulating(places, &sim_cfg, &people, people_discarded, &snap));

    free(sim_cfg.capacity);
    free_places(places);
    free_people(people);
    free_people(people_discarded);
    free_snapshot(snap);
    return (EXIT_SUCCESS);
}

bool simulating(ListPlace *places, Config *cfg, ListPerson **people, ListPerson *discarded_people, Snapshot **snap) {
    uint8_t op = 0;
    uint32_t N = 0;
    char buffer[BUFFER_SIZE];

    // Show minimal statistics
    show_min_stats(places, cfg, *people);
    // Show menu
    if (cfg->console_mode) {
        op = console_mode(places, cfg, people, snap);
    } else {
        op = print_menu();
    }

    switch (op) {
        case 8:
            return false;
        case 7:
            // Generate reports
            generate_reports(places, cfg, *people, discarded_people);
            return false;
        case 1:
            // Save Snapshot before iterating
            save_snapshot(snap, cfg, *people, places);
            step(places, cfg, *people);
            break;
        case 2: {
            uint32_t S = get_num_snapshot(*snap);
            if (S > 0) {
                printf(" Insert number of snapshots to go back (between 1 and %d):\n > ", S);
                scanf("%[^\n]", buffer);
                getchar();
                N = atoi(buffer);
                if (N > 0 && N <= S)
                    load_snapshot(snap, N, people, cfg);
                else
                    printf(" Invalid number of snapshots...\n");
            } else {
                printf(" No snapshots yet...\n");
            }
            break;
        }
        case 3:
            show_max_stats(places, cfg, *people);
            break;
        case 4:
            // Sub menu
            // Add person
            printf("\n");
            adding_people(places, cfg, *people);
            break;
        case 5:
            // Sub menu
            // Transfer people
            printf("\n");
            transfering_people(places, cfg, *people);
            break;
        case 6:
            // Toggle console mode
            cfg->console_mode = !cfg->console_mode;
            break;
        case 22:
            // ignore
            break;
        default:
            break;
    }

    return true;
}

void generate_reports(ListPlace *places, Config *cfg, ListPerson *people, ListPerson *discarded_people) {
    char filename[MAX_FILENAME];
    FILE *rp = NULL, *peo = NULL;
    printf("\n\n\n----------------------\n  Generating reports\n----------------------\n");

    printf(" Input report filename:\n > ");
    scanf("%[^\n]", filename);
    getchar();
    peo = fopen(filename, "w");
    if (peo == NULL) {
        printf(" Could not open '%s'...\n Report not created!\n", filename);
    } else {
        ListPerson *current = people;

        while (current != NULL) {
            fprintf(peo, "%s %d %c", current->person.id, REAL_AGE(current->person.age, cfg->days), current->person.status);
            if (current->person.status == 'D')
                fprintf(peo, " %d", current->person.days);
            fprintf(peo, "\n");
            current = current->next;
        }
        //        print_people___(people, peo, "People who were part of the simulation");
        //        print_people___(discarded_people, peo, "People who were not part of the simulation");
        fclose(peo);
    }
    time_t unixT = time(NULL);
    rp = fopen(REPORT_FILENAME, "a");
    if (rp == NULL) {
        printf(" Could not open '%s'...\n Report not created!\n", REPORT_FILENAME);
        return;
    }
    fprintf(rp, "----------------------\n  Report ID: %li\n----------------------\n", unixT);
    print_people___(people, rp, "People who took part in the simulation");
    print_people___(discarded_people, rp, "People who didn't take part in the simulation");
    show_max_stats___(places, cfg, people, rp, "Full statistics");
    fprintf(rp, "\n----------------------\n  End of Report ID: %li\n----------------------\n\n", unixT);
    fclose(rp);

    printf("\n----------------------\n  Reports generated\n----------------------\n");
}

void adding_people(ListPlace *places, Config *cfg, ListPerson *people) {
    bool adding_people = true, try_add = false;
    Person p;
    snprintf(p.id, MAX_PERSON_ID, "");
    p.days = 0;
    p.place = NULL;
    p.age = 0;
    p.status = SICK;
    int32_t place_id = -1;
    uint8_t op = 0;
    char buffer[BUFFER_SIZE], *token;
    const char s[4] = " \t\n";

    do {
        printf(" (1) - Place ID: %d\n", place_id);
        printf(" (2) - Person ID: '%s'\n", p.id);
        printf(" (3) - Person Age: %hhu\n", p.age);
        printf(" (4) - Days sick: %hu\n", p.days);
        printf(" (5) - Confirm\n");
        printf(" (6) - Cancel\n\n");
        do {
            printf(" > ");
            scanf("%[^\n]", buffer);
            getchar();
            op = atoi(buffer);
        } while (op < 1 || op > 6);
        switch (op) {
            case 1:
                // Ask for Place ID
                printf("\n Input Place ID:\n > ");
                scanf("%[^\n]", buffer);
                getchar();
                place_id = atoi(buffer);
                break;
            case 2:
                // Ask for Person ID
                printf("\n Input Person ID:\n > ");
                scanf("%[^\n]", buffer);
                getchar();
                token = strtok(buffer, s);
                if (token != NULL) {
                    snprintf(p.id, MAX_PERSON_ID, "%s", token);
                }
                break;
            case 3:
                // Ask for Person Age
                printf("\n Input Person age:\n > ");
                scanf("%[^\n]", buffer);
                getchar();
                p.age = atoi(buffer);
                break;
            case 4:
                // Ask for Days sick
                printf("\n Input Person sick days:\n > ");
                scanf("%[^\n]", buffer);
                getchar();
                p.days = atoi(buffer);
                break;
            case 5:
                // Confirm and add people
                adding_people = false;
                try_add = true;
                break;
            case 6:
                adding_people = false;
                try_add = false;
                break;
            default:
                break;
        }
    } while (adding_people);

    if (try_add) {
        // Try adding person
        if (try_add_person(places, cfg, people, &p, place_id)) {
            printf(" Person '%s' added.\n", p.id);
        } else {
            printf(" Person '%s' NOT added.\n", p.id);
        }
    }
}

void transfering_people(ListPlace *places, Config *cfg, ListPerson *people) {
    bool transfering_people = true, try_move = false;
    int32_t N = -1;
    int32_t place_id_src = -1;
    int32_t place_id_dest = -1;
    uint8_t op = 0;
    char buffer[BUFFER_SIZE];

    do {
        printf(" (1) - Number of people to move: %d\n", N);
        printf(" (2) - Source Place ID: %d\n", place_id_src);
        printf(" (3) - Destination Place ID: %d\n", place_id_dest);
        printf(" (4) - Confirm\n");
        printf(" (5) - Cancel\n\n");
        do {
            printf(" > ");
            scanf("%[^\n]", buffer);
            getchar();
            op = atoi(buffer);
        } while (op < 1 || op > 6);
        switch (op) {
            case 1:
                // Ask for Place ID
                printf("\n Input number of people:\n > ");
                scanf("%[^\n]", buffer);
                getchar();
                N = atoi(buffer);
                break;
            case 2:
                // Ask for source place ID
                printf("\n Input source place ID:\n > ");
                scanf("%[^\n]", buffer);
                getchar();
                place_id_src = atoi(buffer);
                break;
            case 3:
                // Ask for destination place ID
                printf("\n Input destination place ID:\n > ");
                scanf("%[^\n]", buffer);
                getchar();
                place_id_dest = atoi(buffer);
                break;
            case 4:
                // Confirm and move people
                transfering_people = false;
                try_move = true;
                break;
            case 5:
                // Cancel
                transfering_people = false;
                try_move = false;
                break;
            default:
                break;
        }
    } while (transfering_people);

    if (try_move) {
        // Try adding person
        if (try_move_people(places, cfg, people, N, place_id_src, place_id_dest)) {
            printf(" %d %s transferred from %d to %d.\n", N, N == 1 ? "person was" : "people were", place_id_src, place_id_dest);
        } else {
            printf(" No people were transferred.\n");
        }
    }
}

void show_min_stats(ListPlace *places, Config *cfg, ListPerson *people) {
    printf("\n----------------------\n Simulation stats\n----------------------\n");
    printf("  Day: %hu\n", cfg->days);
    printf("  Number of places: %d\n", places->size);
    printf("  Maximum capacity: %d\n", cfg->max_capacity);
    printf("  Number of people: %d\n", cfg->real_capacity);
    int32_t total_h = get_total_healthy(people);
    int32_t total_i = get_total_immune(people);
    int32_t total_s = get_total_sick(people);
    float percent_h = (((float) total_h) / cfg->real_capacity)*100.0f;
    float percent_i = (((float) total_i) / cfg->real_capacity)*100.0f;
    float percent_s = (((float) total_s) / cfg->real_capacity)*100.0f;
    char buffer[23];
    show_bar(buffer, percent_h);
    printf("  Number of healthy people: %d\t(%.2f%%) \t%s\n", total_h, percent_h, buffer);
    show_bar(buffer, percent_i);
    printf("  Number of immune people: %d\t(%.2f%%) \t%s\n", total_i, percent_i, buffer);
    show_bar(buffer, percent_s);
    printf("  Number of sick people: %d\t(%.2f%%) \t%s\n", total_s, percent_s, buffer);
    if (cfg->day_0_s == UINT16_MAX && total_s == 0) {
        cfg->day_0_s = cfg->days;
    } else if (cfg->day_0_s != UINT16_MAX && total_s != 0) {
        cfg->day_0_s = UINT16_MAX;
    }
    printf("  Day of virus extinction: %d\n", cfg->day_0_s == UINT16_MAX ? -1 : cfg->day_0_s);
    if (cfg->peak_sick < total_s) {
        cfg->peak_sick = total_s;
        cfg->day_peak = cfg->days;
    }
    printf("  Peak number of people sick: %d\n", cfg->peak_sick);
    printf("  Day of peak: %hu\n", cfg->day_peak);
    printf("----------------------\n End\n----------------------\n\n");
}

void show_max_stats(ListPlace *places, Config *cfg, ListPerson *people) {
    show_max_stats___(places, cfg, people, stdout, "Simulation stats");
}

void show_max_stats___(ListPlace *places, Config *cfg, ListPerson *people, FILE *stream, const char *title) {
    fprintf(stream, "\n----------------------\n %s\n----------------------\n", title);
    fprintf(stream, "  Day: %hu\n", cfg->days);
    fprintf(stream, "  Number of places: %d\n", places->size);
    fprintf(stream, "  Maximum capacity: %d\n", cfg->max_capacity);
    fprintf(stream, "  Number of people: %d\n", cfg->real_capacity);
    int32_t total_h = get_total_healthy(people);
    int32_t total_i = get_total_immune(people);
    int32_t total_s = get_total_sick(people);
    float percent_h = (((float) total_h) / cfg->real_capacity)*100.0f;
    float percent_i = (((float) total_i) / cfg->real_capacity)*100.0f;
    float percent_s = (((float) total_s) / cfg->real_capacity)*100.0f;
    char buffer[23];
    show_bar(buffer, percent_h);
    fprintf(stream, "  Number of healthy people: %d\t(%.2f%%) \t%s\n", total_h, percent_h, buffer);
    show_bar(buffer, percent_i);
    fprintf(stream, "  Number of immune people: %d\t(%.2f%%) \t%s\n", total_i, percent_i, buffer);
    show_bar(buffer, percent_s);
    fprintf(stream, "  Number of sick people: %d\t(%.2f%%) \t%s\n", total_s, percent_s, buffer);
    fprintf(stream, "  Day of virus extinction: %d\n", cfg->day_0_s == UINT16_MAX ? -1 : cfg->day_0_s);
    fprintf(stream, "  Peak number of people sick: %d\n", cfg->peak_sick);
    fprintf(stream, "  Day of peak: %hu\n", cfg->day_peak);
    print_places___(places, stream, "Places");
    fprintf(stream, "\n");

    for (int i = 0; i < places->size; i++) {
        int32_t total_place = get_total_people_room(people, &places->place[i]);
        int32_t total_sick = get_total_sick_room(people, &places->place[i]);
        int32_t people_to_infect = (int32_t) ((total_place * SPREAD_RATE) * total_sick);
        people_to_infect = (people_to_infect > total_place) ? total_place : people_to_infect;
        fprintf(stream, "  Place: %d\n", places->place[i].id);
        fprintf(stream, "   Total people: %d\n", total_place);
        fprintf(stream, "   Total people sick: %d\n", total_sick);
        fprintf(stream, "   Number of people who could get infected: %d\n", people_to_infect);
        Person **bucket_people = get_people_room(people, &places->place[i], total_place);
        for (int j = 0; j < total_place && bucket_people != NULL; j++) {
            fprintf(stream, "    Person: '%s' - Age: %d - Status: '%c' - Days since infected: %d\n",
                    bucket_people[j]->id, REAL_AGE(bucket_people[j]->age, cfg->days), bucket_people[j]->status, bucket_people[j]->days);
        }
        fprintf(stream, "\n");
        free(bucket_people);
    }
    fprintf(stream, "----------------------\n End\n----------------------\n");
}

void show_bar(char *buf, int32_t percent) {
    char buffer[23];
    for (int i = 0; i < 23; i++)
        buffer[i] = ' ';
    int32_t i_l = (percent * 20) / 100;

    buffer[0] = '[';
    for (int i = 1; i < i_l + 1; i++) {
        buffer[i] = '|';
    }
    buffer[21] = ']';
    buffer[22] = '\0';

    snprintf(buf, 23, "%s", buffer);
    //strncpy(buf, buffer, 23);
}

uint8_t print_menu() {
    uint8_t op = 0;
    char buffer[BUFFER_SIZE];
    printf(" (1) - Next day (Advance 1 iteration)\n");
    printf(" (2) - Revert (Go back N iterations)\n");
    printf(" (3) - Show full statistics\n");
    printf(" (4) - Add sick person\n");
    printf(" (5) - Transfer people\n");
    printf(" (6) - Switch (Console mode)\n");
    printf(" (7) - End simulation\n");
    printf(" (8) - Interrupt (End simulation without report)\n\n");
    do {
        printf(" > ");
        scanf("%[^\n]", buffer);
        getchar();
        op = atoi(buffer);
    } while (op < 1 || op > 8);

    return op;
}

uint8_t console_mode(ListPlace *places, Config *cfg, ListPerson **people, Snapshot **snap) {
    uint8_t op = 0;
    char buffer[BUFFER_SIZE], *token;
    const char s[4] = " \t\n";
    printf(" Type 'help' for a list of commands.\n");
    do {
        printf(" [CMD_MODE] > ");
        scanf("%[^\n]", buffer);
        getchar();
        token = strtok(buffer, s);
        if (token != NULL) {
            if (strncmp("help", token, BUFFER_SIZE) == 0) {
                show_cmds();
            } else if (strncmp("step", token, BUFFER_SIZE) == 0 ||
                    strncmp("next", token, BUFFER_SIZE) == 0) {
                op = 1;
            } else if (strncmp("show", token, BUFFER_SIZE) == 0 ||
                    strncmp("stats", token, BUFFER_SIZE) == 0) {
                op = 3;
            } else if (strncmp("switch", token, BUFFER_SIZE) == 0 ||
                    strncmp("console", token, BUFFER_SIZE) == 0 ||
                    strncmp("cmd", token, BUFFER_SIZE) == 0) {
                op = 6;
            } else if (strncmp("end", token, BUFFER_SIZE) == 0 ||
                    strncmp("terminate", token, BUFFER_SIZE) == 0 ||
                    strncmp("exit", token, BUFFER_SIZE) == 0) {
                op = 7;
            } else if (strncmp("kill", token, BUFFER_SIZE) == 0 ||
                    strncmp("int", token, BUFFER_SIZE) == 0 ||
                    strncmp("interrupt", token, BUFFER_SIZE) == 0) {
                op = 8;
            } else if (strncmp("add", token, BUFFER_SIZE) == 0) {
                bool completed = false;
                token = strtok(NULL, s);
                if (token != NULL) {
                    int32_t place_id = atoi(token);
                    token = strtok(NULL, s);
                    if (token != NULL) {
                        char person_id[MAX_PERSON_ID];
                        snprintf(person_id, MAX_PERSON_ID, "%s", token);
                        token = strtok(NULL, s);
                        if (token != NULL) {
                            uint8_t person_age = atoi(token);
                            token = strtok(NULL, s);
                            if (token != NULL) {
                                uint16_t person_days = atoi(token);
                                Person p;
                                snprintf(p.id, MAX_PERSON_ID, "%s", person_id);
                                p.age = person_age;
                                p.status = SICK;
                                p.days = person_days;
                                p.place = NULL;
                                if (try_add_person(places, cfg, *people, &p, place_id)) {
                                    printf(" Person '%s' added.\n", p.id);
                                    completed = true;
                                    return 22;
                                }
                            }
                        }
                    }
                }
                if (!completed) {
                    printf(" Error completing command...\n Make sure you are typing the command correctly!\n");
                }
            } else if (strncmp("move", token, BUFFER_SIZE) == 0) {
                bool completed = false;
                token = strtok(NULL, s);
                if (token != NULL) {
                    int32_t N = atoi(token);
                    token = strtok(NULL, s);
                    if (token != NULL) {
                        int32_t place_id_src = atoi(token);
                        token = strtok(NULL, s);
                        if (token != NULL) {
                            int32_t place_id_dest = atoi(token);
                            if (try_move_people(places, cfg, *people, N, place_id_src, place_id_dest)) {
                                printf(" %d %s transferred from %d to %d.\n", N, N == 1 ? "person was" : "people were", place_id_src, place_id_dest);
                                completed = true;
                                return 22;
                            }
                        }
                    }
                }
                if (!completed) {
                    printf(" Error completing command...\n Make sure you are typing the command correctly!\n");
                }
            } else if (strncmp("undo", token, BUFFER_SIZE) == 0 ||
                    strncmp("prev", token, BUFFER_SIZE) == 0) {
                token = strtok(NULL, s);
                if (token != NULL) {
                    int32_t N = atoi(token);
                    uint32_t S = get_num_snapshot(*snap);
                    if (S > 0)
                        if (N > 0 && N <= S) {
                            load_snapshot(snap, N, people, cfg);
                            return 22;
                        } else {
                            printf(" Number of iterations invalid!\n");
                        }
                    else
                        printf(" Invalid number of iterations...\n");
                }
            } else {
                printf(" Unknown command...\n");
            }
        }
    } while (op < 1 || op > 8);

    return op;
}

bool try_add_person(ListPlace *places, Config *cfg, ListPerson *people, Person *p, const int32_t place_id) {
    if (place_id < 0) {
        printf(" There are no negative IDs...\n");
        return false;
    }
    Place *tmp = NULL;
    int32_t index = get_place_by_id(places, place_id, &tmp);
    if (tmp == NULL || index == -1) {
        printf(" Place does not exist!\n");
        return false;
    }
    if (cfg->capacity[index] >= tmp->capacity) {
        printf(" Capacity reached!\n");
        return false;
    }

    p->place = tmp;
    if (!add_person(people, p))
        return false;

    cfg->capacity[index]++;
    cfg->real_capacity++;

    return true;
}

bool try_move_people(ListPlace *places, Config *cfg, ListPerson *people, const int32_t N, const int32_t place_id_src, const int32_t place_id_dest) {
    if (N <= 0) {
        printf(" N needs to be positive to transfer people...\n");
        return false;
    }
    if (place_id_src < 0 || place_id_dest < 0) {
        printf(" There are no negative IDs...\n");
        return false;
    }
    Place *src = NULL, *dest = NULL;
    int32_t index_src = get_place_by_id(places, place_id_src, &src);
    int32_t index_dest = get_place_by_id(places, place_id_dest, &dest);
    if (src == NULL || dest == NULL || index_src == -1 || index_dest == -1) {
        printf(" One or all places do not exist!\n");
        return false;
    }
    bool is_connected = false;
    for (int i = 0; i < MAX_CONNECTIONS; i++) {
        if (src->connection[i] == dest->id) {
            is_connected = true;
            break;
        }
    }
    if (!is_connected) {
        printf(" Destination place is not connected to source place!\n");
        return false;
    }
    if (cfg->capacity[index_src] < N) {
        printf(" Source place doesn't have enough people to remove...\n");
        return false;
    }
    if (cfg->capacity[index_dest] + N > dest->capacity) {
        printf(" Destination place doesn't have enough space for %d %s.\n", N, N == 1 ? "person" : "people");
        return false;
    }

    int32_t total_place = get_total_people_room(people, &places->place[index_src]);
    Person **bucket_people = get_people_room(people, &places->place[index_src], total_place);
    for (int j = 0; j < N && bucket_people != NULL;) {
        int32_t R = intUniformRnd(0, total_place - 1);
        if (bucket_people[R]->place != dest) {
            bucket_people[R]->place = dest;
            j++;
        }
    }
    free(bucket_people);
    cfg->capacity[index_src] -= N;
    cfg->capacity[index_dest] += N;

    return true;
}

void show_cmds() {
    printf("Here's a list of all commands currently available:\n\n");
    printf("\tstep | next\n\t\t Advances 1 day (iteration).\n\n");
    printf("\tundo [N] | prev [N]\n\t\t Reverts [N] days (iterations).\n\n");
    printf("\tshow | stats\n\t\t Shows full list of statistics.\n\n");
    printf("\tadd [place_id] [person_id] [person_age] [days_sick]\n\t\t Add a new sick person.\n\n");
    printf("\tmove [N] [place_src] [place_dest]\n\t\t Moves [N] people around randomly as long as the [place_src] and [place_dest] are connected.\n\n");
    printf("\tswitch | console | cmd\n\t\t Toggles console mode.\n\n");
    printf("\tend | terminate | exit\n\t\t Ends the simulation.\n\n");
    printf("\tkill | int | interrupt\n\t\t Ends the simulation (without report).\n\n");
    printf("\thelp\n\t\t Shows this.\n\n");
}

// Simulation functions

void step(ListPlace *places, Config *cfg, ListPerson *people) {
    cfg->days++;

    // Spread virus
    spread_virus(places, people);
    // Probability of recovery
    probability_recovery(people, cfg);
    // Maximum duration of infection
    //evaluate_if_recovered(people);
}

void spread_virus(ListPlace *places, ListPerson *people) {
    for (int i = 0; i < places->size; i++) {
        // Get parameters
        int32_t total_room = get_total_people_room(people, &places->place[i]);
        int32_t total_sick = get_total_sick_room(people, &places->place[i]);
        int32_t people_to_infect = (int32_t) ((total_room * SPREAD_RATE) * total_sick);
        people_to_infect = (people_to_infect > total_room) ? total_room : people_to_infect;
        //printf("Total people in room ID (%d): '%d'\n", places->place[i].id, total_room);
        //printf("Total people sick in room ID (%d): '%d'\n", places->place[i].id, total_sick);
        //printf("Total people to infect in room ID (%d): '%d'\n\n", places->place[i].id, people_to_infect);
        // Start infecting people!
        Person **bucket_people = get_people_room(people, &places->place[i], total_room);
        //printf("Room: %d\n", places->place[i].id);
        for (int j = 0; j < people_to_infect && bucket_people != NULL; j++) {
            int32_t R = intUniformRnd(0, total_room - 1);
            if (bucket_people[R]->status != IMMUNE)
                bucket_people[R]->status = SICK;
        }
        /*for (int j = 0; j < total_room && bucket_people != NULL; j++) {
            printf("Person: '%s'  -  Status: '%c'\n", bucket_people[j]->id, bucket_people[j]->status);
        }
        printf("\n");*/
        free(bucket_people);
    }
    //print_people(people);
}

void probability_recovery(ListPerson *people, Config *cfg) {
    ListPerson *current = people;

    while (current != NULL) {
        if (current->person.status == SICK) {
            // Calculate probability!
            if (probEvento(PROB_RECOVERY(REAL_AGE(current->person.age, cfg->days)))) {
                current->person.days = INF_MAX_DUR(REAL_AGE(current->person.age, cfg->days));
            }

            // Evaluate_if_recovered
            if (current->person.days >= INF_MAX_DUR(REAL_AGE(current->person.age, cfg->days))) {
                // Calculate probability!
                if (probEvento(IMMUNITY_RATE)) {
                    current->person.status = IMMUNE;
                } else {
                    current->person.status = HEALTHY;
                }
                current->person.days = 0;
            } else {
                current->person.days++;
            }
        }
        current = current->next;
    }
}

//void evaluate_if_recovered(ListPerson *people) {
//    ListPerson *current = people;
//
//    while (current != NULL) {
//        if (current->person.status == SICK) {
//            if (current->person.days == INF_MAX_DUR(current->person.age)) {
//                // Calculate probability!
//                if (probEvento(IMMUNITY_RATE)) {
//                    current->person.status = IMMUNE;
//                } else {
//                    current->person.status = HEALTHY;
//                }
//            } else {
//                current->person.days++;
//            }
//        }
//        current = current->next;
//    }
//}
