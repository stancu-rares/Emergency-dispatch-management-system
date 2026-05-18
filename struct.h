#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct unit {
    int id;
    char type;
    int availability;
} Unit;

typedef struct incident {
    int id;
    char priority[7];
    char *description;
    char status[11];
    struct incident *prev;
    struct incident *next;
} Incident;

typedef struct intervention {
    Incident *incident;
    Unit *unit;
    struct intervention *prev;
    struct intervention *next;
} Intervention;

typedef struct queue_incident_node {
    Incident *incident_ptr;
    struct queue_incident_node *next;
} QueueIncidentNode;

typedef struct queue_incident {
    QueueIncidentNode *head;
    QueueIncidentNode *tail;
} QueueIncident;

typedef struct queue_unit_node {
    Unit *unit_ptr;
    struct queue_unit_node *next;
} QueueUnitNode;

typedef struct queue_unit {
    QueueUnitNode *head;
    QueueUnitNode *tail;
} QueueUnit;

typedef struct stack_node {
    Intervention *intervention_ptr;
    struct stack_node *next;
} StackNode;

typedef struct stack {
    StackNode *top;
} Stack;

typedef struct system {
    int num_units; 
    Unit *units; 
    Incident *incidents; 
    Intervention *interventions; 
    QueueIncident *queue_high;
    QueueIncident *queue_medium;
    QueueIncident *queue_low;
    QueueUnit *queue_available_units;
    Stack *undo_stack;
} System;

System* init_system();
void add_incident(System *sys, int id, const char *priority, const char *description);
void show_incident(System *sys, int id, FILE *fout);
void queue_available_unit(System *sys, Unit *unit);
void check_units_availability(System *sys, FILE *fout);
void dispatch(System *sys, FILE *fout);
void show_unit(System *sys, int id, FILE *fout);
void show_interventions(System *sys, FILE *fout);
void solved_incident(System *sys, int id, FILE *fout);
void undo_last_dispatch(System *sys, FILE *fout);