# README - Tema 1 SDA

## Structuri de date
`TPage` - reprezinta o pagina web, are trei campuri: id, url si descriere.
`TNode, TStack` - reprezinta stive pentru navigarea inapoi (backward) si inainte (forward) intr-un tab (Tstack este acelasi lucru cu TNode*, reprezentand varful stivei).
`TTab` - reprezinta un tab dintr-un browser, are 4 campuri: id, pagina curenta si doua stive pentru navigare: backward si forward.
`TCell, TTabsList` - reprezinta o lista dublu inlantuita circulara cu santinela, folosita pentru a gestiona toate tab-urile deschide intr-un browser.
`TBrowser` - structura principala ce reprezinta browser-ul.

## Declarari si alocari
- `alocare_browser`: aloca un browser cu o lista de tab-uri si un tab curent, pagina implicita va fi setata ca fiind pagina curenta a tab-ului.
- `alocare_stiva`: creeaza o stiva goala pentru istoricul paginilor.
- `alocare_page`: aloca si initializeaza o pagina, o sa fie folosita pentru a crea vectorul de paginii.
- `alocare_lista`: aloca o lista dublu inlantuita circulara cu santinela.

## `NEW_TAB`
Functia `NEW_TAB` creeaza un tab nou in browser si il adauga la lista de tab-uri, setandu-l ca tab-ul curent. Daca variabila x este 0, ID-ul noului tab va fi calculat prin incrementarea ID-ului ultimului tab. Daca x este diferit de 0, ID-ul noului tab va fi setat cu ajutorul lui x. Dupa aceea, x este resetat la 0. Pagina implicita va fi setata ca pagina curenta a tab-ului nou, iar stivele pentru istoric vor fi initializate pentru acest tab.

## `CLOSE`
Functia `CLOSE` are rolul de a inchide tab-ul curent din browser, eliminand-l din lista de taburi si eliberand memoria asociata acestuia. Tab-ul din stanga devine tab-ul curent. Daca se incearca inchiderea tab-ului cu ID 0, se va afisa un mesaj de eroare (`403 Forbidden`). Daca tab-ul curent este ultimul tab din lista, x va fi setat la ID-ul tab-ului curent + 1.

## `OPEN <ID>`
Funcția `OPEN` are rolul de a schimba tab-ul curent al browserului într-un tab existent în lista de taburi, identificat după un ID specificat.
Funcția începe prin a citi linia de comandă, extragand ID-ul tab-ului dorit folosind funcția sscanf(). Dacă citirea nu reușește, se va afișa un mesaj de eroare și funcția va returna. Se parcurge lista de taburi de la început, comparând ID-ul fiecărui tab cu ID-ul specificat. Dacă tab-ul cu ID-ul căutat este găsit, tab-ul curent va fi setat la acest tab. Dacă nu se găsește niciun tab cu ID-ul specificat (adică ajungem la celula santinelă fără a găsi tab-ul), funcția va scrie un mesaj de eroare în fișierul de ieșire(`403 Forbidden`).

## `NEXT`
Funcția `NEXT` are rolul de a schimba tab-ul curent al browserului la tab-ul care se află imediat după tab-ul curent în lista de taburi. Dacă tab-ul curent este ultimul tab din listă, atunci tab-ul curent va fi setat la primul tab din listă. Funcția începe prin a parcurge lista de taburi pentru a găsi celula care conține tab-ul curent. Aceasta se face prin comparația fiecărui tab din listă cu tab-ul curent. Dacă tab-ul curent nu este ultimul din listă, atunci funcția va seta tab-ul curent la tab-ul care urmează în listă. Dacă tab-ul curent este ultimul din listă, atunci tab-ul curent va fi setat la primul tab din listă.

## `PREV`
Funcția `PREV` are rolul de a schimba tab-ul curent al browserului la tab-ul care se află imediat înaintea tab-ului curent în lista de taburi. Dacă tab-ul curent este primul tab din listă, atunci tab-ul curent va fi setat la ultimul tab din listă. Asemanator functiei `NEXT`, funcția începe prin a parcurge lista pentru a găsi celula care conține tab-ul curent (compara fiecare tab din lista cu cel curent). Dacă tab-ul curent nu este primul din listă, funcția setează tab-ul curent la tab-ul care precede în listă (adică tab-ul anterior celui curent). Dacă tab-ul curent este primul din listă, atunci tab-ul curent va deveni ultimul tab din listă.

## `PAGE <ID>`
Funcția `PAGE` schimba pagina curenta din tab-ul browserului. Aceasta caută o pagină cu ID-ul specificat de utilizator, iar dacă o găsește, o setează ca pagină curentă și adaugă pagina curentă în stiva "Backward". Funcția extrage ID-ul paginii din linia de comandă (`PAGE <ID>`), folosind funcția sscanf(). Dacă linia nu poate fi citită corect, se afișează un mesaj de eroare. Se caută pagina cu ID-ul specificat în vectorul de pagini (pages[]). Dacă pagina este găsită, se setează ca pagină curentă, iar pagina curentă va fi adăugată în stiva "Backward". Stiva "Forward" va fi golită. Dacă nu există o pagină cu ID-ul cerut, se afișează un mesaj de eroare (`403 Forbidden`).

## `BACKWARD`
Funcția `BACKWARD` se ocupă cu navigarea înapoi în istoria paginilor, folosind stiva "Backward" a tab-ului curent. Dacă există pagini anterioare în stiva "Backward", aceasta va extrage ultima pagină adăugată, o va seta ca pagină curentă și va pune pagina curentă în stiva "Forward". Dacă stiva "Backward" este goală (adică nu există pagini anterioare de accesat), se afișează un mesaj de eroare (`403 Forbidden`). Dacă stiva conține pagini, funcția scoate ultima pagină adăugată din stiva "Backward" folosind funcția pop și o salvează într-o variabilă page. Înainte de a schimba pagina curentă, funcția pune pagina curentă în stiva "Forward", pentru a permite revenirea la aceasta mai târziu. Pagina extrasă din stiva "Backward" este setată ca pagină curentă a browserului.

## `FORWARD`
Funcția `FORWARD` se ocupă cu navigarea înainte în istoria paginilor, folosind stiva "Forward" a tab-ului curent. Dacă există pagini viitoare în stiva "Forward", aceasta va extrage ultima pagină adăugată, o va seta ca pagină curentă și va pune pagina curentă în stiva "Backward".
Dacă stiva "Forward" este goală (adică nu există pagini viitoare de accesat), se afișează un mesaj de eroare (`403 Forbidden`). Dacă stiva conține pagini, funcția scoate ultima pagină adăugată din stiva "Forward" folosind funcția pop și o salvează într-o variabilă page. Înainte de a schimba pagina curentă, funcția pune pagina curentă în stiva "Backward", pentru a permite revenirea la aceasta mai târziu. Pagina extrasă din stiva "Forward" este setată ca pagină curentă a browserului.

## `PRINT`
Funcția `PRINT` este utilizată pentru a afisa informații despre tab-urile din browserul curent. Aceasta va parcurge lista de taburi, va afișa ID-urile acestora, va sări peste celula „santinela” și va afisa descrierea paginii curente în tab-ul activ. Functia parcurge lista de taburi a browserului. Odată ajuns la tab-ul curent, funcția va începe să afișeze ID-urile taburilor din lista de taburi, sărind peste celula santinela. După ce au fost afișate ID-urile taburilor, funcția va afisa descrierea paginii curente a tabului activ.

## `PRINT_HISTORY`
Funcția `PRINT_HISTORY` afișează istoricul paginilor vizitate pentru un anumit tab, inclusiv paginile accesate anterior și cele viitoare (din stivele Backward și Forward). Se extrage ID-ul tabului pentru care dorim să afișăm istoricul din linia de comandă, folosind sscanf. Se parcurge lista de taburi pentru a găsi tabul cu ID-ul specificat. Dacă nu există, se va afișa un mesaj de eroare. Pentru a afișa paginile din stiva Forward în ordinea în care au fost accesate, se inversează stiva Forward. Aceasta se face prin crearea unei copii a stivei și apoi inversarea acesteia. Se va afișa istoricul paginilor vizitate din stiva Forward (dacă există), urmat de pagina curentă, și apoi paginile din stiva Backward (dacă există). După ce istoricul este afișat, se eliberează memoria alocată pentru stiva inversată.

## Eliberarea memoriei
- `free_page()`: elibereaza memoria asociata unei pagini (se va folosi pentru a elibera vectorul de pagini).
- `free_stack()`: elibereaza memoria asociata unei stive.
- `free_tab()`: elibereaza memoria unui tab.
- `free_list()`: elibereaza memoria unei liste.
- `free_browser()`: elibereaza memoria unui browser.
Pagina implicita va fi eliberata in int main() la final.

## Functii auxiliare
- inversare(): inverseaza ordinea elementeor dintr-o stiva - stiva rasturnata se va afla in stiva auxiliara, care este transmisa prin parametru.
- pop(): returneaza ultimul element adaugat intr-o stiva si il elimina, eliberand memoria.