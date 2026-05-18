#include "struct.h"

int main() {
    FILE *fin = fopen("tema1.in", "r");
    FILE *fout = fopen("tema1.out", "w");
    if (!fin || !fout) return 1;
    System *sys = init_system();
    int n_units;
    if (fscanf(fin, "%d", &n_units) != 1) return 1;
    sys->num_units = n_units;
    sys->units = malloc(n_units * sizeof(Unit));
    for (int i = 0; i < n_units; i++) {
        fscanf(fin, "%d %c", &sys->units[i].id, &sys->units[i].type);
        sys->units[i].availability = 1;
        queue_available_unit(sys, &sys->units[i]);
    }
    int n_ops;
    if (fscanf(fin, "%d", &n_ops) != 1) return 1;
    char cmd[50];
    for (int i = 0; i < n_ops; i++) {
        if (fscanf(fin, "%s", cmd) != 1) break;
        if (strcmp(cmd, "ADD_INCIDENT") == 0) {
            int id; 
            char prio[10];
            char desc[256];
            char c;
            fscanf(fin, "%d %s", &id, prio);
            while ((c = fgetc(fin)) != '"'); 
            int idx = 0;
            while ((c = fgetc(fin)) != '"') desc[idx++] = c; 
            desc[idx] = '\0';
            add_incident(sys, id, prio, desc);
        } 
        else if (strcmp(cmd, "SHOW_INCIDENT") == 0) {
            int id; fscanf(fin, "%d", &id);
            show_incident(sys, id, fout);
        }
        else if (strcmp(cmd, "CHECK_UNITS_AVAILABILITY") == 0) {
            check_units_availability(sys, fout);
        }
        else if (strcmp(cmd, "DISPATCH") == 0) {
            dispatch(sys, fout);
        }
        else if (strcmp(cmd, "SHOW_UNIT") == 0) {
            int id; fscanf(fin, "%d", &id);
            show_unit(sys, id, fout);
        }
        else if (strcmp(cmd, "SHOW_INTERVENTIONS") == 0) {
            show_interventions(sys, fout);
        }
        else if (strcmp(cmd, "SOLVED_INCIDENT") == 0) {
            int id; fscanf(fin, "%d", &id);
            solved_incident(sys, id, fout);
        }
    }
    fclose(fin); 
    fclose(fout);
    return 0;
}