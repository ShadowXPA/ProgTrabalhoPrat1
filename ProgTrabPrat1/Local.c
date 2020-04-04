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

#include "Local.h"

ListPlace *init_list_place(const char *filename) {
    ListPlace *tmp = calloc(1, sizeof(ListPlace));
    if (tmp == NULL) {
        printf("Erro ao alocar memória para os locais!\n");
        exit(-1);
    }

    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        printf("Erro ao abrir o ficheiro.\n");
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
                printf("Erro ao alocar memória para os locais!\n");
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
        printf("Erro! O ficheiro dos locais contém erros!\n");
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
    // Verificar se dados estão coerentes
    // ID's únicos?
    if (!_evaluate_list_id(list_place))
        return false;
    // Ligações corretas
    if (!_evaluate_list_connection(list_place))
        return false;
    // Ligações não repetidas
    return _evaluate_list_connection_2(list_place);
}

bool _evaluate_list_id(const ListPlace *list_place) {
    for (int i = 0; i < list_place->size; i++) {
        // ID positivo
        if (list_place->place[i].id < 0)
            return false;
        else
            // ID único
            for (int j = i + 1; j < list_place->size; j++) {
                if (list_place->place[i].id == list_place->place[j].id)
                    return false;
            }
    }

    return true;
}

bool _evaluate_list_connection(const ListPlace *list_place) {
    // Nada eficiente... 4 for loops...
    // A cada local, procura cada ligação com id != -1
    // se for != -1 procura cada local até encontrar o id da ligação para i != k
    // e verifica se existe ligação
    // se não existir ligação mútua então retorna false
    // se existir, não precisa de procurar mais
    // no final se todas as ligações estiverem OK, retorna true
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
                // Ligações repetidas
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
    printf(" Locais:\n-----\n");
    char buffer0[11];
    char buffer1[11];
    char buffer2[11];
    for (int i = 0; i < list_place->size; i++) {
        sprintf(buffer0, "%d", list_place->place[i].connection[0]);
        sprintf(buffer1, "%d", list_place->place[i].connection[1]);
        sprintf(buffer2, "%d", list_place->place[i].connection[2]);
        printf(" ID: %d\n Capacidade: %d\n Ligações:\n  [1]: %s\n  [2]: %s\n  [3]: %s\n\n",
                list_place->place[i].id, list_place->place[i].capacity,
                list_place->place[i].connection[0] == -1 ? "Sem ligação." : buffer0,
                list_place->place[i].connection[1] == -1 ? "Sem ligação." : buffer1,
                list_place->place[i].connection[2] == -1 ? "Sem ligação." : buffer2);
    }
    printf(" Fim.\n-----\n");
}