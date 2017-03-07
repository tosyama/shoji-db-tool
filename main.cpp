//
//  main.cpp
//  ShogiDBTool
//
//  Created by tosyama on 2014/12/28.
//  Copyright (c) 2016 tosyama. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "shogiban.h"
#include "kyokumencode.h"
#include "sashite.h"
#include "kifu.h"
#include "shogidb.h"
#include "shogirule.h"

static int interactiveCUI(ShogiKykumen *shogi, Sashite *s);

FILE *logf = NULL;
int main(int argc, const char * argv[]) {
    Kifu kifu;
    ShogiKykumen shogi;
    char code[KykumenCodeLen];
    logf = fopen("shogidbtool.log", "w");

    resetShogiBan(&shogi);
    koma_move(&shogi, 3,3,3,4,0);
    
    Sashite s[200];
    int n;
    createSashite(&shogi, 0, s, &n);
    
    Sashite si[200];
    si[0].type = SASHITE_RESULT;
    int i=0;
    int cmd;
    while ((cmd = interactiveCUI(&shogi, &si[i]))) {
        i+=cmd;
        if (i<0) {i=0; si[0].type = SASHITE_RESULT;}
        if (cmd>0) {si[i]=si[i-1];}
        createSashite(&shogi, 0, s, &n);
        fprintf(logf, "手の数: %d\n", n);

        for (int j=0; j<n; j++) {
            sasu(&shogi, &s[j]);
            printKyokumen(logf, &shogi);
            temodoshi(&shogi, &s[j]);
        }

    }
    readKIF("test.kif", &kifu);
/*    Sashite* sashite = kifu.sashite;
    
    int i = 0;
    while (sasu(&shogi, &sashite[i]) < 0) {
        printKyokumen(&shogi);
        createKyokumenCode(code, &shogi, i%2);
        printf("code %d: %s\n", i, code);
        i++;
    }
    
    if (sashite[i].result.winner == 0) {
        printf("%d手で先手の勝ち\n", kifu.tesuu);
    } else if (sashite[i].result.winner == 1) {
        printf("%d手で後手の勝ち\n", kifu.tesuu);
    } else if (sashite[i].result.winner == 2) {
        printf("千日手\n");
    } else {
        assert(0);
    }

    createShogiDB("shogiDb.db");
    char fname[256];
    for (int i=1; i<=10; i++) {
        sprintf(fname, "/Users/tos/Documents/Dev/ShogiDBTool/KifuData/%08d.kif",i);
        printf("%s\n", fname);
        if (readKIF(fname, &kifu) > 0)
            insertShogiDB("shogiDb.db", &kifu);
    } */
    fclose(logf);
    return 0;
}

static int interactiveCUI(ShogiKykumen *shogi, Sashite *s)
{
    Koma (*shogiBan)[BanX] = shogi->shogiBan;
    char buf[80];
    int fx, fy, tx, ty;
    printKyokumen(stdout, shogi);
    
    while (1) {
        printf("move:1-9 1-9 1-9 1-9 + uchi:[11-17 or 21-27] 1-0 1-9\ntemodoshi:-1, print: p end:0,\n>");
        if(fgets(buf,80,stdin)) {
            if (buf[0] >= '1' && buf[0] <= '9') { // move
                fx = buf[0] - '0'; fy = buf[1] - '0';
                tx = buf[2] - '0'; ty = buf[3] - '0';
                if(fx >= 1 && fx <=9 && fy >= 1 && fy <= 9 && tx >=1 && tx <= 9 && ty >= 1 && ty <= 9) {
                    s->type = SASHITE_IDOU;
                    s->idou.from_x = INNER_X(fx);
                    s->idou.from_y = INNER_Y(fy);
                    s->idou.to_x = INNER_X(tx);
                    s->idou.to_y = INNER_Y(ty);
                    
                    if(buf[4] == '+')
                        s->idou.nari = 1;
                    else
                        s->idou.nari = 0;
                    
                    if (shogiBan[s->idou.from_y][s->idou.from_x] != EMP) {
                        sasu(shogi,s);
                        return 1;
                    }

                }
            } else if (buf[0] == '-') {
                temodoshi(shogi, s);
                return -1;
            } else if (buf[0] == '0') { // end
                return 0;
            } else if (buf[0] == 'p') { // print
                printKyokumen(stdout, shogi);
            }
        } else {
            return 0;
        }
    }
    
    return 0;
}
