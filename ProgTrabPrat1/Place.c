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

ListPlace *init_list_place(const char *filename) {
    printf("\n Initializing List of places.\n");
    ListPlace *tmp = calloc(1, sizeof(ListPlace));
    if (tmp == NULL) {
        printf("Error allocating memory for places!\n");
        exit(-1);
    }

    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Error opening the file '%s'.\n", filename);
        free(tmp);
        exit(-1);
    }

    Place tmp_place;
    uint32_t i = 0;
    size_t read = 0;
    bool keep_reading = true;
    while (keep_reading) {
        if ((read = fread(&tmp_place, sizeof(Place), 1, fp)) == 0) {
            // EOF
            keep_reading = false;
        } else {
            if (!resize_list_place(tmp, (i + 1) * sizeof(Place))) {
                printf("Error! Not able to allocate memory for places\n");
                free(tmp->place);
                free(tmp);
                exit(-1);
            }
            tmp->place[i] = tmp_place;
            tmp->size = ++i;//(i + 1);
            //i++;
        }
    }

    fclose(fp);
    if (!evaluate_list_place(tmp)) {
        printf("Error! The file '%s' contains syntax errors!\n", filename);
        free(tmp->place);
        free(tmp);
        exit(-1);
    }
    return tmp;
}

bool resize_list_place(ListPlace *list_place, const size_t new_size) {
    Place *tmp = NULL;

    tmp = realloc(list_place->place, new_size);
    if (tmp == NULL)
        return false;

    list_place->place = tmp;
    return true;
}

bool evaluate_list_place(const ListPlace *list_place) {
    // Evaluate whether the data is coherent or not
    // Unique IDs?
    if (!_evaluate_list_id(list_place)) {
        printf("Places do not have unique IDs!.\n");
        return false;
    }
    // Correct connections
    if (!_evaluate_list_connection(list_place)) {
        printf("Places do not have coherent connections.\n");
        return false;
    }
    // Non-repeating connections
    return _evaluate_list_connection_2(list_place);
}

bool _evaluate_list_id(const ListPlace *list_place) {
    for (int i = 0; i < list_place->size; i++) {
        // Positive ID
        if (list_place->place[i].id < 0)
            return false;
        else
            // Unique ID
            for (int j = i + 1; j < list_place->size; j++) {
                if (list_place->place[i].id == list_place->place[j].id)
                    return false;
            }
    }

    return true;
}

bool _evaluate_list_connection(const ListPlace *list_place) {
    /* Not efficient... 4 for loops...
     * For each place, search each connection with id != -1
     * if its != -1 search each place until it finds the id with the connection where i != k
     * and evaluates if there is a connection
     * If there is no mutual connection then return false
     * else, it doesn't need to keep searching.
     * In the end if all connections are OKAY, return true
     */
    for (int i = 0; i < list_place->size; i++) {
        for (int j = 0; j < MAX_CONNECTIONS; j++) {
            if (list_place->place[i].connection[j] != -1) {
                for (int k = 0; k < list_place->size; k++) {
                    if (i != k) {
                        if (list_place->place[i].connection[j] == list_place->place[k].id) {
                            bool has_connection = false;
                            for (int l = 0; l < MAX_CONNECTIONS; l++) {
                                if (list_place->place[k].connection[l] == list_place->place[i].id) {
                                    has_connection = true;
                                    break;
                                }
                            }
                            if (!has_connection) {
                                return false;
                            }
                            break;
                        }
                    }
                }
            }
        }
    }

    return true;
}

bool _evaluate_list_connection_2(const ListPlace *list_place) {
    for (int i = 0; i < list_place->size; i++) {
        for (int j = 0; j < MAX_CONNECTIONS - 1; j++) {
            for (int k = j + 1; k < MAX_CONNECTIONS; k++) {
                // Repeated connections
                if (list_place->place[i].connection[j] != -1 &&
                        list_place->place[i].connection[k] != -1 &&
                        list_place->place[i].connection[j] == list_place->place[i].connection[k])
                    return false;
            }
        }
    }

    return true;
}

void view_list_place(const ListPlace *list_place) {
    printf(" Places:\n-----\n");
    char buffer0[11];
    char buffer1[11];
    char buffer2[11];
    for (int i = 0; i < list_place->size; i++) {
        snprintf(buffer0, 11, "%d", list_place->place[i].connection[0]);
        snprintf(buffer1, 11, "%d", list_place->place[i].connection[1]);
        snprintf(buffer2, 11, "%d", list_place->place[i].connection[2]);
        printf(" ID: %d\n Capacity: %d\n Connections:\n  (1): %s\n  (2): %s\n  (3): %s\n\n",
                list_place->place[i].id, list_place->place[i].capacity,
                list_place->place[i].connection[0] == -1 ? "No connection." : buffer0,
                list_place->place[i].connection[1] == -1 ? "No connection." : buffer1,
                list_place->place[i].connection[2] == -1 ? "No connection." : buffer2);
    }
    printf(" End.\n-----\n");
}