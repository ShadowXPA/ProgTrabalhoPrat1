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

#include "Place.h"

ListPlace *init_places(const char *filename) {
    printf("\n Initializing List of places.\n");
    ListPlace *places = calloc(1, sizeof (ListPlace));
    if (places == NULL) {
        printf("Error allocating memory for places!\n");
        return NULL;
    }

    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Error opening the file '%s'.\n", filename);
        free(places);
        return NULL;
    }

    Place place;
    uint32_t i = 0;
    size_t read = 0;
    bool keep_reading = true;
    while (keep_reading) {
        if ((read = fread(&place, sizeof (Place), 1, fp)) == 0) {
            // EOF
            keep_reading = false;
        } else {
            if (!resize_places(places, (i + 1) * sizeof (Place))) {
                printf("Error! Not able to allocate memory for places\n");
                free_places(places);
                fclose(fp);
                return NULL;
            }
            places->place[i] = place;
            places->size = ++i; //(i + 1);
            //i++;
        }
    }

    fclose(fp);
    if (!evaluate_places(places) || places->size == 0) {
        printf("Error! The file '%s' does not contain the proper places!\n", filename);
        free_places(places);
        return NULL;
    }
    return places;
}

bool resize_places(ListPlace *places, const size_t new_size) {
    Place *tmp = NULL;

    tmp = realloc(places->place, new_size);
    if (tmp == NULL)
        return false;

    places->place = tmp;
    return true;
}

bool evaluate_places(const ListPlace *places) {
    // Evaluate whether the data is coherent or not
    // Unique IDs?
    if (!_evaluate_list_id(places)) {
        printf("Places do not have unique IDs!.\n");
        return false;
    }
    // Correct connections
    if (!_evaluate_list_connection(places)) {
        printf("Places do not have coherent connections.\n");
        return false;
    }
    // Non-repeating connections
    return _evaluate_list_connection_2(places);
}

bool _evaluate_list_id(const ListPlace *places) {
    for (uint32_t i = 0; i < places->size; i++) {
        // Positive ID
        if (places->place[i].id < 0)
            return false;
        else
            // Unique ID
            for (uint32_t j = i + 1; j < places->size; j++) {
                if (places->place[i].id == places->place[j].id)
                    return false;
            }
    }

    return true;
}

bool _evaluate_list_connection(const ListPlace *places) {
    /* Not efficient... 4 for loops...
     * For each place, search each connection with id != -1
     * if its != -1 search each place until it finds the id with the connection where i != k
     * and evaluates if there is a connection
     * If there is no mutual connection then return false
     * else, it doesn't need to keep searching.
     * In the end if all connections are OKAY, return true
     */
    for (uint32_t i = 0; i < places->size; i++) {
        for (uint32_t j = 0; j < MAX_CONNECTIONS; j++) {
            if (places->place[i].connection[j] != -1) {
                // Checkpoint = 1
                // Place[i] has Connection[j]
                uint8_t checkpoint = 1;
                for (uint32_t k = 0; k < places->size; k++) {
                    if (i != k && places->place[i].connection[j] == places->place[k].id) {
                        // Checkpoint = 2
                        // Place[i] has confirmed Connection[j]
                        checkpoint++;
                        for (uint32_t l = 0; l < MAX_CONNECTIONS; l++) {
                            if (places->place[k].connection[l] == places->place[i].id) {
                                // Checkpoint = 3
                                // Place[k] has Connection[l] that matches Place[i]'s ID
                                checkpoint++;
                                break;
                            }
                        }
                        // If checkpoint == 2
                        // It has found the Place[k] however it didn't get a match for Connection[l] with Place[i]
                        // If checkpoint == 3
                        // It has found the Place[k] and matched Connection[l] with Place[i]
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
                // Repeated connections
                if (places->place[i].connection[j] != -1 &&
                        places->place[i].connection[k] != -1 &&
                        places->place[i].connection[j] == places->place[i].connection[k])
                    return false;
            }
        }
    }

    return true;
}

void print_places(const ListPlace *places) {
    print_places___(places, stdout, "Places");
}

void print_places___(const ListPlace *places, FILE *stream, const char *title) {
    fprintf(stream, "\n----------------------\n %s:\n----------------------\n", title);
    char buffer[11];
    for (uint32_t i = 0; i < places->size; i++) {
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

void free_places(ListPlace *places) {
    free(places->place);
    free(places);
}

uint32_t get_max_capacity(const ListPlace *places) {
    uint32_t tmp = 0;

    for (uint32_t i = 0; i < places->size; i++) {
        tmp += places->place[i].capacity;
    }

    return tmp;
}

int32_t get_place_by_id(ListPlace *places, const int32_t id, Place **place) {
    *place = NULL;

    for (uint32_t i = 0; i < places->size; i++) {
        if (places->place[i].id == id) {
            *place = &places->place[i];
            return i;
        }
    }

    return (int32_t) - 1;
}
