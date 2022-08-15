#include <stdio.h>
#include <stdlib.h>


/*++++TEMP++++*/
    char TEMP[2][6] = {{'L','E','F','T',0},{'R','I','G','H','T',0}};
    #define LEFT  0
    #define RIGHT 1
    
    char TEMPC[2][5] = {{'R','E','D',0},{'B','L','C','K',0}};
    #define RED  0
    #define BLCK 1

    #define FTHR 0
    #define SIB  1
    #define NPW  2
    #define NPW_ 3
/*++++TEMP++++*/



/*++++ND CLASS ATRIBUTES++++*/
    typedef struct nde{
        int dat;
        struct nde * next[2];
        struct nde * fthr;
        int hgt;
        int color;
    }node;

    typedef node * ndptr;
    node SENTINEL =  {-100, {&SENTINEL,&SENTINEL},&SENTINEL,-100,BLCK};
/*++++ND CLASS ATRIBUTES++++*/                     



/*++++ND CLASS METHODS++++*/
    node crtNode(int key){
        node rtrn;
        rtrn.dat = key;

        rtrn.next[0] = &SENTINEL;
        rtrn.next[1] = &SENTINEL;
        
        rtrn.hgt = 0;
        rtrn.color = RED;

        return rtrn;
    }

    void bind(ndptr son, ndptr fthr){
        printf("bind: son: %d and fthr: %d\n",son->dat,fthr->dat);
        son->fthr = fthr;            
        fthr->next[fthr->dat < son->dat] = son;
        printf("confirmation: fthr: %d LEFT: %d RIGHT: %d\n\n",fthr->dat,fthr->next[LEFT]->dat,fthr->next[RIGHT]->dat);
    }

    void unbind(ndptr son){
        son->fthr->next[son->fthr->dat < son->dat] = &SENTINEL;
        son->fthr = &SENTINEL;
}
/*++++ND CLASS METHODS++++*/



/*++++TR CLASS ATRIBUTES++++*/
    typedef struct tre{
        ndptr root;
    }tree;
/*++++TR CLASS ATRIBUTES++++*/



/*++++TR CLASS UTILITIES++++*/

    //retorna 1 se test for uma folha e 0 se ao menos um dos filhos existir
    int is_leaf(ndptr test){ return (test->next[0] == &SENTINEL) * (test->next[1] == &SENTINEL);}

    //retorna 1 se a direção especificada por way estiver disponivel
    int has_way(ndptr test, int way){return test->next[way] != &SENTINEL;}

    //retorna 1 se ambas as direções tiverem filhos
    int has_both_ways(ndptr test){return (test->next[LEFT] != &SENTINEL) * (test->next[RIGHT] != &SENTINEL);}

    //retorna 1 se test for sentinel 
    int is_sentinel(ndptr test){return test == &SENTINEL;}

    //retorna 1 se a arvore estiver vazia
    int is_empty(tree * test){return test->root == &SENTINEL;}

    //retorna 1 se test tiver um pai
    int has_fthr(ndptr test){return test->fthr != &SENTINEL;}

    //retorna o maior entre 2 inteiros
    int bgst(int A, int B){return A >= B ? A : B;}

    //retorna um char representando a cor
    char gtClr(ndptr bgn){return TEMPC[bgn->color][0];}

    //retorna um inteiro representado a cor
    int Clr(ndptr bgn){return bgn->color;}

    int gtDir(ndptr son, ndptr fthr){return son == fthr->next[RIGHT];}
/*++++TR CLASS UTILITIES++++*/



/*++++TR CLASS BALANCE METHODS++++*/
    void defHgt(ndptr bgn){
        if(is_leaf(bgn)){  
            bgn->hgt = 1;  
            if(has_fthr(bgn)) defHgt(bgn->fthr);
        }
        else{
            int bg = bgst(bgn->next[LEFT]->hgt,bgn->next[RIGHT]->hgt);
            if(bgn->hgt != bg + 1){
                bgn->hgt = 1 + bg;
                if(has_fthr(bgn)) defHgt(bgn->fthr); 
            }
        }        
    }
/*++++TR CLASS BALANCE METHODS++++*/



/*++++TR CLASS ROTATION METHODS++++*/ 
    void RR(ndptr mid, tree * vldte){
        ndptr bgr = mid->fthr;
        unbind(mid);
        bind(mid,bgr->fthr);
        
        if(has_way(mid,RIGHT)) bind(mid->next[RIGHT],bgr);

        if(vldte->root == bgr){//validação da raiz
            vldte->root = mid; 
            vldte->root->color = BLCK;    
        }

        bind(bgr,mid);
    }

    void LL(ndptr mid, tree * vldte){
        ndptr smlr = mid->fthr;
        unbind(mid);
        bind(mid,smlr->fthr);
        
        if(has_way(mid,LEFT)) bind(mid->next[LEFT],smlr);//passagem de guarda
        if(vldte->root == smlr){//validação da raiz
            vldte->root = mid;  
            vldte->root->color = BLCK;
        }

        bind(smlr,mid);
    }

    void LR(ndptr mid, tree * vldte){
        LL(mid,vldte);
        RR(mid,vldte);
    }

    void RL(ndptr mid, tree * vldte){
        RR(mid,vldte);
        LL(mid,vldte);
    }
/*++++TR CLASS ROTATION METHODS++++*/



/*++++TR CLASS RED BLACK METHODS++++*/
 
    void clrSet(ndptr bgn, int clr){
        if(has_fthr(bgn))bgn->color = clr;                  
        if(has_way(bgn,LEFT)) bgn->next[LEFT]->color = !clr;
        if(has_way(bgn,RIGHT)) bgn->next[RIGHT]->color = !clr;
    }

    void vldteRed(ndptr son, tree * t){//corrige nodes vermelhas adjacentes
        if(has_fthr(son)){
            if(son-> color == RED && son->fthr->color == RED){//violação ! 
                
                ndptr grdprt = son->fthr->fthr;
                int aunt_dir = son->fthr->dat < grdprt->dat;
                if(grdprt->next[aunt_dir]->color == RED) clrSet(grdprt,RED);//color-flip quando o tio/tia é vermelho 
                
                else{//rotação quando o tio/tia é preto  
                    int son_dir = son->fthr->dat < son->dat;
                    int fthr_dir = !aunt_dir;

                    if(fthr_dir == LEFT && son_dir == LEFT){
                        RR(son->fthr,t);
                        clrSet(son->fthr,BLCK);
                    }
                    else if(fthr_dir == RIGHT && son_dir == RIGHT){
                        LL(son->fthr,t);
                        clrSet(son->fthr,BLCK);
                    } 
                    else if(fthr_dir == LEFT && son_dir == RIGHT){
                        LR(son,t);
                        clrSet(son,BLCK);
                    }
                    else if(fthr_dir == RIGHT && son_dir == LEFT){
                        RL(son,t);
                        clrSet(son,BLCK);
                    }
                }
            }
            vldteRed(son->fthr,t);//corrige até a raiz
        }
    }

/*++++TR CLASS RED BLACK METHODS++++*/



/*++++TR CLASS BASIC METHODS++++*/ 
    void prtTree(ndptr bgn){
        if(!is_sentinel(bgn)){
            printf("\n  fthr: %d(%d)(%d)(%c)\n\n  left: %d(%d)   right: %d(%d)\n===>\n",bgn->dat,bgn->fthr->dat,bgn->hgt,TEMPC[bgn->color][0],bgn->next[LEFT]->dat,bgn->next[LEFT]->hgt,bgn->next[RIGHT]->dat,bgn->next[RIGHT]->hgt);
            prtTree(bgn->next[LEFT]);
            prtTree(bgn->next[RIGHT]);
        } 
    }
    
    tree crtTree(int root){
        tree rtrn;
        rtrn.root = (ndptr) malloc(1 * sizeof(node));
        *(rtrn.root) = crtNode(root);
        rtrn.root->color = BLCK;
        rtrn.root->fthr = &SENTINEL;
        
        return rtrn;
    }


    // retorna o endereço da node pesquisada, caso esse exista. Do contrário retorna o endereço do pseudo pai
    // esses cenários são inferidos no parâmetro  status
    // a pesquisa tem inicio em bgn  
    ndptr srchTree(ndptr bgn,int key, int *status){
        *status = 1;
        while(1){
            if(bgn->dat == key) return bgn; //valor encontrado!
            else if(has_way(bgn,bgn->dat < key)) bgn = bgn->next[bgn->dat < key];//busca nominal
            else{//valor não encontrado!
              *status = 0;
              return bgn;  
            }
        }
    }

    //aloca uma nova node com o valor de key e realiza a operação de bind entre ela e fthr
    void insNd(ndptr fthr,int key, tree * t){
        ndptr newNd = (ndptr)malloc(sizeof(node)); 
        *newNd = crtNode(key);
        bind(newNd,fthr);
        vldteRed(newNd,t);
    }


/*++++TR CLASS BASIC METHODS++++*/



/*++++RB TREE DELETION MONITOR++++*/

    void fmly(ndptr ft, ndptr * dat, int sub){
        ndptr fthr = ft;//buferiza pra não dar pau
        dat[FTHR] = fthr;
        dat[SIB] = fthr->next[sub];
        dat[NPW] = fthr->next[sub]->next[LEFT];
        dat[NPW_] = fthr->next[sub]->next[RIGHT];   
    }

    void blckFix(ndptr * fm, int way, tree * t){
        printf("blckFix(%d)\n",fm[FTHR]->next[!way]->dat);
        printf("fmly colors %c %c %c %c\n",gtClr(fm[0]),gtClr(fm[1]),gtClr(fm[2]),gtClr(fm[3]));
        printf("fmly        %d %d %d %d\n",fm[0]->dat,fm[1]->dat,fm[2]->dat,fm[3]->dat);
        
        if(fm[FTHR]->next[!way]->color == RED){
            fm[FTHR]->next[!way]->color = BLCK; 
            return;
        }   
        while(!is_sentinel(fm[FTHR]) && fm[FTHR]->next[!way]->color == BLCK){
            if(fm[SIB]->color == RED){
                fm[SIB]->color = BLCK;
                fm[FTHR]->color = RED;

                LL(fm[SIB],t);
                fmly(fm[FTHR],fm,way);
            }
            if(fm[NPW]->color == BLCK && fm[NPW_]->color == BLCK){
                fm[SIB]->color = RED;
                fmly(fm[FTHR]->fthr,fm,!gtDir(fm[FTHR],fm[FTHR]->fthr));
            }
            else{
                if(fm[NPW + way]->color == BLCK){
                    fm[NPW + !way]->color = BLCK;
                    fm[SIB]->color = RED;
                    
                    RR(fm[NPW + !way],t);
                    fmly(fm[FTHR],fm,way);
                }
                fm[SIB]->color = fm[FTHR]->color;
                fm[FTHR]->color = BLCK;
                fm[NPW + way]->color = BLCK;

                LL(fm[SIB],t);
                break;
            }
        }
        fm[FTHR]->next[!way]->color = BLCK;
        t->root->color = BLCK;
    
        /*
        printf("blckFix(%d)\n",fm[FTHR]->next[!way]->dat);
        printf("fmly colors %c %c %c %c\n",gtClr(fm[0]),gtClr(fm[1]),gtClr(fm[2]),gtClr(fm[3]));
        printf("fmly        %d %d %d %d\n",fm[0]->dat,fm[1]->dat,fm[2]->dat,fm[3]->dat);
        
        if(fm[FTHR]->next[!way]->color == RED){
            // printf("dead node's child(%d) is (%c), setting it to black (simpleFix)\n",fm[FTHR]->next[!way]->dat,gtClr(fm[FTHR]->next[!way]));
            fm[FTHR]->next[!way]->color = BLCK; 
        }
        else{
            if(Clr(fm[SIB]) == RED){
                fm[SIB]->color = BLCK;
                fm[FTHR]->color = RED;

                if(way == RIGHT) LL(fm[SIB],t);
                else RR(fm[SIB],t);

                fmly(fm[FTHR],fm,!way);
                if(Clr(fm[NPW]) == BLCK && Clr(fm[NPW_]) == BLCK) goto c2;
                else if(Clr(fm[NPW + way]) == BLCK) goto c3;
            }
            else if(Clr(fm[NPW]) == BLCK && Clr(fm[NPW_]) == BLCK){
        c2:  
                fm[SIB]->color = RED;
                
                way = !gtDir(fm[FTHR],fm[FTHR]->fthr);
                fmly(fm[FTHR],fm,way);
                if(!is_sentinel(fm[FTHR])) blckFix(fm,way,t);
            }
            else if(Clr(fm[NPW + way]) == BLCK){
        c3:        
                // printf("everyone is black\n");
                fm[SIB]->color = RED;
                fm[NPW + !way]->color = BLCK;

                if(way == RIGHT) RR(fm[NPW + !way],t);
                else LL(fm[NPW + !way],t);
                goto c4;
            }
            else if(Clr(fm[SIB]) == BLCK && Clr(fm[NPW + way]) == RED){
                // printf("SIB: %d is red",fm[SIB]->dat);
        c4:
                fm[SIB]->color = fm[FTHR]->color;
                fm[FTHR]->color = BLCK;
                fm[NPW + way]->color = BLCK;

                if(way == RIGHT) LL(fm[SIB],t);
                else RR(fm[SIB],t);

            }
        }*/
    }

    void delNd(ndptr tgt, tree * vldte){
        
        ndptr root = vldte->root, fm[4] = {NULL,NULL,NULL,NULL};
        int way,f_way;

        if(is_leaf(tgt)){//                 NENHUM FILHO:
            printf("delNd(%d) is leaf\n",tgt->dat);
            if(root != tgt){
                if(Clr(tgt) == BLCK){//fudeu
                    printf("tring to delete a black leaf fuck...\n\n");
                    
                    f_way = !gtDir(tgt,tgt->fthr);
                    fmly(tgt->fthr,fm,f_way);
                }
                unbind(tgt);
                free(tgt);


            }
            else{
                tgt = &SENTINEL;
            }
        }
        else if(has_both_ways(tgt)){//      DOIS FILHOS:
            printf("delNd(%d) has both ways...",tgt->dat);            
            ndptr srch = tgt->next[RIGHT];
            
            while(1){
                if(has_way(srch,LEFT)) srch = srch->next[LEFT];
                else break;
            }
            
            if(Clr(srch) == BLCK){
                printf("fuck the sucessor(%d) is %c\n",srch->dat,gtClr(srch));
                f_way = !gtDir(srch,srch->fthr); 
                fmly(srch->fthr,fm,f_way);
            }

            if(has_way(srch,RIGHT)) bind(srch->next[RIGHT],srch->fthr);
            else unbind(srch);
            tgt->dat = srch->dat; 
            free(srch);
            

        }
        else{//                             UM FILHO:
            printf("delNd(%d) has one way...",tgt->dat);
            ndptr fthr = tgt->fthr;
            int way = has_way(tgt,RIGHT);
            
            if(Clr(tgt) == BLCK){
                f_way = !gtDir(tgt,tgt->fthr);
                fmly(tgt->fthr,fm,f_way);
            }

            printf("fuck the tgt(%d) is %c, LEFT %d RIGHT %d, way: %s\n",tgt->dat,gtClr(tgt),tgt->next[LEFT]->dat,tgt->next[RIGHT]->dat,TEMP[way]);
            printf("tgt->next[way]: %d\n",tgt->next[way]->dat);
            bind(tgt->next[way],tgt->fthr);
            free(tgt);
        
        }

        if(fm[0] != NULL) blckFix(fm,f_way,vldte);
        SENTINEL.color = BLCK;
        // printf("ok\n\n");
    }

/*++++RB TREE DELETION MONITOR++++*/


/*++++GABIARRAS++++*/

int redHgt(ndptr bgn){
    if(has_both_ways(bgn)) return bgst(redHgt(bgn->next[LEFT]),redHgt(bgn->next[RIGHT])) + (bgn->color == RED);
    else if(has_way(bgn,LEFT)) return redHgt(bgn->next[LEFT]) + (bgn->color == RED); 
    else if(has_way(bgn,RIGHT)) return redHgt(bgn->next[RIGHT]) + (bgn->color == RED); 
    else return (bgn->color == RED);
}

int Hgt(ndptr bgn){
    if(is_sentinel(bgn)) return 0;

    if(has_both_ways(bgn)) return bgst(Hgt(bgn->next[LEFT]),Hgt(bgn->next[RIGHT])) + 1;         
    else if(has_way(bgn,LEFT)) return Hgt(bgn->next[LEFT]) + 1; 
    else if(has_way(bgn,RIGHT)) return Hgt(bgn->next[RIGHT]) + 1; 
    else return 1;   
}
/*++++GABIARRAS++++*/


/*++++MAIN LOGIC++++*/
int main(){
    int hold;
    scanf("%d",&hold);

    tree t = crtTree(hold);
    int status;
    ndptr srch;

    while(1){
        scanf("%d",&hold);
        if(hold == -1) break;
        srch = srchTree(t.root,hold,&status);
        if(status == 0) insNd(srch,hold,&t);
    }
        int L = Hgt(t.root->next[LEFT]);
        int R = Hgt(t.root->next[RIGHT]);
        printf("%d, %d, %d\n",bgst(L,R),L,R);
    while(1){
        scanf("%d",&hold);
        if(hold == -1) break;
        srch = srchTree(t.root,hold,&status);
        if(status == 1){
            L = Hgt(srch->next[LEFT]);
            R = Hgt(srch->next[RIGHT]);
            printf("%d, %d, %d\n",bgst(L,R),L,R);
            delNd(srch,&t);
            //prtTree(t.root);
        }
        else insNd(srch,hold,&t);
    }
    
    scanf("%d",&hold);
    srch = srchTree(t.root,hold,&status);
    if(status == 1)printf("%d",redHgt(srch));
    else printf("Valor nao encontrado");

    // prtTree(t.root);
    return 0;
}


