/* IACOB Andrei - 313CB */
#include "functii.h"

/*  
    Aloca si initializa memoria pentru structura banda, pentru santinela si elementul 1 (elementul curent) 
    din lista dublu inlantuita(cu verificarea fiecarei alocari) 
*/
TBanda* InitBanda() {
    TBanda *b = malloc(sizeof(TBanda));
    if (!b) {
        return NULL;
    }
    b->s = malloc(sizeof(TCelula2));
    if (!b->s) {
        free(b);
        b = NULL;
        return NULL;
    }
    b->s->info = SANTI;
    b->s->pre = NULL;
    b->s->urm = malloc(sizeof(TCelula2));
    if (!b->s->urm) {
        free(b->s);
        free(b);
        b = NULL;
        return NULL;
    }
    b->crt = b->s->urm;
    b->crt->pre = b->s;
    b->crt->urm = NULL;
    b->crt->info = '#';
    return b;
}
/* 
    Cu degetul aflat pe ultimul element, aloca o noua celula in banda fara a muta degetul pe aceasta 
*/
int ExtindBanda(TBanda *b) {
    TLista2 aux = malloc(sizeof(TCelula2));
    if (!aux) {
        return 0;
    }
    aux->info = '#';
    aux->pre = b->crt;
    aux->urm = NULL;
    b->crt->urm = aux;
    //b->crt=aux;
    return 1;
}

int MOVE_LEFT(TBanda *b) {
    if (b->crt->pre == b->s) {
        return 0;
    }
    b->crt = b->crt->pre;
    
    return 1;
}
int MOVE_RIGHT(TBanda *b) {
    if (b->crt->urm == NULL) {
        // daca nu se poate extinde banda, nu se mai intampla nimic
        if (!ExtindBanda(b)) {
            return 0;
        }
    }
    b->crt = b->crt->urm;
    return 1;
}

/*
    Parcurge banda de la pozitia degetului catre stanga pana cand gaseste caracterul si muta degetul pe acesta.
    Daca caracterul nu este gasit pana se ajunge la santinela, apare mesajul "ERROR" si pozitia degetului ramana nemodificata.
*/
int MOVE_LEFT_CHAR(TBanda *b, char c, FILE *fo) {
    TLista2 p;
    for (p = b->crt; p != b->s; p = p->pre) {
        if (p->info == c) {
            break;
        }
    }
    if (p != b->s) {
        b->crt = p;
        return 1;
    } else {
        fprintf(fo,"ERROR\n");
        return 0;
    }
}

/*  
    Parcurge banda de la pozitia degetului la dreapta pana cand gaseste caracterul, aducand mereu degetul cu un pas in urma parcurgerii.
    Daca se ajunge cu parcurgerea la finalul listei si caracterul nu este gasit, se aloca o noua celula si se iese din for.
    In ambele cazuri, in final degetul avanseaza la drepta pe celula gasita/nou adaugata.  
*/
int MOVE_RIGHT_CHAR(TBanda *b, char c) {
    TLista2 p, temp = b->crt;
    for (p = b->crt; p != NULL; b->crt = p, p = p->urm) {
        if (p->info == c) {
            break;
        }
    }
    if (p == NULL) {
        if (!ExtindBanda(b)) {
            //aduce degetul la pozitia initiala
            b->crt = temp;
            return 0;
        }
        p = b->crt->urm;
    }
    b->crt = p;
    return 1;
}

void WRITE(TBanda *b, char c) {
    b->crt->info = c;
}

/*
    Se muta degetul la stanga, se aloca o noua celula cu caracterul specificat, se refac legaturile pentru a fi inserata la dreapta
    degetului, si se aduce degetul pe celula nou adaugata.
    Daca degetul se afla chiar pe elementul 1, mutarea initiala la stanga esueaza, se afiseaza "ERROR" si functia se opreste.
*/
int INSERT_LEFT(TBanda *b, char c, FILE *fo) {
    if (!MOVE_LEFT(b)) {
        fprintf(fo, "ERROR\n");
        return 0;
    } else {
        TLista2 aux = malloc(sizeof(TCelula2));
        if (!aux) {
            // se revine in pozitia initiala
            MOVE_RIGHT(b);
            return 0;
        }
        aux->info = c;
        aux->pre = b->crt;
        aux->urm = b->crt->urm;
        b->crt->urm = aux;
        aux->urm->pre = aux;
        b->crt = aux;
        return 1;
    }
}

/*
    Se aloca o noua celula cu caracterul specificat, se refac legaturile pentru a fi inserat la dreapta degetului
    si se aduce degetul pe celula nou adaugata.
*/
int INSERT_RIGHT(TBanda *b, char c) {
    TLista2 aux = malloc(sizeof(TCelula2));
    if (!aux) {
        return 0;
    }
    aux->info = c;
    aux->pre = b->crt;
    aux->urm = b->crt->urm;
    b->crt->urm = aux;
    // daca aux nu se insereaza la finalul listei
    if (aux->urm) {
        aux->urm->pre = aux;
    }
    b->crt = aux;
    return 1;
}

void SHOW_CURRENT(TBanda *b, FILE *fo) {
    fprintf(fo, "%c\n", b->crt->info);
}

/*
    Se parcuge lista de la elementul 1, se afiseaza direct fiecare caracter, iar caracterul din dreptul degetului se afiseaza evidentiat 
*/
void SHOW(TBanda *b, FILE *fo) {
    TLista2 p;
    for (p = b->s->urm; p; p = p->urm) {
        if(p != b->crt) {
           fprintf(fo, "%c", p->info);
        } else {
            fprintf(fo, "|%c|", p->info);
        }
    }
    fprintf(fo, "\n");
}

// Aloca memorie pentru coada vida
TCoada* InitQ() {
    TCoada *c= malloc(sizeof(TCoada));
    if (!c) {
        return NULL;
    }
    c->inc = NULL;
    c->sf = NULL;
    return c;
}


// Aloca celula pentru operatie si o leaga la finalul cozii
int IntrQ(TCoada *c, char *line) {
    TLista aux = malloc(sizeof(TCelula));
    if (!aux) {
        return 0;
    }
    aux->info = strdup(line);
    if (!aux->info) {
        free(aux);
        aux = NULL;
        return 0;
    }
    aux->urm = NULL;
    /*  
        Daca coada nu e vida, aux se leaga de ultima celula si devine noul ultim;
        Daca coada e vida, aux devine ultima si ultima celula din coada(lista);
    */
    if (c->sf) {
        c->sf->urm = aux;
    } else {
        c->inc = aux; 
    }
    c->sf = aux;
    return 1;
}

/*
    Salveaza operatia de la inceputul cozii in variabila primita ca parametru
    Pastreaza un pointer la celula de inceput a cozii, avanseaza inceputul cozii si elibereaza prima celula
*/
int ExtQ(TCoada *c, char *line) {
    if (!c->inc) {
        // nu aven ce extrage
        return 0;
    }
    TLista aux = c->inc;
    strncpy(line, aux->info, LINE_SIZE);
    c->inc = aux->urm;
    // daca coada avea doar un element, acum a ajuns vida
    if (!c->inc) {
        c->sf = c->inc;
    }
    free(aux->info);
    free(aux);
    return 1;
}

// Parcurge coada si afiseaza fiecare operatie
void AfisareCoada(TCoada *c) {
    TLista p = c->inc;
    while (p) {
        fputs(p->info, stdout);
        p = p->urm;
    }
}

// Pentru simetrie cu InitQ()
TStiva InitS() {
    return NULL;
}


// Aloca si initializeaza celula de stiva
TStiva AlocCelulaStiva(TLista2 crt) {
    TStiva aux = malloc(sizeof(TCelulaStiva));
    if (!aux) {
        return NULL;
    }
    aux->info = crt;
    aux->urm = NULL;
    return aux;
}

// Aloca o celula ce contine un pointer la pozitia curenta, leaga celula in varful stivei, si actualizeaza varful de stiva
int Push(TStiva *vf, TLista2 crt) {
    TStiva aux = AlocCelulaStiva(crt);
    if (!aux) {
        return 0;
    }
    aux->urm = *vf;
    *vf = aux;
    return 1;
}

/*
    Salveaza pointerul din varful stivei in variabila primita ca parametru
    Pastreaza un pointer la celula din varful stivei, muta varful stivei si elibereaza prima celula
*/
int Pop(TStiva *vf, TLista2 *crt) {
    if (*vf == NULL) { 
        return 0;
    }
    TStiva aux = *vf;
    *crt = aux->info;
    *vf = aux->urm;
    free(aux);
    return 1;
}

/*
    Se parcuge stiva modificandu-se varful de stiva si se elibereaza mereu prima celula(celula dinaintea varfului)
    La final stiva ajunge automat vida; 
*/
void DistrugeS(TStiva *vf) {
    TStiva aux;
    while (*vf) {
        aux = *vf;
        *vf = (*vf)->urm;
        free(aux);
    }
}

/*
    Se parcuge lista cozii si se elibereaza mereu celula anterioara
    Se elibereaza memoria pentru coada si pointerul la coada devine NULL; 
*/
void DistrugeQ(TCoada **c) {
    TLista p = (*c)->inc, aux;
    while (p) {
        aux = p;
        p = p->urm;
        free(aux->info);
        free(aux);
    }
    free(*c);
    *c = NULL;
}

/*
    Se parcuge lista banzii si se elibereaza mereu celula anterioara
    Se elibereaza memoria pentru banda si pointerul la banda devine NULL; 
*/
void DistrugeB(TBanda **b) {
    TLista2 p = (*b)->s, aux;
    while (p) {           
        aux = p;
        p = p->urm;   
        free(aux);
    }
    free(*b);
    *b = NULL;
}

/* 
    Se salveaza pozitia degetului in varful stivei pentru redu si degetul se muta la pozitia indicata
    in varful stivei pentru undo(pozitie care se scoate din stiva)
*/
void UNDO(TBanda *b, TStiva *un, TStiva *re) {
    Push(re, b->crt);
    Pop(un,&(b->crt));
}

/* 
    Se salveaza pozitia degetului in varful stivei pentru undu si degetul se muta la pozitia indicata
    in varful stivei pentru redo(pozitie care se scoate din stiva)
*/
void REDO(TBanda *b, TStiva *un, TStiva *re) {
    Push(un, b->crt);
    Pop(re,&(b->crt));
}
