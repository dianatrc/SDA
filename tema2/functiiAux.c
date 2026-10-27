/*Tararache Diana Cristina 312CB*/

#include "struct.h"

THeap *alocHeap(int nrMax) {
    THeap *h = (THeap *)malloc(sizeof(THeap));
    if (!h) {
        printf("eroare la alocare");
        return NULL;
    }
    h->v = (TArb *)malloc(nrMax * sizeof(TArb));
    if (!h->v) {
        printf("eroare la alocare");
        free(h);
        return NULL;
    }
    h->nrMax = nrMax;
    h->nrElem = 0;
    return h;
}

void insertMinHeap(THeap *h, TNode *nod) {
    if (h->nrElem == h->nrMax) {
        printf("nu mai e loc in heap");
        return;
    }
    h->v[h->nrElem] = nod;
    h->nrElem++;
    int index = h->nrElem - 1, parent = 0;
    for (index = h->nrElem - 1; index > 0; index = parent) {
        parent = (index - 1) / 2;
        if (h->v[index]->satelit.frecventa > h->v[parent]->satelit.frecventa)
            break;  // cu cat este mai mica frecventa, cu atat este mai aproape
                    // de radacina
        else if (h->v[index]->satelit.frecventa ==
                     h->v[parent]->satelit.frecventa &&
                 strcmp(h->v[index]->satelit.nume, h->v[parent]->satelit.nume) >
                     0)
            break;  // in stanga o sa fie plasat primul in ordine alfabetica
        TNode *aux = h->v[parent];
        h->v[parent] = h->v[index];
        h->v[index] = aux;
    }
}

int comp(TArb a, TArb b) {
    if (a->satelit.frecventa < b->satelit.frecventa) return 1;
    if (a->satelit.frecventa > b->satelit.frecventa) return 0;
    if (strcmp(a->satelit.nume, b->satelit.nume) < 0) return 1;
    return 0;
}

TArb extragereHeap(THeap *h) {
    if (h->nrElem == 0) {
        printf("heap ul este gol");
        return NULL;
    }
    TArb radacina = h->v[0];
    TArb ultim = h->v[h->nrElem - 1];
    h->v[0] = ultim;
    h->nrElem--;
    int index = 0;
    while (1) {
        int st = 2 * index + 1;
        int dr = 2 * index + 2;
        int el_sel = index;
        if (st < h->nrElem && comp(h->v[st], h->v[el_sel]) == 1) el_sel = st;
        if (dr < h->nrElem && comp(h->v[dr], h->v[el_sel]) == 1) el_sel = dr;
        if (el_sel != index) {
            TArb aux = h->v[index];
            h->v[index] = h->v[el_sel];
            h->v[el_sel] = aux;
            index = el_sel;
        } else {
            break;
        }
    }
    return radacina;
}

TArb contruiecteArb(THeap *h) {
    while (h->nrElem > 1) {
        TArb a = extragereHeap(h);
        TArb b = extragereHeap(h);
        TNode *nod = (TNode *)malloc(sizeof(TNode));
        if (!nod) {
            printf("eroare la alocare");
            return NULL;
        }
        nod->satelit.frecventa = a->satelit.frecventa + b->satelit.frecventa;
        if (a->satelit.frecventa < b->satelit.frecventa ||
            (a->satelit.frecventa == b->satelit.frecventa &&
             strcmp(a->satelit.nume, b->satelit.nume) < 0)) {
            // a o sa fie nodul din stanga, b cel din dreapta
            nod->st = a;
            nod->dr = b;
        } else {
            nod->dr = a;
            nod->st = b;
        }
        // alocare dinamica nume
        int lung_st = strlen(nod->st->satelit.nume);
        int lung_dr = strlen(nod->dr->satelit.nume);
        nod->satelit.nume = (char *)malloc(lung_dr + lung_st + 1);
        if (!nod->satelit.nume) {
            printf("eroare la alocare");
            free(nod);
            return NULL;
        }
        strcpy(nod->satelit.nume, nod->st->satelit.nume);
        strcat(nod->satelit.nume, nod->dr->satelit.nume);
        // inseram noul nod in heap
        insertMinHeap(h, nod);
    }
    if (h->nrElem == 1) return h->v[0];
    return NULL;
}

int calcNivel(TArb radacina) {
    if (radacina == NULL) return 0;
    int st = calcNivel(radacina->st);
    int dr = calcNivel(radacina->dr);
    if (st > dr) return 1 + st;
    return 1 + dr;
}

void afisareNivelArb(TArb arb, int level, FILE *fout) {
    if (arb == NULL) return;
    if (level == 0) {
        fprintf(fout, "%d-%s ", arb->satelit.frecventa, arb->satelit.nume);
        return;
    }
    afisareNivelArb(arb->st, level - 1, fout);
    afisareNivelArb(arb->dr, level - 1, fout);
}

void afisareArb(TArb arb, FILE *fout) {
    int level = calcNivel(arb);
    for (int i = 0; i < level; i++) {
        afisareNivelArb(arb, i, fout);
        fprintf(fout, "\n");
    }
}

void distrugereArb(TArb arb) {
    if (!arb) return;
    distrugereArb(arb->st);
    distrugereArb(arb->dr);
    free(arb->satelit.nume);
    free(arb);
}

void decode(TArb arb, char codif[], FILE *fout) {
    TNode *p = arb;
    for (int i = 0; i < strlen(codif); i++) {
        if (codif[i] == '0')
            p = p->st;
        else if (codif[i] == '1')
            p = p->dr;
        // verificam daca e frunza si daca da o afisam
        if (p->st == NULL && p->dr == NULL) {
            fprintf(fout, "%s ", p->satelit.nume);
            p = arb;  // revenim la radacina arborelui
        }
    }
    fprintf(fout, "\n");
}

int code(TArb arb, const char *nume, char *codif, int level, FILE *fout) {
    if (!arb) return 0;
    // verificam daca este frunza
    if (arb->st == NULL && arb->dr == NULL) {
        if (strcmp(arb->satelit.nume, nume) == 0) {
            // este frunza cu numele corect
            codif[level] = '\0';
            fprintf(fout, "%s", codif);
            return 1;
        }
    }
    // verificam fiu stang
    codif[level] = '0';
    if (code(arb->st, nume, codif, level + 1, fout)) return 1;
    // verificam fiu dreapta
    codif[level] = '1';
    if (code(arb->dr, nume, codif, level + 1, fout)) return 1;
    return 0;
}

void findPath(TArb arb, const char *nume, TNode **paths, int level, int *ok) {
    if (!arb || *ok) return;
    paths[level] = arb;
    if (strcmp(arb->satelit.nume, nume) == 0) {
        paths[level + 1] = NULL;
        *ok = 1;
        return;
    }
    findPath(arb->st, nume, paths, level + 1, ok);
    findPath(arb->dr, nume, paths, level + 1, ok);
}

void findParent(TArb arb, char **nume, int nr_sateliti, FILE *fout) {
    int ok = 0;
    int levels[100], level = 0;
    // alocare memorie
    TNode ***paths = malloc(nr_sateliti * sizeof(TNode **));
    if (!paths) {
        printf("eroare la alocare");
        return;
    }
    for (int i = 0; i < nr_sateliti; i++) {
        paths[i] = malloc(100 * sizeof(TNode *));
        if (!paths[i]) {
            printf("eroare la alocare");
            for (int j = 0; j < i; j++) free(paths[j]);
            free(paths);
            return;
        }
        for (int j = 0; j < 100; j++)
            paths[i][j] = NULL;  // initializam cu NULL
    }
    // formam paths unde o sa fie salvat calea de la radacina la fiecare nod
    // satelit dat
    for (int i = 0; i < nr_sateliti; i++) {
        ok = 0;
        findPath(arb, nume[i], paths[i], 0, &ok);
        level = 0;
        while (paths[i][level] != NULL) level++;
        levels[i] = level - 1;  // salvam lungimile cailor
    }
    level = 99999;
    for (int i = 0; i < nr_sateliti; i++) {
        if (levels[i] < level) level = levels[i];
    }
    TNode *parent = NULL;
    for (int i = 0; i <= level; i++) {
        TNode *curent = paths[0][i];
        int ok = 0;
        for (int j = 1; j < nr_sateliti; j++) {
            if (curent != paths[j][i]) {
                ok = 1;
                break;
            }
        }
        if (ok == 1) break;
        parent = curent;
    }
    if (parent) fprintf(fout, "%s\n", parent->satelit.nume);
    // eliberarea memorie
    for (int i = 0; i < nr_sateliti; i++) free(paths[i]);
    free(paths);
}