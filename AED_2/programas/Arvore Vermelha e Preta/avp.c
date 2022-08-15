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
    node SENTINEL =  {-100, {&SENTINEL,&SENTINEL},&SENTINEL,-100,BLCK};//node sentinela, opera como valor nulo
    //ao verificar a arvore recusivamente, caso encontremos SENTINEL, isso indicará o final da árvore 
    //SENTINEL aponta para si mesmo para avitar erros de segmentação ao tentar acessa-lo
    //SENTINEL é preto sempre (nulos devem ser sempre pretos em arvores AVP)
/*++++ND CLASS ATRIBUTES++++*/                     



/*++++ND CLASS METHODS++++*/
    node crtNode(int key){
        node rtrn;
        rtrn.dat = key;

        rtrn.next[0] = &SENTINEL;//nodes são criadas sempre com SENTINEL como filho, dessa forma, podemos identificar folhas mais facilmente
        rtrn.next[1] = &SENTINEL;
        
        rtrn.hgt = 0;//nodes novas sempre são folhas, ou seja, a altura é 0
        rtrn.color = RED;//nodes novas sempre são vermelhas

        return rtrn;//(inserções devem ser sempre vermelhas em arvores AVP)
    }

    void bind(ndptr son, ndptr fthr){//esse método é extremamente importante, ele conecta uma node filha e pai, seguindo as propriedades de arvores binárias
        son->fthr = fthr;            //ele é importante por que eu não tenho que ficar me preocupando com a administração dos ponteiros, o que facilita remoções, inserções e rotações
        fthr->next[fthr->dat < son->dat] = son;
    }

    void unbind(ndptr son){//ubind realiza o inverso de bind(), ele disconecta um pai e um filho, também é muito útil na administração de ponteiros
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

/*++++TR CLASS UTILITIES++++*/



/*++++TR CLASS BALANCE METHODS++++*/
    void defHgt(ndptr bgn){//esse método de altura é eficiente para calcular as alturas conforme a arvore é modificada
        if(is_leaf(bgn)){  //mas não usei ele na hora de imprimir o resultado porque ele calcula as alturas corretamente
            bgn->hgt = 0;  //e o professor não gosta de respostas corretas, então não pude usar esse método
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



/*++++TR CLASS ROTATION METHODS++++*/ //minhas rotações tomam como parâmetro sempre o elemento mediano, eu chamo ele de pivô, um jeito fácil de identificar o pivô e ver quem que fica mais alto no final da rotação
    void RR(ndptr mid, tree * vldte){
        ndptr bgr = mid->fthr;
        unbind(mid);
        bind(mid,bgr->fthr);
        
        if(has_way(mid,RIGHT)) bind(mid->next[RIGHT],bgr);//passagem de guarda caso o elemento pivô (o valor que sobe na árvore depois da rotação) tenha filhos

        if(vldte->root == bgr){//validação da raiz, pode ser que uma rotação coloque um elemento vermelho na raiz, ela tem que ser validada para que esse elemento seja imediatamente configurado como preto
            vldte->root = mid; //além disso, a árvore tem que ser informada de que agora tem uma nova raiz  
            vldte->root->color = BLCK;    
        }

        bind(bgr,mid);
        defHgt(bgr);//recalcula a altura a partir do elemento modificado (Não esquece que eu não uso esse cálculo, o porque ta explicado na definição de defHgt())
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
        defHgt(smlr);
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
    
    void clrSet(ndptr bgn, int clr){//realiza a correção do cores em um color flip (clr = RED), ou após uma rotação (clr = BLCK)
        if(has_fthr(bgn))bgn->color = clr;                  
        if(has_way(bgn,LEFT)) bgn->next[LEFT]->color = !clr;
        if(has_way(bgn,RIGHT)) bgn->next[RIGHT]->color = !clr;
        //has_way, protege a cor de SENTINEL (NULL) que deve ser sempre preta 
        //has_fthr, protege a cor da raiz
    }

    void vldteRed(ndptr son, tree * t){//corrige nodes vermelhas adjacentes
        if(has_fthr(son)){
            if(son-> color == RED && son->fthr->color == RED){//violação ! 
                
                ndptr grdprt = son->fthr->fthr;//descobre quem é o avô
                int aunt_dir = son->fthr->dat < grdprt->dat;//descobre a posição relativa entre o avô e o tio/tia
                //essa linha de cima é um pouco estranha man, o que ela faz é que se o pai for menor do que o avô (esquerda) ela retorna 1
                //nesse programa 1 significa direita. Então assim, se o pai está na esquerda do avô, é porque o tio/tia ta na direita do avô
                //isso também funciona no sentido contrário, quando o pai ta na direita do avô

                if(grdprt->next[aunt_dir]->color == RED) clrSet(grdprt,RED);//color-flip quando o tio/tia é vermelho 
                
                else{//rotação quando o tio/tia é preto  
                    int son_dir = son->fthr->dat < son->dat;//posição relativa entre filho e pai
                    int fthr_dir = !aunt_dir;//posição relativa entre pai e avô

                    if(fthr_dir == LEFT && son_dir == LEFT){//mano aqui eu escolho as rotações, fica esperto porque o pivô muda conforme a rotação
                        RR(son->fthr,t);//rotaciona e corrige as cores a partir do pivô
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
            vldteRed(son->fthr,t);//corrige até a raiz, idependete se foi encontrada uma violação ou não, se vc n fizer isso pode ser que a sua árvore fique errada
        }
    }

    void vldteBlck(ndptr bgn){//validação de nodes pretas, só ocorre durante a remoção de nodes, então eu não implementei ainda
        
    }

/*++++TR CLASS RED BLACK METHODS++++*/



/*++++TR CLASS BASIC METHODS++++*/ 
    void prtTree(ndptr bgn){//printa a arvore a partir da raiz, eu uso isso no debug
        if(!is_sentinel(bgn)){
            printf("\n  fthr: %d(%d)(%d)(%c)\n\n  left: %d(%d)   right: %d(%d)\n===>\n",bgn->dat,bgn->fthr->dat,bgn->hgt,TEMPC[bgn->color][0],bgn->next[LEFT]->dat,bgn->next[LEFT]->hgt,bgn->next[RIGHT]->dat,bgn->next[RIGHT]->hgt);
            prtTree(bgn->next[LEFT]);
            prtTree(bgn->next[RIGHT]);
        } 
    }
    
    tree crtTree(int root){//cria uma árvore
        tree rtrn;
        rtrn.root = (ndptr) malloc(1 * sizeof(node));
        *(rtrn.root) = crtNode(root);
        rtrn.root->color = BLCK;//RAIZ É PRETA!!
        rtrn.root->fthr = &SENTINEL;//o pai da raiz é sentinel, isso é importante porque é assim que eu identifico que uma node qualquer é a raiz
        
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
        vldteRed(newNd,t);//nodes são validadas após a inserção
        defHgt(newNd);
    }

    void delNd(ndptr tgt, tree * vldte){//não se preucupa com essa função mano, a gente não tem que deletar nodes por enquanto
        ndptr root = vldte->root;
        
        if(is_leaf(tgt)){//                 NENHUM FILHO:
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
            ndptr srch = tgt->next[RIGHT];
            
            while(1){
                if(has_way(srch,LEFT)) srch = srch->next[LEFT];
                else break;
            }            
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

            bind(tgt->next[way],tgt->fthr);
            defHgt(fthr);//recalcula a altura a partir do pai da node retirada
            free(tgt);
        }
    }
/*++++TR CLASS BASIC METHODS++++*/



/*++++GABIARRAS++++*/

int blckHgt(ndptr bgn){//verifica 1 caminho genérico de bgn até a raiz, durante essa viagem, é contamos o número de nodes pretas
    if(has_both_ways(bgn)) return blckHgt(bgn->next[LEFT]) + (bgn->color == BLCK);
    else if(has_way(bgn,LEFT)) return blckHgt(bgn->next[LEFT]) + (bgn->color == BLCK); 
    else if(has_way(bgn,RIGHT)) return blckHgt(bgn->next[RIGHT]) + (bgn->color == BLCK); 
    else return (bgn->color == BLCK);
}

int Hgt(ndptr bgn){//calculo errado da altura
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
        if(hold == 100) break;
        srch = srchTree(t.root,hold,&status);
        if(status == 0) insNd(srch,hold,&t);
    }
        int L = Hgt(t.root->next[LEFT]);
        int R = Hgt(t.root->next[RIGHT]);
        // printf("%d, %d, %d\n",bgst(L,R),L,R);
    while(1){
        scanf("%d",&hold);
        if(hold == 100) break;
        srch = srchTree(t.root,hold,&status);
        if(status == 0) delNd(srch,&t);
        else{
            L = Hgt(srch->next[LEFT]);
            R = Hgt(srch->next[RIGHT]);
            // printf("%d, %d, %d\n",bgst(L,R),L,R);
        }
    }
    
    scanf("%d",&hold);
    srch = srchTree(t.root,hold,&status);
    if(status == 1)printf("%d",blckHgt(srch));
    else printf("Valor nao encontrado");
    return 0;
}


