/*Tararache Diana Cristina 312CB*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct satelit {
	int frecventa;
	char *nume;
} TSatelit;

typedef struct Node {
	TSatelit satelit;
	struct Node *st, *dr;
} TNode, *TArb;

typedef struct Heap {
    int nrMax, nrElem;
    TArb *v;
} THeap;

THeap* alocHeap(int nrMax);
void insertMinHeap(THeap* h, TNode* nod);
int comp(TArb a, TArb b);
TArb extragereHeap(THeap* h);
TArb contruiecteArb(THeap* h);
int calcNivel(TArb radacina);
void afisareNivelArb(TArb arb, int level, FILE *fout);
void afisareArb(TArb arb, FILE *fout);
void distrugereArb(TArb arb);
void decode(TArb arb, char codif[], FILE *fout);
int code(TArb arb,const char *nume, char *codif, int level, FILE *fout);
void findPath(TArb arb, const char *nume, TNode **paths, int level, int *ok);
void findParent(TArb arb, char **nume, int nr_sateliti, FILE *fout);