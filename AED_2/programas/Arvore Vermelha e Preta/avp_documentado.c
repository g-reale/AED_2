#include <stdio.h>
#include <stdlib.h>


/*++++TEMP++++*/
    char TEMP[2][5] = {"LEFT","RIGHT"};
    #define LEFT 0
    #define RIGHT 1
    
    char TEMPC[2][4] = {"RED","BLCK"};
    #define RED 0
    #define BLCK 1
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
        son->fthr = fthr;            
        fthr->next[fthr->dat < son->dat] = son;
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

    char gtClr(ndptr bgn){return TEMPC[bgn->color][0];}


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
    /*
    REGRAS ELEMENTARES:
        1- Toda node é vermelha ou preta (OK) 
        2 - A raiz sempre é preta (+-)
        3 - Inserções são vermelhas(OK-> ctrNds())
        4 - Qualquer caminho de raiz até folhas, tem o mesmo número de BLCK nodes
        5 - Nenhum caminho pode ter duas nodes RED consecutivas
        6 - NULL nodes são pretas(OK-> SENTINEL)   

    REGRAS PARA REBALANCEAMENTO:
        1 - Caso a node desbalanceada tenha um Tio/Tia preto:
            ->Executar uma rotação
            ->Corrigir cores, arranjo pós-rotação:
                            B
                            |
                          +-+-+
                          |   |
                          R   R
        2 - Caso a node desbalanceada tenha um Tio/Tia vermelho:
            -> Inversão de cores, arranjo pós correção:
                            R   <- avô
                            |
                          +-+-+
                          |   |
                          B   B <-pai e tio/tia
    */
    
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
/*void vldteDelPt2(ndptr x){
        if(x->color == RED){
            x->color == BLCK;
        }
    }

    c

    void delNd(ndptr tgt, tree * vldte){
        ndptr root = vldte->root;
        ndptr rplc;//vldteDel
        ndptr x;//vldteDel

        if(is_leaf(tgt)){//                 NENHUM FILHO:
            if(root != tgt){
                ndptr fthr = tgt->fthr;
                rplc = x = &SENTINEL;

                unbind(tgt);
                defHgt(fthr);//recalcula a altura a partir do pai da node retirada
                free(tgt);
            }
            else{
                tgt = &SENTINEL;
            }
        }
        else if(has_both_ways(tgt)){//      DOIS FILHOS:
            ndptr srch = tgt->next[RIGHT];
            
            while(1){
                if(has_way(srch,LEFT)) srch = srch->next[LEFT];
                else break;
            }

            //            
            rplc = srch;
            x = rplc->next[RIGHT];
            //

            ndptr fthr = srch->fthr; 
            
            if(has_way(srch,RIGHT)) bind(srch->next[RIGHT],srch->fthr);
            else unbind(srch);
            tgt->dat = srch->dat; 
            defHgt(fthr);//recalcula a altura a partir do pai da node retirada
            free(srch);
        }
        else{//                             UM FILHO:
            ndptr fthr = tgt->fthr;
            int way = has_way(tgt,RIGHT);
            
            if(tgt == root){//validação da raiz
                vldte->root = tgt->next[way];  
                vldte->root->color = RED;
            }
            
            //
            x = rplc = tgt->next[way];
            //

            bind(tgt->next[way],tgt->fthr);
            defHgt(fthr);//recalcula a altura a partir do pai da node retirada
            free(tgt);
        }
    

    void DL2(ndptr * fm, int way, tree *t){
        //printf("DL2()\n");
        if(way == RIGHT){
            //printf("sibling(%d) is on the RIGHT, LL rotation...\n",fm[1]->dat);
            LL(fm[1],t);
        }
        else{
            //printf("sibling(%d) is on the LEFT, RR rotation...\n",fm[1]->dat);
            RR(fm[1],t);
        }
        //printf("fm[0] = %d (%c) and fm[1] = %d (%c)... reversing colors... ",fm[0]->dat,gtClr(fm[0]),fm[1]->dat,gtClr(fm[1]));
        fm[0]->color = RED;
        fm[1]->color = BLCK;
        //printf("fm[0] = %c and fm[1] = %c, going back\n\n",gtClr(fm[0]),gtClr(fm[1]));
    }

    ndptr DL3(ndptr * fm){
        //printf("DL3()\n");
        //printf("fm[1] = %d (%c) ... reversing colors ...",fm[1]->dat,gtClr(fm[1]));
        fm[1]->color = RED;
        //printf("fm[1] = %c, returning fm[0] = %d\n\n",gtClr(fm[1]),fm[0]->dat);
        return fm[0];
    }

    void DL4(ndptr * fm){
        //printf("DL4()\n");
        //printf("fm[0] = %d (%c) and fm[1] = %d (%c)... reversing colors... ",fm[0]->dat,gtClr(fm[0]),fm[1]->dat,gtClr(fm[1]));
        fm[0]->color = BLCK;
        fm[1]->color = RED;   
        //printf("fm[0] = %c and fm[1] = %c, DONE!\n\n",gtClr(fm[0]),gtClr(fm[1]));
    }

    void DL5(ndptr * fm, int way, tree *t){
        //printf("DL5()\n");
        if(way == RIGHT){
            //printf("x(%d)(%d) is on the LEFT, RR rotation...\n",fm[2 + !way]->dat,fm[2 + !way]->fthr->dat);
            RR(fm[2 + !way],t);
        }
        else{
            //printf("x(%d)(%d) is on the RIGHT, LL rotation...\n",fm[2 + !way]->dat,fm[2 + !way]->fthr->dat);
            LL(fm[2 + !way],t);
        }
        //printf("fm[1] = %d (%c) and fm[2 + !way] = %d (%c)... reversing colors... ",fm[1]->dat,gtClr(fm[1]),fm[2 + !way]->dat,gtClr(fm[2 + !way]));
        fm[1]->color = RED;
        fm[2 + !way]->color = BLCK;
        //printf("fm[1] = %c and fm[2 + !way] = %c, going back\n\n",gtClr(fm[0]),gtClr(fm[2 + !way]));
    }

    void DL6(ndptr * fm, int way, tree *t){
        //printf("DL6()\n");
        if(way == RIGHT){
            //printf("y(%d)(%d) is on the RIGHT, LL rotation on fm[1]...\n",fm[2 + way]->dat,fm[2 + way]->fthr->dat);
            LL(fm[1],t);
        }
        else{
            //printf("x(%d)(%d) is on the LEFT, RR rotation on fm[1]... \n",fm[2 + way]->dat,fm[2 + way]->fthr->dat);
            RR(fm[1],t);
        }
        //printf("fm[1] = %d (%c) and fm[2 + way] = %d (%c)... chaging colors... ",fm[1]->dat,gtClr(fm[1]),fm[2 + way]->dat,gtClr(fm[2 + way]));
        fm[1]->color = fm[0]->color;
        fm[2 + way]->color = BLCK;
        //printf("fm[1] = %c and fm[2 + way] = %c, DONE!\n\n",gtClr(fm[0]),gtClr(fm[2 + way]));

    }

    void DL7(ndptr * fm, int way, tree *t){
        //printf("DL7()\n");
        if(way == RIGHT) RL(fm[2 + !way],t);
        else LR(fm[2 + !way],t);
        clrSet(fm[2 + !way],RED);
        //printf("fmly colors %c %c %c %c\n",gtClr(fm[0]),gtClr(fm[1]),gtClr(fm[2]),gtClr(fm[3]));
        //printf("fmly        %d %d %d %d\n",fm[0]->dat,fm[1]->dat,fm[2]->dat,fm[3]->dat);
        //printf("fthr: %d LEFT: %d RIGHT: %d\n",fm[2 + !way]->dat,fm[2 + !way]->next[LEFT]->dat,fm[2 + !way]->next[RIGHT]->dat);
    }   
            if(fm[0]->color == BLCK && fm[1]->color == RED && fm[2]->color == BLCK && fm[3]->color == BLCK)                   DL2(fm,way,t),complexFix(ddBlck,t);
            else if(fm[0]->color == BLCK && fm[1]->color == BLCK && fm[2]->color == BLCK && fm[3]->color == BLCK)                                     complexFix(DL3(fm),t);
            else if(fm[0]->color == RED && fm[1]->color == BLCK && fm[2]->color == BLCK && fm[3]->color == BLCK)              DL4(fm);
            else if(fm[0]->color == BLCK && fm[1]->color == BLCK && fm[2 + !way]->color == RED && fm[2 + way]->color == BLCK) DL5(fm,way,t),complexFix(ddBlck,t);      
            else if(fm[1]->color == BLCK && fm[2 + way]->color == RED)                                                               DL6(fm,way,t);
            else DL7(fm,way,t);
    }*/

    int fmly(ndptr son,ndptr * dat){
        int way = son->dat < son->fthr->dat;
        dat[0] = son->fthr;
        dat[1] = son->fthr->next[way];
        dat[2] = son->fthr->next[way]->next[LEFT];
        dat[3] = son->fthr->next[way]->next[RIGHT];  
        return way;     
    }

    void DL1(ndptr root){
        //printf("DL1(%d) ... DONE!\n\n",root->dat);
        root->color = BLCK;
    }

    void restructure(){

    }

    void complexFix(ndptr ddBlck, tree * t){
        //printf("complexFix(%d)\n",ddBlck->dat);
        if(!has_fthr(ddBlck)) DL1(ddBlck);
        else{
            ndptr fm[4];
            int way = fmly(ddBlck,fm);
            //printf("fmly colors %c %c %c %c\n",gtClr(fm[0]),gtClr(fm[1]),gtClr(fm[2]),gtClr(fm[3]));
            //printf("fmly        %d %d %d %d\n",fm[0]->dat,fm[1]->dat,fm[2]->dat,fm[3]->dat);
            
        }
    }

    void simpleFix(ndptr die,tree * t){
        //printf("simpleFix(%d) LEFT: %d(%c) RIGHT: %d(%c)\n",die->dat,die->next[LEFT]->dat,gtClr(die->next[LEFT]),die->next[RIGHT]->dat,gtClr(die->next[RIGHT]));
        if(!is_leaf(die)){
            int way = has_way(die,RIGHT);
            if(die->next[way]->color == RED){
                //printf("%d has a %c son (%d)...",die->dat,TEMPC[die->next[way]->color][0],die->next[way]->dat);
                die->next[way]->color = BLCK;
                //printf(" %d is now %c\n",die->next[way]->dat,TEMPC[die->next[way]->color][0]);
                return;
            }
        }
        //printf("could not find a suitable subistitute fuck...\n\n");
        complexFix(die,t);      
    }

    void delNd(ndptr tgt, tree * vldte){
        ndptr root = vldte->root;
        if(is_leaf(tgt)){//                 NENHUM FILHO:
            //printf("delNd(%d) is leaf\n",tgt->dat);
            if(root != tgt){
                ndptr fthr = tgt->fthr;
                int way = fthr->dat < tgt->dat;

                if(tgt->color == BLCK){//fudeu
                    //printf("tring to delete a black leaf fuck...\n\n");
                    complexFix(tgt,vldte);
                }

                unbind(tgt);
                free(tgt);
            }
            else{
                tgt = &SENTINEL;
            }
        }
        else if(has_both_ways(tgt)){//      DOIS FILHOS:
            //printf("delNd(%d) has both ways...",tgt->dat);            
            ndptr srch = tgt->next[RIGHT];
            
            while(1){
                if(has_way(srch,LEFT)) srch = srch->next[LEFT];
                else break;
            }
            ndptr fthr = srch->fthr;

            if(srch->color == BLCK){
                //printf("fuck the sucessor(%d) is %c\n",srch->dat,gtClr(srch));
                simpleFix(srch,vldte);
            }
            if(has_way(srch,RIGHT)) bind(srch->next[RIGHT],srch->fthr);
            else unbind(srch);
            tgt->dat = srch->dat; 
            free(srch);
        }
        else{//                             UM FILHO:
            //printf("delNd(%d) has one way...n",tgt->dat);
            ndptr fthr = tgt->fthr;
            int way = has_way(tgt,RIGHT);
            
            if(tgt->color == BLCK){
                //printf("fuck the tgt(%d) is %c\n",tgt->dat,gtClr(tgt));
                simpleFix(tgt,vldte);
            }

            bind(tgt->next[way],tgt->fthr);
            free(tgt);
        }
        SENTINEL.color = BLCK;
        //printf("ok\n\n");
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
        //else insNd(srch,hold,&t);
    }
    
    scanf("%d",&hold);
    srch = srchTree(t.root,hold,&status);
    if(status == 1)printf("%d",redHgt(srch));
    else printf("Valor nao encontrado");

    //prtTree(t.root);
    return 0;
}


