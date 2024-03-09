/*IACOB Andrei - 313CB*/
#ifndef FUNCTII_H
#define FUNCTII_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SANTI '|'
#define LINE_SIZE 40

typedef struct celula {
    char *info;
    struct celula *urm;
} TCelula, *TLista;

typedef struct celula2 {
    char info;
    struct celula2 *pre, *urm;
} TCelula2, *TLista2;

typedef struct celulastiva {
    TLista2 info;
    struct celulastiva *urm;
} TCelulaStiva, *TStiva;

typedef struct{
    TLista inc, sf;
} TCoada;

typedef struct {
    TLista2 s, crt;
} TBanda;

TBanda* InitBanda();
int ExtindBanda(TBanda *b);
int MOVE_LEFT(TBanda *b);
int MOVE_RIGHT(TBanda *b);
int MOVE_LEFT_CHAR(TBanda *b, char c, FILE *fo);
int MOVE_RIGHT_CHAR(TBanda *b, char c);
void WRITE(TBanda *b, char c);
int INSERT_LEFT(TBanda *b, char c, FILE *fo);
int INSERT_RIGHT(TBanda *b, char c);
void SHOW_CURRENT(TBanda *b, FILE *fo);
void SHOW(TBanda *b, FILE *fo);
TCoada* InitQ();
int IntrQ(TCoada *c, char *line);
int ExtQ(TCoada *c, char *line);
void AfisareCoada (TCoada *c);
TStiva InitS();
TStiva AlocCelulaStiva(TLista2 info);
int Push(TStiva *vf, TLista2 elem);
int Pop(TStiva *vf, TLista2 *elem);
void DistrugeS(TStiva *vf);
void DistrugeQ(TCoada **c);
void DistrugeB(TBanda **b);
void UNDO(TBanda *b, TStiva *un, TStiva *re);
void REDO(TBanda *b, TStiva *un, TStiva *re);

#endif