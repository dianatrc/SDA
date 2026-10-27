/*Tararache Diana Cristina 312CB*/

#include "struct.h"

int main(int argc, char *argv[]) {
    char line[256];
    int n = 0;
    if (argc != 4) {
        printf("eroare la citire\n");
        return 0;
    }
    FILE *fin = fopen(argv[2], "r");   // deschidere pentru citire
    FILE *fout = fopen(argv[3], "w");  // deschidere pentru scrierre
    if (!fin || !fout) {
        printf("eroare la deschidere fisiere");
        return 0;
    }
    fgets(line, sizeof(line), fin);
    n = atoi(line);
    THeap *h = alocHeap(n);
    for (int i = 0; i < n; i++) {
        fgets(line, sizeof(line), fin);
        TSatelit s;
        char name[15];
        sscanf(line, "%d %s\n", &s.frecventa, name);
        s.nume = malloc(strlen(name) + 1);
        if (!s.nume) {
            printf("eroare la alocare");
            return 0;
        }
        strcpy(s.nume, name);
        TNode *nod = (TNode *)malloc(sizeof(TNode));
        nod->dr = NULL;
        nod->st = NULL;
        nod->satelit = s;
        insertMinHeap(h, nod);
    }
    TArb arbore;
    arbore = contruiecteArb(h);
    if (strcmp(argv[1], "-c1") == 0) {
        // cerinta 1
        afisareArb(arbore, fout);
    } else if (strcmp(argv[1], "-c2") == 0) {
        // cerinta 2
        char codif[1000];
        fgets(line, sizeof(line), fin);
        int nr_comenzi = atoi(line);
        for (int i = 0; i < nr_comenzi; i++) {
            fgets(line, sizeof(line), fin);
            strcpy(codif, line);
            decode(arbore, codif, fout);
        }
    } else if (strcmp(argv[1], "-c3") == 0) {
        fgets(line, sizeof(line), fin);
        int nr_comenzi = atoi(line);
        char nume[100], codif[1000];
        for (int i = 0; i < nr_comenzi; i++) {
            fgets(line, sizeof(line), fin);
            sscanf(line, "%s\n", nume);
            code(arbore, nume, codif, 0, fout);
        }
    } else if (strcmp(argv[1], "-c4") == 0) {
        fgets(line, sizeof(line), fin);
        int nr_sateliti = atoi(line);
        char **nume = malloc(nr_sateliti * sizeof(char *));
        for (int i = 0; i < nr_sateliti; i++) {
            nume[i] = malloc(100 * sizeof(char));
            fgets(line, sizeof(line), fin);
            sscanf(line, "%s\n", nume[i]);
        }
        findParent(arbore, nume, nr_sateliti, fout);
        for (int i = 0; i < nr_sateliti; i++) free(nume[i]);
        free(nume);
    }
    distrugereArb(arbore);
    free(h->v);
    free(h);
    fclose(fin);
    fclose(fout);
    return 0;
}