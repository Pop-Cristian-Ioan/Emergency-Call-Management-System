# SDA Tema1 - Liste inlantuite, stive, cozi

## Durata de Realizare a Temei
* **Prima versiune a codului** - 12 ore
* **Ultima versiune a codului** - 8 ore
* **Optimizari** - 4 ore
* **Debugging** - 3 ore
* **Comentarii si README** - 3 ore
* **Total** - 30 ore

## Structuri de Date
* **Liste Simplu Înlanțuite** - Gestionarea unităților (`unit`).
* **Liste Dublu Înlanțuite (Santinelă)** - Incidente și Intervenții (`incident`, `intervention`).
* **Cozi (Queues)** - Organizarea unităților disponibile (`queue_available_units`) și a incidentelor de prioritate (`queue_high`, `queue_medium`, `queue_low`).
* **Stivă (Stack)** - Monitorizarea intervențiilor active (`intervention_stack`)pentru operații de tip `UNDO`.

## Comenzi Implementate
* **ADD_INCIDENT** - Adaugă un incident în coada de prioritate corespunzătoare.
* **CHECK_UNITS_AVAILABILITY** - Afiseaza numarul de unitati valabile pentru misiune.
* **DISPATCH**  - Alocă prima unitate disponibilă celui mai prioritar incident.
* **UNDO_LAST_DISPATCH** - Anuleaza ultima comanda de `DISPATCH`.
* **SOLVED_INCIDENT** - Finalizează un incident și eliberează unitatea.
* **SHOW_UNIT/INCIDENT** - Afișează detaliile echipajului după ID.
* **SHOW_INCIDENT** - Afiseaza detaliile incidentului dupa ID.
* **SHOW_INTERVENTIONS** - Listează istoricul tuturor intervențiilor.
* **FUNCTIILE AUXILIARE** - functii suplimentare pentru a scrie codul mai compact si mai complet.

## Nota de Final
* **Time Management** - Codul a durat mai mult decat am anticipat, din cauza ca nu am inteles initial cerinta (am implementat fara cozi si stive).
* **Cele Doua Versiune** - Pentru ca am implementat de la inceput gresit, a durat mai mult sa adaug cozile si stivele, a fost o experienta.
* **Input si Output** - Codul ia inputul din tema1.in si afiseaza rezultate in fisierul tema1.out.
