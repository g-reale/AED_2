#include <stdio.h>
#include <stdlib.h>


/*++++TEMP++++*/
    char TEMP[2][5] = {"LEFT","RIGHT"};
    #define LEFT 0
    #define RIGHT 1
    
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
    node SENTINEL =  {-100, {NULL,NULL},NULL,-100};
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
        printf("bind(son %d,fthr %d)...",son->dat,fthr->dat);
        son->fthr = fthr;
        fthr->next[fthr->dat < son->dat] = son;
        printf("confirmation:%d->next[%s] = %d && %d->fthr = %d\n",fthr->dat,TEMP[fthr->dat < son->dat],fthr->next[fthr->dat < son->dat]->dat,son->dat,son->fthr->dat);
    }

    void unbind(ndptr son){
        printf("unbind(%d)...",son->dat);
        ndptr fthr = son->fthr;//apagar dps

        son->fthr->next[son->fthr->dat < son->dat] = &SENTINEL;
        son->fthr = &SENTINEL;
    
        printf("confirmation: %d->next[%s] = %d &&  %d->fthr = %d\n",fthr->dat,TEMP[fthr->dat < son->dat],fthr->next[fthr->dat < son->dat]->dat,son->dat,son->fthr->dat);
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

/*++++TR CLASS UTILITIES++++*/



/*++++TR CLASS BALANCE METHODS++++*/
    void defHgt(ndptr bgn){
//        printf("\n\n===>\ndefHgt(%d)...",bgn->dat);
//        printf(" %d has_a_fthr = %d\n",bgn->dat,bgn->fthr->dat);

        if(is_leaf(bgn)){
//              printf("%d is a leaf, setting hgt to 0\n",bgn->dat);
            bgn->hgt = 0;
            if(has_fthr(bgn)) defHgt(bgn->fthr);
        }
        else{
            int bg = bgst(bgn->next[LEFT]->hgt,bgn->next[RIGHT]->hgt);

//                printf("verifing hgts: (%d , %d) (bg , %d)...",bgn->dat,bgn->hgt,bg);
            if(bgn->hgt != bg + 1){
//                    printf(" inconsitent, setting (%d , %d) to %d...calling defHgt(%d)\n",bgn->dat,bgn->hgt,bg + 1,bgn->dat);
                bgn->hgt = 1 + bg;
                if(has_fthr(bgn)) defHgt(bgn->fthr); 
            }
        }
        
        //toda node válida tem 2 filhos, esses podem ser outras nodes válidas, ou SENTINEL            
        //por isso acessar ambos os filhos não gera erros de segmentação.

        //dito isso, podemos observar se a altura do pai é definida pelo mais alto de seus filhos      
        //ou seja, para esse mais alto, temos que (filho->hgt) + 1 == pai->hgt
        //se isso não for verdade, é porque existe uma inconsistência na altura do pai  
        //caso isso ocorra, basta encontar a subárvore com maior altura reestruturar a definição a partir dela
        //na prática isso significa que , com base no filho mais alto, definir o pai como, pai->hgt = (filho->hgt) + 1
        //esse processo continua de forma recursiva até que nenhuma inconsistência seja encontrada, ou, até que bgn
        //atinja a raiz 
    
        //é importante citar que a primeira node que chama a recusrsão, não tem sua altura corrigida, o que deixa 
        //espaço para programar exeções de forma conviniente, enquanto defHgt instancia tais exeções, seguindo as
        //propriedades de uma árvore binária comum.

        //em casos de remoção de nodes, é interessante que a função saiba diferenciar entre uma folha e uma node comum. 
    }
/*++++TR CLASS BALANCE METHODS++++*/




/*++++TR CLASS BASIC METHODS++++*/ 
    tree crtTree(int root){
        printf("crtTree(%d)\n",root);
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
    void insNd(ndptr fthr,int key){
        printf("insNd(%d)\t",key);
        ndptr newNd = (ndptr)malloc(sizeof(node)); 
        *newNd = crtNode(key);
        bind(newNd,fthr);
        defHgt(newNd);
    }

    void prtTree(ndptr bgn){
        if(!is_sentinel(bgn)){
            printf("\n\t fthr: %d(%d)(%d)\n\t|\t|\n  left: %d(%d)   right: %d(%d)\n===>\n",bgn->dat,bgn->fthr->dat,bgn->hgt,bgn->next[LEFT]->dat,bgn->next[LEFT]->hgt,bgn->next[RIGHT]->dat,bgn->next[RIGHT]->hgt);
            prtTree(bgn->next[LEFT]);
            prtTree(bgn->next[RIGHT]);
        } 
    }
    
    //ADIMINISTRAÇÃO DAS ALTURAS
    void delNd(ndptr tgt, tree * vldte){
        printf("delNd(%d)...",tgt->dat);
        ndptr root = vldte->root;
        
        if(is_leaf(tgt)){//                 NENHUM FILHO:
                printf("is leaf\n");
            
            if(root != tgt){
                ndptr fthr = tgt->fthr;
                unbind(tgt);
                defHgt(fthr);//recalcula a altura a partir do pai da node retirada
                free(tgt);
            }
            else{
                tgt = &SENTINEL;
            }
        }
        else if(has_both_ways(tgt)){//      DOIS FILHOS:
            printf("has 2 sons...");
            ndptr srch = tgt->next[RIGHT];
            
            while(1){
                if(has_way(srch,LEFT)) srch = srch->next[LEFT];
                else break;
            }
            printf("linear sucessor: %d\n",srch->dat);
            
            ndptr fthr = srch->fthr; 
            if(has_way(srch,RIGHT)) bind(srch->next[RIGHT],srch->fthr);
            else unbind(srch);
            tgt->dat = srch->dat; 
            defHgt(fthr);//recalcula a altura a partir do pai da node retirada
            free(srch);
        }
        else{//                             UM FILHO:
            printf("has 1 son\n");
            ndptr fthr = tgt->fthr;
            int way = has_way(tgt,RIGHT);
            if(tgt == root) vldte->root = tgt->next[way];  
            bind(tgt->next[way],tgt->fthr);
            defHgt(fthr);//recalcula a altura a partir do pai da node retirada
            free(tgt);
        }
    }
/*++++TR CLASS BASIC METHODS++++*/


//ADIMINISTRAÇÃO DAS ALTURAS
/*++++TR CLASS ROTATION METHODS++++*/
    void RR(ndptr mid, tree * vldte){
        ndptr bgr = mid->fthr;
        unbind(mid);
        bind(mid,bgr->fthr);
        
        if(has_way(mid,RIGHT)) bind(mid->next[RIGHT],bgr);//passagem de guarda
        if(vldte->root == bgr) vldte->root = mid; //adiministração da raiz 

        bind(bgr,mid);
    }

    void LL(ndptr mid, tree * vldte){
        ndptr smlr = mid->fthr;
        unbind(mid);
        bind(mid,smlr->fthr);
        
        if(has_way(mid,LEFT)) bind(mid->next[LEFT],smlr);//passagem de guarda
        if(vldte->root == smlr) vldte->root = mid; //adiministração da raiz 

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




int main(){
    int hold;
    scanf("%d",&hold);

    tree t = crtTree(hold);
    int status;
    ndptr srch;

    while(1){
        scanf("%d",&hold);
        if(hold < 0) break;
        srch = srchTree(t.root,hold,&status);
        if(status == 0) insNd(srch,hold);
    }
    prtTree(t.root); 

    printf("\n===> PT2 ===> PT2 ===> PT2 ===> PT2 ===>\n\n");
    while(1){
        scanf("%d",&hold);
        if(hold < 0) break;
        srch = srchTree(t.root,hold,&status);
        if(status == 0) insNd(srch,hold);
        else delNd(srch,&t);

        printf("\n");
        //prtTree(t.root); 
    }
    printf("\n===> PT3 ===> PT3 ===> PT3 ===> PT3 ===>\n");
    prtTree(t.root); 
    
    scanf("%d",&hold);
    srch = srchTree(t.root,hold,&status);
    if(status == 1)printf("%d %d %d",srch->hgt,srch->next[LEFT]->hgt,srch->next[RIGHT]->hgt);
    else printf("valor nao encontrado");
    return 0;
}


