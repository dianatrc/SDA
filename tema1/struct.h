/*Tararache Diana Cristina 312CB*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int id;
	char url[50];
	char *description;
} TPage;

// definirea unei liste simplu inlantuita (stiva)
typedef struct TNode{
	TPage *page;
	struct TNode *urm;
} TNode, *TStack;

typedef struct {
	int id;
	TPage *currentPage;
	TStack backwardStack;
	TStack forwardStack;
} TTab;

// definirea unei liste dublu inlantuita circulara cu santinela
typedef struct TCell{
	TTab *info;
	struct TCell *urm;
	struct TCell *ant;
} TCell;

typedef struct {
	struct TCell *santinela;
} TTabsList;

typedef struct {
	TTab *current;
	TTabsList *list;
} TBrowser;

TPage* alocare_page(int id, char url[], char *description);
TStack alocare_stiva();
TTabsList* alocare();
TBrowser* alocare_browser(TPage* pag_implicita);
void NEW_TAB(TBrowser* browser, int *x, TPage *pag_implicita);
void PRINT(TBrowser* browser, FILE *file_out);
void CLOSE(TBrowser* browser, FILE *file_out, int *x);
void OPEN(TBrowser* browser, char line[], FILE *file_out);
void NEXT(TBrowser* browser);
void PREV(TBrowser* browser);
void push(TStack *stack, TPage *page);
void PAGE(TBrowser* browser, char line[], FILE *file_out, TPage* pages[100], int nr_pagini);
void PRINT_HISTORY(TBrowser* browser, char line[], FILE *file_out);
void BACKWARD(TBrowser* browser, FILE *file_out);
void FORWARD(TBrowser* browser, FILE *file_out);
void free_page(TPage *page);
void free_stack(TStack *stack, int free_pages);
void free_tab(TTab *tab);
void free_browser(TBrowser *browser);
void inversare(TStack *stack, TStack *aux);
TPage* pop(TStack* stack);