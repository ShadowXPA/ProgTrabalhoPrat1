/*
 * Simulação da Propagação de Vírus - Criador de locais ou Visualizador de locais
 *
 * Trabalho realizado por: Pedro dos Santos Alves
 * Nº: 2019112789
 * Programação
 * Licenciatura em Engenharia Informática
 * Instituto Superior de Engenharia de Coimbra
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define MAX_CONNECTIONS 3
#define MAX_FILENAME 256
#define BUFFER_SIZE 512

typedef struct place {
    int32_t id;
    int32_t capacity;
    int32_t connection[MAX_CONNECTIONS];
} Place;

typedef struct arr_place {
    Place *place;
    uint32_t size;
} ListPlace;

bool add_connection(ListPlace *places, const int32_t P_SRC_id, const int32_t P_DEST_id);
bool add_place(ListPlace *places, const Place *place);
void create_file(ListPlace *places);
void print_places___(const ListPlace *places, FILE *stream, const char *title);
void show_help();
bool add_con(ListPlace *places, const int32_t P_SRC_id, const int32_t P_DEST_id);
bool evaluate_places(const ListPlace *places);
bool _evaluate_list_id(const ListPlace *places);
bool _evaluate_list_connection(const ListPlace *places);
bool _evaluate_list_connection_2(const ListPlace *places);

/*
 * 
 */
int main(int argc, char *argv[], char **envp) {
    char buffer[BUFFER_SIZE], *token;
    const char s[4] = " \t\n";
    int32_t N_places = 0, P_cap, P_id, P_SRC_id, P_DEST_id;
    bool adding_connections = true, save = true;
    ListPlace places;
    places.place = NULL;
    places.size = 0;

    if (argc == 2) {
        FILE *rf = fopen(argv[1], "rb");
        if (rf == NULL) {
            printf("\n\n File error!\n");
            return (EXIT_FAILURE);
        }

        Place place, *tmp;
        uint32_t i = 0;
        size_t read = 0;
        bool keep_reading = true;
        while (keep_reading) {
            if ((read = fread(&place, sizeof(Place), 1, rf)) == 0) {
                // EOF
                keep_reading = false;
            } else {
                tmp = realloc(places.place, (i + 1) * sizeof(Place));
                if (tmp == NULL) {
                    printf("Error! Not able to allocate memory for places\n");
                    fclose(rf);
                    free(places.place);
                    return (EXIT_FAILURE);
                } else {
                    places.place = tmp;
                }
                places.place[i] = place;
                places.size = ++i;
            }
        }

        fclose(rf);
        print_places___(&places, stdout, "Places from file");
        if (evaluate_places(&places)) {
            printf("\n\n  Valid file!\n\n");
        } else {
            printf("\n\n  Invalid file!\n\n");
        }
        free(places.place);
        return (EXIT_SUCCESS);
    }

    do {
        printf("\n\n Input number of places:\n > ");
        scanf("%[^\n]", buffer);
        getchar();
        N_places = atoi(buffer);
    } while (N_places <= 0);
    places.place = calloc(N_places, sizeof (Place));
    if (places.place == NULL) {
        return -1;
    }

    for (uint32_t i = 0; i < N_places;) {
        Place p;
        P_cap = -1;
        P_id = -1;
        do {
            printf("\n\n Input ID of place [%d]:\n > ", i);
            scanf("%[^\n]", buffer);
            getchar();
            P_id = atoi(buffer);
        } while (P_id <= 0);
        do {
            printf("\n\n Input capacity of place [%d]:\n > ", i);
            scanf("%[^\n]", buffer);
            getchar();
            P_cap = atoi(buffer);
        } while (P_cap <= 0);
        p.id = P_id;
        p.capacity = P_cap;
        for (uint32_t j = 0; j < MAX_CONNECTIONS; j++) {
            p.connection[j] = -1;
        }
        if (add_place(&places, &p)) {
            i++;
            printf("\n\n Place added!\n");
        } else {
            printf("\n\n Place not added!\n");
        }
    }

    do {
        P_SRC_id = 0;
        P_DEST_id = 0;
        printf("\n\n Input connections [Type 'help' for commands]:\n > ");
        scanf("%[^\n]", buffer);
        getchar();
        token = strtok(buffer, s);
        if (token != NULL) {
            if (strcmp("add", token) == 0) {
                token = strtok(NULL, s);
                if (token != NULL) {
                    P_SRC_id = atoi(token);
                    token = strtok(NULL, s);
                    if (token != NULL) {
                        P_DEST_id = atoi(token);
                        if (P_SRC_id != P_DEST_id && add_connection(&places, P_SRC_id, P_DEST_id)) {
                            printf("\n\n Added connection!\n");
                        } else {
                            printf("\n\n Couldn't add connection!\n");
                        }
                    }
                }
            } else if (strcmp("exit", token) == 0) {
                adding_connections = false;
            } else if (strcmp("cheat", token) == 0) {
                token = strtok(NULL, s);
                if (token != NULL) {
                    P_SRC_id = atoi(token);
                    token = strtok(NULL, s);
                    if (token != NULL) {
                        P_DEST_id = atoi(token);
                        if (add_con(&places, P_SRC_id, P_DEST_id)) {
                            printf("\n\n Cheated connections added...\n");
                        } else {
                            printf("\n\n Connections are full...\n");
                        }
                    }
                }
            } else if (strcmp("show", token) == 0) {
                print_places___(&places, stdout, "Places");
            } else if (strcmp("help", token) == 0) {
                show_help();
            } else if (strcmp("cancel", token) == 0) {
                adding_connections = false;
                save = false;
            } else if (strcmp("view", token) == 0) {

            }
        }
    } while (adding_connections);

    if (save)
        create_file(&places);
    free(places.place);
    return (EXIT_SUCCESS);
}

void print_places___(const ListPlace *places, FILE *stream, const char *title) {
    fprintf(stream, "\n----------------------\n %s:\n----------------------\n", title);
    char buffer[11];
    for (int i = 0; i < places->size; i++) {
        fprintf(stream, " ID: %d\n Capacity: %d\n Connections:\n",
                places->place[i].id, places->place[i].capacity);
        for (int j = 0; j < MAX_CONNECTIONS; j++) {
            snprintf(buffer, 11, "%d", places->place[i].connection[j]);
            fprintf(stream, "  (%d): %s\n", (j + 1), places->place[i].connection[j] == -1 ? "No connection." : buffer);
        }
        fprintf(stream, "\n");
    }
    fprintf(stream, "----------------------\n End.\n----------------------\n");
}

void show_help() {
    printf("Here's a list of all commands currently available:\n\n");
    printf("\tadd [SRC_ID] [DEST_ID]\n\t\t Adds a connection from SRC_ID to DEST_ID.\n\n");
    printf("\tshow\n\t\t Shows places.\n\n");
    printf("\tcheat [SRC_ID] [DEST_ID]\n\t\t Adds a connection from SRC_ID to DEST_ID, however DEST_ID won't be connected to SRC_ID.\n\n");
    printf("\tcancel\n\t\t Exits program without saving.\n\n");
    printf("\texit\n\t\t Saves places into specified file.\n\n");
    printf("\thelp\n\t\t Shows this.\n\n");
}

bool add_con(ListPlace *places, const int32_t P_SRC_id, const int32_t P_DEST_id) {
    for (uint32_t i = 0; i < places->size; i++) {
        if (places->place[i].id == P_SRC_id) {
            for (uint32_t j = 0; j < MAX_CONNECTIONS; j++) {
                if (places->place[i].connection[j] == -1) {
                    places->place[i].connection[j] = P_DEST_id;
                    return true;
                }
            }
        }
    }
    return false;
}

bool add_connection(ListPlace *places, const int32_t P_SRC_id, const int32_t P_DEST_id) {
    if (P_SRC_id == 0 || P_DEST_id == 0) {
        return false;
    }
    for (uint32_t i = 0; i < places->size; i++) {
        if (places->place[i].id == P_SRC_id) {
            for (uint32_t j = 0; j < places->size; j++) {
                if (i != j && places->place[j].id == P_DEST_id) {
                    uint32_t k = 0, l = 0, both = 0;
                    while (both != 2 && !(k == MAX_CONNECTIONS && l == MAX_CONNECTIONS)) {
                        if (places->place[i].connection[k] == P_DEST_id && places->place[j].connection[l] == P_SRC_id) {
                            return false;
                        }
                        if (places->place[i].connection[k] == -1) {
                            places->place[i].connection[k] = P_DEST_id;
                            both++;
                        } else if (places->place[i].connection[k] != P_DEST_id) {
                            k++;
                        }

                        if (places->place[j].connection[l] == -1) {
                            places->place[j].connection[l] = P_SRC_id;
                            both++;
                        } else if (places->place[j].connection[l] != P_SRC_id) {
                            l++;
                        }
                    }
                    if (both != 2) {
                        printf("\n\n Connections not added!\n");
                        return false;
                    } else {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

bool add_place(ListPlace *places, const Place *place) {
    uint32_t i = 0;
    for (; i < places->size; i++) {
        if (places->place[i].id == place->id || place->capacity <= 0) {
            return false;
        }
    }

    places->place[i] = *place;
    places->size++;
    return true;
}

void create_file(ListPlace *places) {
    char filename[MAX_FILENAME];
    FILE *fp = NULL;

    printf(" Input report filename:\n > ");
    scanf("%[^\n]", filename);
    getchar();
    fp = fopen(filename, "wb");
    fwrite(places->place, sizeof (Place), places->size, fp);
    fclose(fp);
}

bool evaluate_places(const ListPlace *places) {
    if (!_evaluate_list_id(places)) {
        return false;
    }
    if (!_evaluate_list_connection(places)) {
        return false;
    }
    return _evaluate_list_connection_2(places);
}

bool _evaluate_list_id(const ListPlace *places) {
    for (uint32_t i = 0; i < places->size; i++) {
        if (places->place[i].id < 0)
            return false;
        else
            for (uint32_t j = i + 1; j < places->size; j++) {
                if (places->place[i].id == places->place[j].id)
                    return false;
            }
    }

    return true;
}

bool _evaluate_list_connection(const ListPlace *places) {
    for (uint32_t i = 0; i < places->size; i++) {
        for (uint32_t j = 0; j < MAX_CONNECTIONS; j++) {
            if (places->place[i].connection[j] != -1) {
                uint8_t checkpoint = 1;
                for (uint32_t k = 0; k < places->size; k++) {
                    if (i != k && places->place[i].connection[j] == places->place[k].id) {
                        checkpoint++;
                        for (uint32_t l = 0; l < MAX_CONNECTIONS; l++) {
                            if (places->place[k].connection[l] == places->place[i].id) {
                                checkpoint++;
                                break;
                            }
                        }
                        break;
                    }
                }
                if (checkpoint != 3) {
                    return false;
                }
            }
        }
    }

    return true;
}

bool _evaluate_list_connection_2(const ListPlace *places) {
    for (uint32_t i = 0; i < places->size; i++) {
        for (uint32_t j = 0; j < MAX_CONNECTIONS - 1; j++) {
            for (uint32_t k = j + 1; k < MAX_CONNECTIONS; k++) {
                if (places->place[i].connection[j] != -1 &&
                        places->place[i].connection[k] != -1 &&
                        places->place[i].connection[j] == places->place[i].connection[k])
                    return false;
            }
        }
    }

    return true;
}
