#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// STRUCTURI
//

// Structuri Unitati
// all_units - lista simplu inlantuita
typedef struct unit
{
    int id;
    char type;
    int availability;
    struct unit* next;
} Unit_Cell, *Unit_List;

// queue_available_units - lista simplu inlantuita
typedef struct queue_unit
{
    Unit_Cell *info;
    struct queue_unit* next;
} Queue_Unit_Cell, *Queue_Unit_List;

// manager (head si tail) pentru coada queue_available_units
typedef struct queue_unit_manager
{
    Queue_Unit_Cell *head;
    Queue_Unit_Cell *tail;
} Queue_Unit_Manager;

//Structuri Incidente
// all_incidents - lista dublu inlantuita cu santinela
typedef struct incident
{
    int id;
    char priority[7];
    char *description;
    char status[11];
    struct incident* prev;
    struct incident* next;
} Incident_Cell, *Incident_List;

// queue_priority - lista simplu inlantuita
typedef struct queue_incident
{
    Incident_Cell *info;
    struct queue_incident *next;
} Queue_Incident_Cell, *Queue_Incient_List;

// manager (head si tail) pentru coada queue_priority
typedef struct queue_incident_manager
{
    Queue_Incident_Cell *head;
    Queue_Incident_Cell *tail;
} Queue_Incident_Manager;

// Structuri Interventii
// all_interventions - lista dublu inlantuita cu santinela
typedef struct intervention
{
    struct incident *incident;
    struct unit *unit;
    struct intervention* next;
    struct intervention* prev;
} Intervention_Cell, *Intervention_List;

// intervention_stack - stiva cu incidentele "intervened"
typedef struct stack_intervention
{
    Intervention_Cell *info;
    struct stack_intervention *next;
} Stack_Intervention_Cell, *Stack_Intervention_List;

//
// FUNCTII AUXILIARE - INITIALIZARI SI SUBFUNCTII
//

// initializeaza santinela pentru all_incidents
Incident_List INCIDENT_INIT()
{
    Incident_List incident_list = (Incident_List)malloc(sizeof(Incident_Cell));
    if(incident_list)
    {
        incident_list->id = 0;
        incident_list->description = NULL;
        incident_list->priority[0] = '\0';
        incident_list->status[0] = '\0';
        incident_list->next = incident_list;
        incident_list->prev = incident_list;
        return incident_list;
    }
    return NULL;
}

// initializeaza santinela pentru all_interventions
Intervention_List INTERVENTION_INIT()
{
    Intervention_List intervention_list = (Intervention_List)malloc(sizeof(Intervention_Cell));
    if(intervention_list)
    {
        intervention_list->incident = NULL;
        intervention_list->unit = NULL;
        intervention_list->next = intervention_list;
        intervention_list->prev = intervention_list;
        return intervention_list;
    }
    return NULL;
}


// returneaza NULL s-au capul cozii din queue_available_units
// apoi schimba capul acestuia cu urmatorul sau face coada NULL
// si da free la nodul respectiv (doar din coada, nu si din Unit_List)
Unit_List DEQUEUE_AVAILABLE_UNITS(Queue_Unit_Manager *queue_available_units)
{
    if (queue_available_units->head == NULL) return NULL;
    Queue_Unit_List temp = queue_available_units->head;
    Unit_List unit_info = temp->info;

    queue_available_units->head = queue_available_units->head->next;
    if (queue_available_units->head == NULL) queue_available_units->tail = NULL;
    free(temp);

    return unit_info;
}

// returneaza NULL s-au capul cozii din queue_priority
// apoi schimba capul acestuia cu urmatorul sau face coada NULL
// si da free la nodul respectiv (doar din coada, nu si din Incident_List)
Incident_List DEQUEUE_INCIDENT_PRIORITY(Queue_Incident_Manager *queue_priority)
{
    if (queue_priority->head == NULL) return NULL;
    Queue_Incient_List temp = queue_priority->head;
    Incident_List incident_info = temp->info;

    queue_priority->head = queue_priority->head->next;
    if (queue_priority->head == NULL) queue_priority->tail = NULL;
    free(temp);

    return incident_info;
}

// returneaza NULL s-au primul element din intervention_stack
// apoi schimba capul acestuia cu urmatorul sau face stiva NULL
// si da free la nodul respectiv (doar din stiva, nu si din Intervention_List)
Intervention_List POP_INTERVENTION(Stack_Intervention_List *intervention_stack)
{
    if (*intervention_stack == NULL) return NULL;
    Stack_Intervention_List temp = *intervention_stack;
    Intervention_List intervention_info = temp->info;
    *intervention_stack = temp->next;
    free(temp);
    return intervention_info;
}

// adauga un element nou in cap, schimba capul cu el in stiva intervention_stack
void TOP_INTEVENTION(Stack_Intervention_List *intervention_stack, Intervention_List new_intervention)
{
        Stack_Intervention_List new_node = (Stack_Intervention_List)malloc(sizeof(Stack_Intervention_Cell));
        if(new_node)
        {
            new_node->info = new_intervention;
            new_node->next = *intervention_stack;
            *intervention_stack = new_node;
        }
}
// adauga un element nou in coada la queue_unit_available
// si schimba tail-ul cu pozitia ultimul element
// daca acesta e primul element schimba si head-ul
void ENQUEUE_AVAILABLE_UNITS(Queue_Unit_Manager *queue_units_available, Unit_List new_unit)
{
    Queue_Unit_List new_node = (Queue_Unit_List)malloc(sizeof(Queue_Unit_Cell));
    if(new_node)
    {
        new_node->info = new_unit;
        new_node->next = NULL;

        if (queue_units_available->head == NULL)
        {
            queue_units_available->head = new_node;
            queue_units_available->tail = new_node;
        } else
        {
            queue_units_available->tail->next = new_node;
            queue_units_available->tail = new_node;
        }
    }
}

// adauga un element nou in coada la queue_priority
// si schimba tail-ul cu pozitia ultimul element
// daca acesta e primul element schimba si head-ul
void ENQUEUE_INCIDENT_PRIORITY(Queue_Incident_Manager *queue_priority, Incident_List new_incident)
{
    Queue_Incient_List new_node = (Queue_Incient_List)malloc(sizeof(Queue_Incident_Cell));
    if(new_node)
    {
        new_node->info = new_incident;
        new_node->next = NULL;

        if (queue_priority->head == NULL)
        {
            queue_priority->head = new_node;
            queue_priority->tail = new_node;
        } else
        {
            queue_priority->tail->next = new_node;
            queue_priority->tail = new_node;
        }
    }
}

//
// FUNCTII PRINCIPALE
//

// adauga unitatea noua la final, atat in all_units, cat si in queue_available_units
// ca sa adauge in all_units se foloseste un while, sau daca e primul il pune ca primul element
void ADD_UNIT(Unit_List *all_units, Queue_Unit_Manager *queue_available_units, int id_unit, char type_unit)
{
    Unit_List new_unit = (Unit_List)malloc(sizeof(Unit_Cell));
    if (new_unit)
    {
        new_unit->id = id_unit;
        new_unit->type = type_unit;
        new_unit->availability = 1;
        new_unit->next = NULL;

        if (*all_units == NULL)
        {
            *all_units = new_unit;
        } else
        {
            Unit_List temp = *all_units;
            while(temp->next) temp = temp->next;
            temp->next = new_unit;
        }
        ENQUEUE_AVAILABLE_UNITS(queue_available_units, new_unit);
    }
}

// adauga incidentul nou la final, atat in all_incidents, cat si in queue_priority
void ADD_INCIDENT(Incident_List all_incidents, Queue_Incident_Manager *queue_high, Queue_Incident_Manager *queue_medium, Queue_Incident_Manager *queue_low,
                  int id, char* new_priority, char* new_description)
{
    Incident_List new_incident = (Incident_List)malloc(sizeof(Incident_Cell));
    if(new_incident)
    {
        new_incident->id = id;
        new_incident->description = (char*)malloc(strlen(new_description) + 1);
        strcpy(new_incident->description, new_description);
        strcpy(new_incident->priority, new_priority);
        strcpy(new_incident->status, "queued");

        new_incident->prev = all_incidents->prev;
        new_incident->next = all_incidents;
        all_incidents->prev->next = new_incident;
        all_incidents->prev = new_incident;

        if (strcmp(new_priority, "high") == 0) ENQUEUE_INCIDENT_PRIORITY(queue_high, new_incident);
        else if (strcmp(new_priority, "medium") == 0) ENQUEUE_INCIDENT_PRIORITY(queue_medium, new_incident);
        else ENQUEUE_INCIDENT_PRIORITY(queue_low, new_incident);
    }
}

// Functia creeaza un nou element atat in all_interventions, cat si in intervention_stack
// Mai intai cautam daca avem unitati si incidente (dupa fiecare grad de prioritate)
// Scoatem din ambele cozi primul element si il inseram in new_intervention
// Schimbam in incident si in unitate statusul respectiv, valabilitatea
// Apoi adaugam interventia la final in all_intervention si la final in intervention_stack
void DISPATCH(FILE *fout, Intervention_List all_interventions, Stack_Intervention_List *intervention_stack, Queue_Unit_Manager *queue_available_units,
              Queue_Incident_Manager *queue_high, Queue_Incident_Manager *queue_medium, Queue_Incident_Manager *queue_low)
{
    if (queue_available_units->head == NULL)
    {
        fprintf(fout, "INVALID OPERATION! ERROR 404\n");
        return;
    }

    Queue_Incident_Manager *queue_used = NULL;
    if (queue_high->head != NULL) queue_used = queue_high;
    else if (queue_medium->head != NULL) queue_used = queue_medium;
    else if (queue_low->head != NULL) queue_used = queue_low;

    if (queue_used == NULL)
    {
        fprintf(fout, "INVALID OPERATION! ERROR 404\n");
        return;
    }

    Unit_List unit_used = DEQUEUE_AVAILABLE_UNITS(queue_available_units);
    Incident_List incident_used = DEQUEUE_INCIDENT_PRIORITY(queue_used);

    Intervention_List new_intervention = (Intervention_List)malloc(sizeof(Intervention_Cell));
    if (new_intervention)
    {
        new_intervention->incident = incident_used;
        new_intervention->unit = unit_used;
        strcpy(incident_used->status, "intervened");
        unit_used->availability = 0;

        new_intervention->next = all_interventions;
        new_intervention->prev = all_interventions->prev;
        all_interventions->prev->next = new_intervention;
        all_interventions->prev = new_intervention;

        TOP_INTEVENTION(intervention_stack, new_intervention);
    }
}

// folosim prima oara functia POP_INTERVENTION care returneaza NULL sau ultima interventie (scoate interventia din stiva)
// verificam interventia, schimbam informatiile din ea si adaugam unitatea si incidentul in cozile lor
// unitatea la final si incidentul la inceput, apoi interschimbam nodurile din all_interventions
void UNDO_LAST_DISPATCH(FILE *fout, Stack_Intervention_List *intervention_stack, Queue_Unit_Manager *queue_available_units,
                        Queue_Incident_Manager *queue_high, Queue_Incident_Manager *queue_medium, Queue_Incident_Manager *queue_low)
{
    Intervention_List intervention_used = POP_INTERVENTION(intervention_stack);
    if (intervention_used == NULL)
    {
        fprintf(fout, "INVALID OPERATION! ERROR 404\n");
        return;
    }

    intervention_used->unit->availability = 1;
    ENQUEUE_AVAILABLE_UNITS(queue_available_units, intervention_used->unit);

    strcpy(intervention_used->incident->status, "queued");
    Queue_Incident_Manager *queue_used = NULL;
    if (strcmp(intervention_used->incident->priority, "high") == 0) queue_used = queue_high;
    else if (strcmp(intervention_used->incident->priority, "medium") == 0) queue_used = queue_medium;
    else queue_used = queue_low;

    Queue_Incient_List new_incident = (Queue_Incient_List)malloc(sizeof(Queue_Incident_Cell));
    new_incident->info = intervention_used->incident;
    new_incident->next = queue_used->head;
    queue_used->head = new_incident;
    if (queue_used->tail == NULL) queue_used->tail = new_incident;

    intervention_used->prev->next = intervention_used->next;
    intervention_used->next->prev = intervention_used->prev;
    free(intervention_used);
}

// functia cauta in stiva incidentul care are deja "intervened"
// cu ajutorul a 2 pointeri current si slow (pentru inerschimbare)
// daca nu il gaseste afiseaza eroare, exista 3 cazuri de acest gen
// ori nu exista incidentul ori e deja rezolvat ori nu este preluat
// daca il gaseste rezolva incidentul, schimba valabilitatea la unitate
// apoi adauga unitatea in coada si face interschimbarea in stiva si da free
void SOLVED_INCIDENT(FILE *fout, Stack_Intervention_List *intervention_stack, Queue_Unit_Manager *queue_available_units, int id)
{
    Stack_Intervention_List current = *intervention_stack;
    Stack_Intervention_List slow = NULL;

    while (current != NULL)
    {
        if (current->info->incident->id == id)
        {
            strcpy(current->info->incident->status, "solved");
            current->info->unit->availability = 1;
            ENQUEUE_AVAILABLE_UNITS(queue_available_units, current->info->unit);

            if (slow == NULL) *intervention_stack = current->next;
            else slow->next = current->next;
            free(current);
            return;
        }
        slow = current;
        current = current->next;
    }
    fprintf(fout, "INVALID OPERATION! ERROR 404\n");
}

// afiseaza numarul de unitati valabile cu un while cu contor
void CHECK_UNITS_AVAILABILITY(FILE *fout, Queue_Unit_Manager queue_available_units)
{
    int count = 0;
    Queue_Unit_List temp = queue_available_units.head;
    while(temp)
    {
        count++;
        temp = temp->next;
    }
    fprintf(fout, "Number of available units: %d\n", count);
}

// afiseaza unitatea cu un while, cand gaseste unitatea cu un id specific il afiseaza, daca nu il gaseste afiseaza eroare
void SHOW_UNIT(FILE *fout, Unit_List all_units, int id)
{
    Unit_List temp = all_units;
    while(temp)
    {
        if(temp->id == id)
        {
            fprintf(fout, "Unit %d is type %c and is %s\n", temp->id, temp->type, temp->availability ? "available" : "unavailable");
            return;
        }
        temp = temp->next;
    }
    fprintf(fout, "INVALID OPERATION! ERROR 404\n");
}

// afiseaza incidentul cu un while, cand gaseste incidentul cu un id specific il afiseaza, daca nu il gaseste afiseaza eroare
// afiseaza eroare si in cazul in care ne cere incidentul santinela
void SHOW_INCIDENT(FILE *fout, Incident_List all_incidents, int id)
{
    Incident_List temp = all_incidents->next;
    while(temp != all_incidents)
    {
        if(temp->id == id)
        {
            fprintf(fout, "Incident %d has %s priority, the following description: \"%s\" and is %s\n",
                   temp->id, temp->priority, temp->description, temp->status);
            return;
        }
        temp = temp->next;
    }
    fprintf(fout, "INVALID OPERATION! ERROR 404\n");
}

// afiseaza toate interventiile cu un while, daca lista are doar santinela afiseaza mesajul "No intervention has been initiated"
void SHOW_INTERVENTIONS(FILE *fout, Intervention_List all_interventions)
{
    if (all_interventions->next == all_interventions)
    {
        fprintf(fout, "No intervention has been initiated\n");
        return;
    }
    Intervention_List temp = all_interventions->next;
    while(temp != all_interventions)
    {
        fprintf(fout, "Incident %d was assigned to unit %d, and has the following status: \"%s\"\n",
               temp->incident->id, temp->unit->id, temp->incident->status);
        temp = temp->next;
    }
}

//
// FUNCTII SUPLIMENTARE - DE INPUT
//

// functia ia inputul si adauga in all_units si queue_available_units cu ajutorul unui for si functiei ADD_UNIT
void UNIT_INPUT(FILE *fin, Unit_List *all_units, Queue_Unit_Manager *manager)
{
    int units_number;
    int id;
    char type;

    fscanf(fin, "%d", &units_number);

    for(int i = 0; i < units_number; i++)
    {
        fscanf(fin, " %d %c", &id, &type);
        ADD_UNIT(all_units, manager, id, type);
    }
}

// functia aceasta se ocupa cu manageuirea inputurilor si le transforma in comenzi
// pentru a afla line-ul am folosit fgets, iar pentru a gasi comanda am folosit sscanf (pentru a manipula mai usor inputul)
// pentru ADD_INCIDENT am folosit strchr si strrchr ca sa caut ghilimelele (una ia de la inceput, cealalta de la final)
// functiile se folosesc de if si else if, nu am folosit case, prefer varianta mai simpla
void COMMAND_INPUT(FILE *fout, char *line, Unit_List all_units, Queue_Unit_Manager *queue_units_available,
                   Queue_Incident_Manager *queue_high, Queue_Incident_Manager *queue_medium, Queue_Incident_Manager *queue_low,
                   Incident_List all_incidents, Intervention_List all_interventions, Stack_Intervention_List *intervention_stack)
{
    char command_input[50];
    sscanf(line, "%s", command_input);

    if(strcmp(command_input, "ADD_INCIDENT") == 0)
    {
        int id;
        char priority_input[7];
        char *description_input;
        char *first_quote = strchr(line, '\"');
        char *last_quote = strrchr(line, '\"');
        if(first_quote && last_quote && first_quote != last_quote)
        {
            *last_quote = '\0';
            description_input = first_quote + 1;
            sscanf(line, "ADD_INCIDENT %d %s", &id, priority_input);
            ADD_INCIDENT(all_incidents, queue_high, queue_medium, queue_low, id, priority_input, description_input);
        }
    }
    else if(strcmp(command_input, "SOLVED_INCIDENT") == 0)
    {
        int id;
        sscanf(line, "SOLVED_INCIDENT %d", &id);
        SOLVED_INCIDENT(fout, intervention_stack, queue_units_available, id);
    }
    else if(strcmp(command_input, "SHOW_UNIT") == 0)
    {
        int id;
        sscanf(line, "SHOW_UNIT %d", &id);
        SHOW_UNIT(fout, all_units, id);
    }
    else if(strcmp(command_input, "SHOW_INCIDENT") == 0)
    {
        int id;
        sscanf(line, "SHOW_INCIDENT %d", &id);
        SHOW_INCIDENT(fout, all_incidents, id);
    }
    else if(strcmp(command_input, "DISPATCH") == 0) DISPATCH(fout, all_interventions, intervention_stack, queue_units_available, queue_high, queue_medium, queue_low);
    else if(strcmp(command_input, "UNDO_LAST_DISPATCH") == 0) UNDO_LAST_DISPATCH(fout, intervention_stack, queue_units_available, queue_high, queue_medium, queue_low);
    else if(strcmp(command_input, "SHOW_INTERVENTIONS") == 0) SHOW_INTERVENTIONS(fout, all_interventions);
    else if(strcmp(command_input, "CHECK_UNITS_AVAILABILITY") == 0) CHECK_UNITS_AVAILABILITY(fout, *queue_units_available);
}

// aceasta este functia care se asigura la finalul codului ca memoria alocata dinamic este eliberata
// pentru a evita double free-ul mai intai stergem listele care contin doar pointeri la noduri
// apoi listele cu toate incidente si echipajele la final
// (pasi sunt urmatorii: stiva, cozile, listele cu santinela, lista cu echipaje)
void FREE_ALL(Unit_List *all_units, Queue_Unit_Manager *queue_available_units,
              Queue_Incident_Manager *queue_high, Queue_Incident_Manager *queue_medium, Queue_Incident_Manager *queue_low,
              Incident_List all_incidents, Intervention_List all_interventions, Stack_Intervention_List *intervention_stack)
{
    // free intervention_stack
    while (*intervention_stack) POP_INTERVENTION(intervention_stack);

    // free queue_available_units
    while(queue_available_units->head) DEQUEUE_AVAILABLE_UNITS(queue_available_units);

    // free queue_high, queue_medium, queue_low
    Queue_Incident_Manager* queue_priority[] = {queue_high, queue_medium, queue_low};
    for(int i = 0; i < 3; i++)
    {
        while(queue_priority[i]->head) DEQUEUE_INCIDENT_PRIORITY(queue_priority[i]);
    }

    // free all_interventions
    Intervention_List current_interventions = all_interventions->next;
    while(current_interventions != all_interventions)
    {
        Intervention_List to_free = current_interventions;
        current_interventions = current_interventions->next;
        free(to_free);
    }
    free(all_interventions);

    // free all_incidents
    Incident_List current_incidents = all_incidents->next;
    while(current_incidents != all_incidents)
    {
        Incident_List to_free = current_incidents;
        current_incidents = current_incidents->next;
        free(to_free->description);
        free(to_free);
    }
    free(all_incidents);

    // free all_units
    Unit_List current_units = *all_units;
    while(current_units)
    {
        Unit_List to_free = current_units;
        current_units = current_units->next;
        free(to_free);
    }
    *all_units = NULL;
}

// functia de main
// are 3 parti, initializarea cu toate functiile de initializare,
// programul in sine cu toate functiile cerute
// si la final free-urile pentru a nu ramane memorie salvata aiurea
int main()
{
    FILE *fin = fopen("tema1.in", "r");
    FILE *fout = fopen ("tema1.out", "w");
    if (fin == NULL || fout == NULL) return 1;

    Unit_List all_units = NULL;
    Queue_Unit_Manager queue_available_units = {NULL, NULL};

    Queue_Incident_Manager queue_high = {NULL, NULL};
    Queue_Incident_Manager queue_medium = {NULL, NULL};
    Queue_Incident_Manager queue_low = {NULL, NULL};

    Incident_List all_incidents = INCIDENT_INIT();
    Intervention_List all_interventions = INTERVENTION_INIT();
    Stack_Intervention_List intervention_stack = NULL;

    UNIT_INPUT(fin, &all_units, &queue_available_units);

    int commands_number;
    char line[512];

    fscanf(fin, "%d ", &commands_number);

    for(int i = 0; i < commands_number; i++)
    {
        fgets(line, sizeof(line), fin);
        int len = strlen(line);
        if (len > 0 && line[len - 1] == '\n')
        {
            line[len - 1] = '\0';
            len--;
        }
        if (strlen(line) == 0) continue;

        COMMAND_INPUT(fout, line, all_units, &queue_available_units, &queue_high, &queue_medium, &queue_low, all_incidents, all_interventions, &intervention_stack);
    }

    FREE_ALL(&all_units, &queue_available_units, &queue_high, &queue_medium, &queue_low, all_incidents, all_interventions, &intervention_stack);
    fclose(fin);
    fclose(fout);
    return 0;
}
