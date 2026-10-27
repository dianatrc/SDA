/*Tararache Diana Cristina 312CB*/

#include "struct.h"

TPage *alocare_page(int id, char url[], char *description) {
    TPage *page = (TPage *)malloc(sizeof(TPage));
    if (page == NULL) {
        printf("eroare la alocare pagina");
        return NULL;
    }
    page->id = id;
    strcpy(page->url, url);
    page->description = malloc(strlen(description) + 1);
    if (page->description == NULL) {
        printf("eroare la alocare descriere");
        free(page);
        return NULL;
    }
    strcpy(page->description, description);
    return page;
}

TStack alocare_stiva() {
    TStack stack = NULL;  // stiva este initial goala
    return stack;
}

TTabsList *alocare_lista() {
    TTabsList *list = (TTabsList *)malloc(sizeof(TTabsList));
    if (list == NULL) {
        printf("eroare la alocare lista");
        return NULL;
    }
    list->santinela = (TCell *)malloc(sizeof(TCell));
    if (list->santinela == NULL) {
        printf("eroare la alocare celula santinela");
        free(list);
        return NULL;
    }
    list->santinela->urm = list->santinela;
    list->santinela->ant = list->santinela;
    list->santinela->info = NULL;
    return list;
}

TBrowser *alocare_browser(TPage *pag_implicita) {
    TBrowser *browser = (TBrowser *)malloc(sizeof(TBrowser));
    if (browser == NULL) {
        printf("eroare la alocare browser");
        return NULL;
    }
    browser->list = alocare_lista();
    browser->current = (TTab *)malloc(sizeof(TTab));
    if (browser->current == NULL) {
        printf("eroare la alocare tab curent");
        free(browser->list);
        free(browser);
        return NULL;
    }
    browser->current->id = 0;
    // setam pagina curenta ca fiind cea implicita
    browser->current->currentPage = pag_implicita;
    browser->current->backwardStack = alocare_stiva();
    browser->current->forwardStack = alocare_stiva();
    // legam tab-ul curent de lista de tab-uri
    TCell *new = (TCell *)malloc(sizeof(TCell));
    if (new == NULL) {
        printf("eroare la alocare");
        free(browser->current);
        free(browser->list);
        free(browser);
        return NULL;
    }
    new->info = browser->current;
    new->ant = browser->list->santinela;
    new->urm = browser->list->santinela;
    browser->list->santinela->urm = new;
    browser->list->santinela->ant = new;
    return browser;
}

void NEW_TAB(TBrowser *browser, int *x, TPage *pag_implicita) {
    TCell *new_cell = (TCell *)malloc(sizeof(TCell));
    if (new_cell == NULL) {
        printf("eroare la alocare");
        return;
    }
    TTab *new_tab = (TTab *)malloc(sizeof(TTab));
    if (new_tab == NULL) {
        printf("eroare la alocare");
        free(new_cell);
        return;
    }
    // x-ul o sa fie diferit de 0 in cazul in care am avut un apel CLOSE
    // pentru a ne ajuta sa scriem id-ul tab-ului nou corect
    if ((*x) != 0) {
        new_tab->id = (*x);
        (*x) = 0;
    } else {
        new_tab->id = (browser->list->santinela->ant->info->id) + 1;
    }
    // setam pagina implicita pentru tab-ul curent al tab-ului nou
    new_tab->currentPage = pag_implicita;
    new_tab->backwardStack = alocare_stiva();
    new_tab->forwardStack = alocare_stiva();
    // legam tab ul in lista la final
    new_cell->info = new_tab;
    new_cell->ant = browser->list->santinela->ant;
    new_cell->urm = browser->list->santinela;
    browser->list->santinela->ant->urm = new_cell;
    browser->list->santinela->ant = new_cell;
    browser->current = new_tab;
}

void PRINT(TBrowser *browser, FILE *file_out) {
    TCell *p = browser->list->santinela->urm;
    // parcurgem pana ajungem la tab-ul curent
    while (p != browser->list->santinela && p->info != browser->current)
        p = p->urm;
    fprintf(file_out, "%d ", p->info->id);
    // afisam restul id-urilor sarind peste santinela
    for (TCell *aux = p->urm; aux != p; aux = aux->urm) {
        if (aux == browser->list->santinela) continue;
        fprintf(file_out, "%d ", aux->info->id);
    }
    fprintf(file_out, "\n");
    fprintf(file_out, "%s\n", browser->current->currentPage->description);
}

void CLOSE(TBrowser *browser, FILE *file_out, int *x) {
    // afisam mesajul de eroare in cazul in care vrem sa inchidem tab-ul curent
    if (browser->current->id == 0) {
        fprintf(file_out, "403 Forbidden\n");
        return;
    }
    // in cazul in care inchidem ultimul tab din lista retinem id+1
    if (browser->list->santinela->ant->info->id == browser->current->id)
        (*x) = browser->current->id + 1;
    TCell *p = browser->list->santinela->urm;
    while (p != browser->list->santinela && p->info->id != browser->current->id)
        p = p->urm;
    if (p->ant != browser->list->santinela)
        browser->current = p->ant->info;
    else
        browser->current = browser->list->santinela->ant->info;
    p->ant->urm = p->urm;
    p->urm->ant = p->ant;
    // eliberam stivele si apoi tab-ul cerut
    free_tab(p->info);
    free(p);
}

void OPEN(TBrowser *browser, char line[], FILE *file_out) {
    int ID = 0;
    if (sscanf(line, "OPEN %d", &ID) != 1) {
        printf("eroare la citire");
        return;
    }
    TCell *p = browser->list->santinela->urm;
    // parcurgem lista pana ajungem la tab-ul cautat
    while (p != browser->list->santinela && p->info->id != ID) p = p->urm;
    // daca conditia este indeplinita nu exista tab-ul cu ID-ul cerut
    if (p == browser->list->santinela) {
        fprintf(file_out, "403 Forbidden\n");
        return;
    }
    browser->current = p->info;
}

void NEXT(TBrowser *browser) {
    TCell *p = browser->list->santinela->urm;
    while (p != browser->list->santinela && p->info != browser->current)
        p = p->urm;
    if (p->urm == browser->list->santinela)
        browser->current = browser->list->santinela->urm->info;
    else
        browser->current = p->urm->info;
}

void PREV(TBrowser *browser) {
    TCell *p = browser->list->santinela->urm;
    while (p != browser->list->santinela && p->info != browser->current)
        p = p->urm;
    if (p->ant == browser->list->santinela)
        browser->current = browser->list->santinela->ant->info;
    else
        browser->current = p->ant->info;
}

void push(TStack *stack, TPage *page) {
    // adauga un nod in varful stivei
    TStack new_node = (TStack)malloc(sizeof(TNode));
    if (!new_node) {
        printf("eroare la alocare");
        return;
    }
    new_node->page = page;
    new_node->urm = *stack;
    *stack = new_node;
}

void PAGE(TBrowser *browser, char line[], FILE *file_out, TPage *pages[100],
          int nr_pagini) {
    int ID = 0;
    // extragem ID ul paginii la care vrem sa ajungem
    if (sscanf(line, "PAGE %d", &ID) != 1) {
        fprintf(file_out, "eroare la citire");
        return;
    }
    int ok = 0;
    // parcurgem vectorul de pointeri si cautam pagina ceruta
    for (int i = 0; i < nr_pagini; i++) {
        if (pages[i]->id == ID) {
            ok = 1;
            push(&(browser->current->backwardStack),
                 browser->current->currentPage);
            browser->current->currentPage = pages[i];
            free_stack(&(browser->current->forwardStack), 0);
            break;
        }
    }
    // verficam daca a existat pagina cu ID ul cerut
    if (ok == 0) {
        fprintf(file_out, "403 Forbidden\n");
        return;
    }
}

void PRINT_HISTORY(TBrowser *browser, char line[], FILE *file_out) {
    int ID = 0;
    // extragem ID ul paginii la care vrem sa ajungem
    if (sscanf(line, "PRINT_HISTORY %d", &ID) != 1) {
        fprintf(file_out, "eroare la citire\n");
        return;
    }
    TCell *p = browser->list->santinela->urm;
    while (p != browser->list->santinela && p->info->id != ID) p = p->urm;
    // afisam mesajul de eroare daca nu exista tab-ul cu ID-ul specificat
    if (p == browser->list->santinela) {
        fprintf(file_out, "403 Forbidden\n");
        return;
    }
    TNode *aux = NULL, *copie = p->info->forwardStack;
    inversare(&copie,
              &aux);  // inversam o copie pentru a nu distruge stiva originala
    TNode *temp = aux;
    while (temp) {
        fprintf(file_out, "%s\n", temp->page->url);
        temp = temp->urm;
    }
    free_stack(&(aux), 0);
    fprintf(file_out, "%s\n", p->info->currentPage->url);
    temp = p->info->backwardStack;
    while (temp) {
        fprintf(file_out, "%s\n", temp->page->url);
        temp = temp->urm;
    }
}

void BACKWARD(TBrowser *browser, FILE *file_out) {
    TStack *p = &(browser->current->backwardStack);
    if ((*p) == NULL) {
        fprintf(file_out, "403 Forbidden\n");
        return;
    }
    // scoatem ultima gaina adaugata in stiva Backward si o salvez in page
    TPage *page = pop(p);
    // adaugam pagina curenta in stiva Forward
    push(&(browser->current->forwardStack), browser->current->currentPage);
    // setam pagina salvata ca fiind pagina curenta
    browser->current->currentPage = page;
}

void FORWARD(TBrowser *browser, FILE *file_out) {
    TStack *p = &(browser->current->forwardStack);
    if ((*p) == NULL) {
        fprintf(file_out, "403 Forbidden\n");
        return;
    }
    // scoatem ultima gaina adaugata in stiva Forward si o salvez in page
    TPage *page = pop(p);
    // adaugam pagina curenta in stiva Backward
    push(&(browser->current->backwardStack), browser->current->currentPage);
    // setam pagina salvata ca fiind pagina curenta
    browser->current->currentPage = page;
}

void free_page(TPage *page) {
    if (page == NULL) return;
    free(page->description);
    page->description = NULL;
    free(page);
    page = NULL;
}

void free_stack(TStack *stack, int free_pages) {
    // free_pages o sa fie mereu 0 pentru a nu elibera si paginiile din noduri
    if (stack == NULL || *stack == NULL) return;
    TNode *p = *stack;
    while (p) {
        TNode *urm = p->urm;
        if (free_pages) free_page(p->page);
        free(p);
        p = urm;
    }
    *stack = NULL;
}

void free_tab(TTab *tab) {
    if (tab == NULL) return;
    // al doilea parametru este 0 pentru a nu elibera paginile
    free_stack(&(tab->backwardStack), 0);
    free_stack(&(tab->forwardStack), 0);
    free(tab);
}

void free_list(TTabsList *list) {
    if (list == NULL) return;
    TCell *curent = list->santinela->urm, *urm;
    while (curent != list->santinela) {
        urm = curent->urm;
        free_tab(curent->info);
        free(curent);
        curent = urm;
    }
    free(list->santinela);
    free(list);
}

void free_browser(TBrowser *browser) {
    if (browser == NULL) return;
    free_list(browser->list);
    free(browser);
}

void inversare(TStack *stack, TStack *aux) {
    // in stiva aux o sa rezulte stiva originala, inversata
    while (*stack) push(aux, pop(stack));
}

TPage *pop(TStack *stack) {
    // returneaza ultimul element adaugat si il elimina din stiva
    if (*stack == NULL) return NULL;
    TNode *p = *stack;
    TPage *page = p->page;
    *stack = (*stack)->urm;
    free(p);
    return page;
}