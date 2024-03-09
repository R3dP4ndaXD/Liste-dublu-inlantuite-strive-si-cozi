/*IACOB Andrei - 313CB*/
#include "functii.h"

int main() {
    TBanda *b;
    TCoada *op;
    TStiva un = InitS(), re = InitS();
    b = InitBanda();
    if (!b) {
        return -1;
    }
    op= InitQ();
    if (!op) {
        DistrugeB(&b);
        return -1;
    }
    FILE *fi = fopen("tema1.in", "r");
    FILE *fo = fopen("tema1.out", "w");
    int n, i;
    char *line = malloc(LINE_SIZE * sizeof(char));
    if (!line) {
        DistrugeQ(&op);
        DistrugeB(&b);
        return -1;
    }
    /*
        La cititea din fisier, operatiile sunt interpretate astfel:
        -SHOW si SHOW_CURRENT sunt executate direct
        -MOVE, INSERT si WRITE sunt introduse in coada de operatii
        -EXECUTE extrage o operatie din coada, o interpreteaza si daca aceasta se executa cu succes,
         fie se salveaza vechea pozitie a degetului in stiva de undo(in cazul operatiilor de move), 
         fie se golesc stivele de undo si redo(in cazul operatiilor de insert si write)
        -UNDO si REDO sunt executate direct
    */
    fscanf(fi, "%i\n", &n);
    char cmd[LINE_SIZE - 2], c = '\0';
    for (i = 0; i < n && fgets(line, LINE_SIZE, fi); i++) {
        if (strstr(line, "SHOW_CURRENT")) {
            SHOW_CURRENT(b, fo);
        } else if (strstr(line, "SHOW")) {
            SHOW(b, fo);
        } else if (strstr(line, "MOVE") || strstr(line, "INSERT") || strstr(line, "WRITE")) { 
            if (!IntrQ(op, line)) {
                continue;
            } 
        } else if (strstr(line, "EXECUTE")) {
            if (!ExtQ(op, line)) {
                continue;
            }
            c = '\0';
            sscanf(line, "%s %c", cmd, &c);
            if (strstr(line, "MOVE_LEFT")) {
                int no_err;
                TLista2 temp = b->crt;
                if (c == '\0') {
                    no_err = MOVE_LEFT(b);
                    if (no_err) {
                        Push(&un,temp);
                    }
                } else {
                    no_err = MOVE_LEFT_CHAR(b, c, fo);
                }   
            } else if (strstr(line, "MOVE_RIGHT")) {
                int no_err;
                TLista2 temp = b->crt;
                if (c == '\0') {
                    no_err = MOVE_RIGHT(b);
                    if (no_err) {
                        Push(&un,temp);
                    }
                } else {   
                    no_err = MOVE_RIGHT_CHAR(b, c);
                }  
            } else if (strstr(line, "INSERT_LEFT")) {
                if (INSERT_LEFT(b, c, fo)) {
                    DistrugeS(&un);
                    DistrugeS(&re);
                } else {
                    //
                }
            } else if (strstr(line, "INSERT_RIGHT")) {
                if (!INSERT_RIGHT(b, c)){
                    DistrugeS(&un);
                    DistrugeS(&re);
                } else {
                    //
                }
            } else if (strstr(line, "WRITE")) {
                WRITE(b, c);
                DistrugeS(&un);
                DistrugeS(&re);
            }
        } else if (strstr(line, "UNDO")) { 
                UNDO(b, &un, &re);

        } else if (strstr(line, "REDO")) {
                REDO(b, &un, &re);
        }

    }
    //AfisareCoada(op);
    
    // eliberare memorie
    if (un) {
        DistrugeS(&un);
    }
    if (re) {
        DistrugeS(&re);
    }
    free(line);
    DistrugeQ(&op);
    DistrugeB(&b);
    fclose(fi);
    fclose(fo);
    return 0;
}