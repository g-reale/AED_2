#include <stdio.h>
#include <stdlib.h>

/*		 TEMP		*/
char TEMP[4][5] = {"LEFT","RIGHT","FREE","BOTH"};
char TEMP_AVL[5][2] = {"RR","LL","RL","LR","OK"};
/*		 TEMP		*/


/*		NODE DEFINITIONS		*/
typedef enum {
	LEFT, RIGHT, FREE, BOTH, 
} path;

typedef struct nde{
	int dat;
	struct nde * next[3];
	struct nde * fthr;
	path way;
	int hght;
}node;

typedef node * ndptr;

node crtNode(int key){
	node rtrn;
	rtrn.dat = key;
	rtrn.way = FREE;
	rtrn.hght = 0; 
	return rtrn;
}

/*		NODE DEFINITIONS		*/


/*		TREE DEFINITIONS		*/
typedef struct tre{
	ndptr root;
	ndptr sentinel;
	int size;
}tree;

tree crtTree(int root){
	tree rtrn;
	
	rtrn.root = (ndptr) malloc(1 * sizeof(node));
	*(rtrn.root) = crtNode(root);
	
	rtrn.sentinel = (ndptr) malloc(1 * sizeof(node));;
	*(rtrn.sentinel) = crtNode(-100);
	
	rtrn.root->fthr = rtrn.sentinel;
	
	rtrn.size = 1;
	return rtrn;
}


ndptr srchTree(tree * t,int key, path * turn){
	ndptr srch = t->root; 

	while(1){
		*turn = BOTH;
		if(srch->dat == key) break;//se o valor for encontrado, retorna aonde ele está
		
		if(srch->dat < key) *turn = RIGHT;
		else *turn = LEFT;

		if(srch->way == *turn || srch->way == BOTH) srch = srch->next[*turn]; 
		else break;//se o valor não foi encontrado, retorna aonde ele deveria estar
	}
	return srch;
}

void printfTree(ndptr begin){
	if(begin->way == BOTH){
		printf("---------------------------------------\n");
		printf("\tfather: %d (%d)(%d)\nLEFT: %d (%d)(%d)\tRIGHT: %d (%d)(%d)\n",begin->dat,begin->hght,begin->fthr->dat,begin->next[LEFT]->dat,begin->next[LEFT]->hght,begin->next[LEFT]->fthr->dat,begin->next[RIGHT]->dat,begin->next[RIGHT]->hght,begin->next[RIGHT]->fthr->dat);
		printf("---------------------------------------\n\n");
		printfTree(begin->next[LEFT]);
		printfTree(begin->next[RIGHT]);
	}
	else if(begin->way < FREE){
		printf("---------------------------------------\n");
		printf("\tfather: %d (%d)(%d)\n%s: %d (%d)(%d)\n",begin->dat,begin->hght,begin->fthr->dat,TEMP[begin->way],begin->next[begin->way]->dat,begin->next[begin->way]->hght,begin->next[begin->way]->fthr->dat);
		printf("---------------------------------------\n\n");
		printfTree(begin->next[begin->way]);
	}
	else{
		printf("---------------------------------------\n");
		printf("\tfather: %d (%d)(%d)\n",begin->dat,begin->hght,begin->fthr->dat);
		printf("---------------------------------------\n\n");
	}
}
/*		TREE DEFINITIONS		*/


/*		AVL DEFINITIONS		*/
typedef enum {
	RR, LL, RL, LR, OK 
} rtt;

int getBal(ndptr node){
	if (node->way == FREE) return 0;
	if(node->way == LEFT) return node->next[LEFT]->hght + 1;
	if(node->way == RIGHT) return -node->next[RIGHT]->hght - 1;
	return (node->next[LEFT]->hght - node->next[RIGHT]->hght);
}
void correcHght(ndptr node,int hght){
	node->hght = hght;
	hght++;
	if(node->fthr->dat != -100 && hght > node->fthr->hght ) correcHght(node->fthr,hght); 
}
void rfrshHght(ndptr node){
	node->hght = -1;
	if(node->way == RIGHT) rfrshHght(node->next[RIGHT]);
	if(node->way == LEFT) rfrshHght(node->next[LEFT]);
	if(node->way == BOTH){
		rfrshHght(node->next[RIGHT]);
		rfrshHght(node->next[LEFT]);
	}
	if(node->way == FREE) correcHght(node,0);
}

rtt detec(ndptr node, int mode ,ndptr *A, ndptr *B){
	ndptr last = node;
	int hold,hold_;
	if(mode){
		while(1){
			if(node->dat == -100) return OK;	
			hold = getBal(node);
			if(hold == 2 || hold == -2) break;
			last = node;
			node = node->fthr;
		}
	hold_ = getBal(last);
	*A = node;
	*B = last;

	if(hold == 2 && hold_ == 1) return LL;
	if(hold == 2 && hold_ == -1) return	RL;
	if(hold == -2 && hold_ == 1) return LR;
	if(hold == -2 && hold_ == -1) return RR;
	}
	else{
		while(1){
			if(node->dat == -100) return OK;	
			hold = getBal(node);
			
			if(hold == 2 || hold == -2){
				if(last == node->next[LEFT]) last = node->next[RIGHT]; 
				else last = node->next[LEFT];
				break;	
			}
			last = node;
			node = node->fthr;
		}
	hold_ = getBal(last);
	*A = node;
	*B = last;

	if(hold == -2 && hold_ == 0 || hold == -2 && hold_ == -1) return RR;	
	if(hold == 2 && hold_ == 0  || hold == 2 && hold_ == 1) return LL;
	if(hold == -2 && hold_ == 1) return LR;	
	if(hold == 2 && hold_ == -1) return RL;
	
	}
}

ndptr rotate(ndptr A, ndptr B,rtt rtte, tree *t){
	
	switch(rtte){
		case LL:
			{

			//Realocates the fthr of A to B 
			path temp = A->fthr->dat < A->dat;
			A->fthr->next[temp] = B;
			B->fthr = A->fthr;
			//

			//turns B the fthr of A
			A->fthr = B;
			//

			//Realocate the RIGHT son of B, as the left son of A, if needed, else, A doesn´t has any sons 
			if(B->way == BOTH){
				A->next[LEFT] = B->next[RIGHT];
				B->next[RIGHT]->fthr = A;
				//A->way doesn`t need to be ajusted in this instance
			}
			else{
				if (A->way == BOTH) A->way = RIGHT;
				if (A->way == LEFT) A->way = FREE;
				if (B->way == FREE) B->way = RIGHT;
				if (B->way == LEFT) B->way = BOTH;
				
			} 
			//

			B->next[RIGHT] = A;
			if(B->fthr->dat == -100) t->root = B;
			return B;
			}
		break;

		case RR:
			{
			path temp = A->fthr->dat < A->dat;
			A->fthr->next[temp] = B;

			B->fthr = A->fthr;
			A->fthr = B;
			
			if(B->way == BOTH){
				A->next[RIGHT] = B->next[LEFT];
				B->next[LEFT]->fthr = A;
				//A->way doesn`t need to be ajusted in this instance
			}
			else{
				if (A->way == BOTH) A->way = LEFT;
				if (A->way == RIGHT) A->way = FREE;
				if (B->way == FREE) B->way = LEFT;
				if (B->way == RIGHT) B->way = BOTH;
			}
			B->next[LEFT] = A;
			if(B->fthr->dat == -100) t->root = B;
			return B;
			}
		break;

		case RL:
			{
			ndptr C = B->next[RIGHT];
	
			rotate(B,C,RR,t);
			rotate(A,C,LL,t);
			return C;
			}
		break;
		
		case LR:
			{
			ndptr C = B->next[LEFT];
			
			rotate(B,C,LL,t);
			rotate(A,C,RR,t);
			return C;
			}
		break;
	}
}

void correc(tree * t, ndptr begin, int mode){
	ndptr A;
	ndptr B;
	rtt rtte = 0;

	while(rtte != OK){
		rfrshHght(t->root);
		rtte = detec(begin,mode,&A,&B);
		if(rtte != OK){
			correc(t,rotate(A,B,rtte,t),mode);
		}
		else rtte = OK;
	}
}

int apndTree(tree * t,int key,ndptr * alert){
	path status;
	ndptr father = srchTree(t,key,&status);
	
	*alert = father;
	if(status == BOTH) return 0;//item já presente 

	t->size++;
	ndptr newNd = (ndptr)malloc(sizeof(node)); 
	*newNd = crtNode(key);
	newNd->fthr = father;

	//gambiarra
	if(t->size-1 == 1) father = t->root;
	//

	father->next[status] = newNd;

	if(father->way == FREE) father->way = status;
	else father->way = BOTH;

	*alert = newNd;
	correc(t,newNd,1);
	return	1;
}

void dltnode(tree *t,node * die){
	t->size--;
	switch(die->way){		
		case BOTH:
			{
				ndptr srch = die->next[RIGHT];
				while(1){
					if(srch->way == BOTH || srch->way == LEFT) srch = srch->next[LEFT];
					else break;	
				}
				path turn = srch->dat > srch->fthr->dat;
				die->dat = srch->dat;

				if(srch->way != FREE){
					srch->next[RIGHT]->fthr = srch->fthr;
					srch->fthr->next[turn] = srch->next[RIGHT]; 
				}
				else{
					if(srch->fthr->way < FREE) srch->fthr->way = FREE;
					else srch->fthr->way = 1 - turn;
				}
				correc(t,srch,0);
				free(srch);
			}
		break;
		
		case FREE:
			{
				if(die->fthr->way < FREE) die->fthr->way = FREE;
				else die->fthr->way = die->dat < die->fthr->dat;
				correc(t,die,0);
				free(die);
			}
		break;
	
		default:
			{
				path turn = die->dat > die->fthr->dat;
				die->fthr->next[turn] = die->next[die->way];
				die->next[die->way]->fthr = die->fthr;
				correc(t,die,0);
				free(die);
			}
		break;
	}
}
/*		AVL DEFINITIONS		*/

int main(){
	ndptr hold;
	int buff;
	scanf("%d",&buff);
	tree a = crtTree(buff);
	
	while(1){
		scanf("%d",&buff);
		if(-1 < buff) apndTree(&a,buff,&hold);
		else break;
	}

	{
	rfrshHght(a.root);
		int l = 0; 
		int	r = 0; 
		int	b = 0;

		if(a.root->way == LEFT || a.root->way == BOTH){
			l = a.root->next[LEFT]->hght + 1;	
		}
		if(a.root->way == RIGHT || a.root->way == BOTH){
			r = a.root->next[RIGHT]->hght + 1;	
		}
		
		b = a.root->hght;
		printf("%d, %d, %d\n",b,l,r);	
	}
	while(1){
		scanf("%d",&buff);
		if(-1 < buff){
			if(!apndTree(&a,buff,&hold)){
				dltnode(&a,hold);
			}
		}
		else break;
	}
	scanf("%d",&buff);
	path exists;
	hold = srchTree(&a,buff,&exists);
	if(hold->dat == buff){
		rfrshHght(a.root);
		int l = 0; 
		int	r = 0; 
		int	b = 0;

		if(a.root->way == LEFT || a.root->way == BOTH){
			l = a.root->next[LEFT]->hght + 1;	
		}
		if(a.root->way == RIGHT || a.root->way == BOTH){
			r = a.root->next[RIGHT]->hght + 1;	
		}
		
		b = a.root->hght;
		printf("%d, %d, %d\n",b,l,r);	
	}
	else printf("Valor nao encontrado");
}


