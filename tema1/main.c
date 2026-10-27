/* Tararache Diana Cristina 312CB */

#include "struct.h"

int main() {
    FILE *file_in = fopen("tema1.in", "r");
    FILE *file_out = fopen("tema1.out", "w");
    char line[256];
    int nr_pagini = 0, id = 0, x = 0;
    char url[50], descriere[100];

    // alocam si initializam pagina implicita
    TPage *pag_implicita = malloc(sizeof(TPage));
    pag_implicita->id = 0;
    strcpy(pag_implicita->url, "https://acs.pub.ro/");
    pag_implicita->description = malloc(20 * sizeof(char));
    strcpy(pag_implicita->description, "Computer Science");
    TPage *pages[100];  // maxim 50 pagini
    TBrowser *browser = alocare_browser(pag_implicita);
    fgets(line, sizeof(line), file_in);
    nr_pagini = atoi(line);
    // alocam de populam vectorul de pointeri pages
    for (int i = 0; i < nr_pagini; i++) {
        fgets(line, sizeof(line), file_in);
        id = atoi(line);
        fgets(line, sizeof(line), file_in);
        strcpy(url, line);
        url[strcspn(url, "\n")] = 0;  // eliminam '/n' in cazul in care exista
        fgets(line, sizeof(line), file_in);
        strcpy(descriere, line);
        descriere[strcspn(descriere, "\n")] = 0;
        pages[i] = alocare_page(id, url, descriere);
    }
    fgets(line, sizeof(line), file_in);
    int nr_comenzi = atoi(line);
    for (int i = 0; i < nr_comenzi; i++) {
        fgets(line, sizeof(line), file_in);
        if (strstr(line, "NEW_TAB") != 0)
            NEW_TAB(browser, &x, pag_implicita);
        else if (strstr(line, "HISTORY") != 0)
            PRINT_HISTORY(browser, line, file_out);
        else if (strstr(line, "PRINT") != 0)
            PRINT(browser, file_out);
        else if (strstr(line, "CLOSE") != 0)
            CLOSE(browser, file_out, &x);
        else if (strstr(line, "OPEN") != 0)
            OPEN(browser, line, file_out);
        else if (strstr(line, "NEXT") != 0)
            NEXT(browser);
        else if (strstr(line, "PREV") != 0)
            PREV(browser);
        else if (strstr(line, "PAGE") != 0)
            PAGE(browser, line, file_out, pages, nr_pagini);
        else if (strstr(line, "BACKWARD") != 0)
            BACKWARD(browser, file_out);
        else if (strstr(line, "FORWARD") != 0)
            FORWARD(browser, file_out);
    }
    // eliberare browser
    free_browser(browser);
    // eliberare vectorul de pointeri pagini
    for (int i = 0; i < nr_pagini; i++)
        free_page(pages[i]);
    // eliberare pagina implicita
    free(pag_implicita->description);
    free(pag_implicita);
    fclose(file_in);
    fclose(file_out);
    return 0;
}