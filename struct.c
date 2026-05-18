#include "struct.h"

System* init_system() {
    System *sys = malloc(sizeof(System));
    sys->queue_high = malloc(sizeof(QueueIncident));
    sys->queue_high->head = NULL;
    sys->queue_high->tail = NULL;
    sys->queue_medium = malloc(sizeof(QueueIncident));
    sys->queue_medium->head = NULL;
    sys->queue_medium->tail = NULL;
    sys->queue_low = malloc(sizeof(QueueIncident));
    sys->queue_low->head = NULL;
    sys->queue_low->tail = NULL;
    sys->queue_available_units = malloc(sizeof(QueueUnit));
    sys->queue_available_units->head = NULL;
    sys->queue_available_units->tail = NULL;
    sys->undo_stack = malloc(sizeof(Stack));
    sys->undo_stack->top = NULL;
    Incident *santinela_inc = malloc(sizeof(Incident));
    santinela_inc->id = 0;
    strcpy(santinela_inc->priority, "low");
    santinela_inc->description = strdup("test incident");
    strcpy(santinela_inc->status, "solved");
    santinela_inc->next = santinela_inc;
    santinela_inc->prev = santinela_inc;
    sys->incidents = santinela_inc;
    Intervention *santinela_int = malloc(sizeof(Intervention));
    santinela_int->incident = NULL;
    santinela_int->unit = NULL;
    santinela_int->next = santinela_int;
    santinela_int->prev = santinela_int;
    sys->interventions = santinela_int;
    return sys;
}

void add_incident(System *sys, int id, const char *priority, const char *description) {
    Incident *new = malloc(sizeof(Incident));
    new->id = id;
    strcpy(new->priority, priority);
    new->description = strdup(description);
    strcpy(new->status, "queued");
    Incident *santinela = sys->incidents;
    new->next = santinela;
    new->prev = santinela->prev;
    santinela->prev->next = new;
    santinela->prev = new;
    QueueIncidentNode *stack_tail = malloc(sizeof(QueueIncidentNode));
    stack_tail->incident_ptr = new;
    stack_tail->next = NULL;
    QueueIncident *queue = NULL;
    if (strcmp(priority, "high") == 0) {
        queue = sys->queue_high;
    } else if (strcmp(priority, "medium") == 0) {
        queue = sys->queue_medium;
    } else if (strcmp(priority, "low") == 0) {
        queue = sys->queue_low;
    }
    if (queue != NULL) {
        if (queue->head == NULL) {
            queue->head = stack_tail;
            queue->tail = stack_tail;
        } else {
            queue->tail->next = stack_tail;
            queue->tail = stack_tail;
        }
    }
}

void queue_available_unit(System *sys, Unit *unit) {
    QueueUnitNode *node = malloc(sizeof(QueueUnitNode));
    node->unit_ptr = unit;
    node->next = NULL;
    if (sys->queue_available_units->head == NULL) {
        sys->queue_available_units->head = node;
        sys->queue_available_units->tail = node;
    } else {
        sys->queue_available_units->tail->next = node;
        sys->queue_available_units->tail = node;
    }
}

void check_units_availability(System *sys, FILE *fout) {
    int nr_disp = 0;
    QueueUnitNode *curent = sys->queue_available_units->head;
    while (curent != NULL) {
        nr_disp++;
        curent = curent->next;
    }
    fprintf(fout, "Number of available units: %d\n", nr_disp);
}

Incident* extrage_incident(QueueIncident *queue) {
    if (queue->head == NULL) return NULL;
    QueueIncidentNode *temp = queue->head;
    Incident *inc_extras = temp->incident_ptr;
    queue->head = queue->head->next;
    if (queue->head == NULL) {
        queue->tail = NULL;
    }
    free(temp);
    return inc_extras;
}

Unit* extrage_unitate(QueueUnit *queue) {
    if (queue->head == NULL) return NULL;
    QueueUnitNode *temp = queue->head;
    Unit *u_extras = temp->unit_ptr;
    queue->head = queue->head->next;
    if (queue->head == NULL) {
        queue->tail = NULL;
    }
    free(temp);
    return u_extras;
}

void dispatch(System *sys, FILE *fout) {
    if (sys->queue_high->head == NULL && sys->queue_medium->head == NULL && sys->queue_low->head ==NULL) {
        fprintf(fout, "INVALID OPERATION! ERROR 404\n");
    } else if (sys->queue_available_units->head == NULL) {
        fprintf(fout, "INVALID OPERATION! ERROR 404\n");
    } else {
        Incident *inc_ales = extrage_incident(sys->queue_high);
        if (inc_ales == NULL) {
            inc_ales = extrage_incident(sys->queue_medium);
        }
        if (inc_ales == NULL) {
            inc_ales = extrage_incident(sys->queue_low);
        }
        Unit *u_ales = extrage_unitate(sys->queue_available_units);
        strcpy(inc_ales->status, "intervened");
        u_ales->availability = 0;
        Intervention *int_noua = malloc(sizeof(Intervention));
        int_noua->incident = inc_ales;
        int_noua->unit = u_ales;
        Intervention *santinela = sys->interventions;
        int_noua->next = santinela;
        int_noua->prev = santinela->prev;
        santinela->prev->next = int_noua;
        santinela->prev = int_noua;
        StackNode *stack_node = malloc(sizeof(StackNode));
        stack_node->intervention_ptr = int_noua;
        stack_node->next = sys->undo_stack->top;
        sys->undo_stack->top = stack_node;
    }
}

void show_incident(System *sys, int id, FILE *fout) {
    Incident *curent = sys->incidents->next;
    int gasit = 0;
    while (curent != sys->incidents) {
        if (curent->id == id) {
            fprintf(fout, "Incident %d has %s priority, the following description: \"%s\" and is %s\n",
                    curent->id, curent->priority, curent->description, curent->status);
            gasit = 1;
            break;
        }
        curent = curent->next;
    }
    if (gasit == 0) {
        fprintf(fout, "INVALID OPERATION! ERROR 404\n");
    }
}

void show_unit(System *sys, int id, FILE *fout) {
    int gasit = 0;
    for (int i = 0; i < sys->num_units; i++) {
        if (sys->units[i].id == id) {
            fprintf(fout, "Unit %d is type %c and is %s\n",
                    sys->units[i].id, sys->units[i].type,
                    sys->units[i].availability ? "available" : "unavailable");
            gasit = 1;
            break;
        }
    }
    if (gasit == 0) {
        fprintf(fout, "INVALID OPERATION! ERROR 404\n");
    }
}

void show_interventions(System *sys, FILE *fout) {
    if (sys->interventions->next == sys->interventions) {
        fprintf(fout, "No intervention has been initiated\n");
    } else {
        Intervention *curent = sys->interventions->next;
        while (curent != sys->interventions) {
            fprintf(fout, "Incident %d was assigned to unit %d, and has the following status: \"%s\"\n",
                    curent->incident->id, curent->unit->id, curent->incident->status);
            curent = curent->next;
        }
    }
}

void solved_incident(System *sys, int id, FILE *fout) {
    Incident *inc_gasit = NULL;
    Incident *curent_inc = sys->incidents->next;
    while (curent_inc != sys->incidents) {
        if (curent_inc->id == id) {
            inc_gasit = curent_inc;
            break;
        }
        curent_inc = curent_inc->next;
    }
    
    if (inc_gasit == NULL || strcmp(inc_gasit->status, "intervened") != 0) {
        fprintf(fout, "INVALID OPERATION! ERROR 404\n");
    } else {
        Intervention *curent_int = sys->interventions->next;
        while (curent_int != sys->interventions) {
            if (curent_int->incident->id == id) {
                strcpy(inc_gasit->status, "solved");
                curent_int->unit->availability = 1;
                queue_available_unit(sys, curent_int->unit);
                break;
            }
            curent_int = curent_int->next;
        }
    }
}