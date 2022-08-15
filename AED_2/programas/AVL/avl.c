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


/*
int getHght(ndptr node, int hght){//very ineficient!! i should search for all leaves, and return back up, updating all the hghts on the way up
	if(node->way == BOTH){
		int rtrn = getHght(node->next[LEFT],hght + 1);
		int rtrn_ = getHght(node->next[RIGHT],hght + 1);
		return	rtrn < rtrn_ ? rtrn_ : rtrn; 
	}
	else if(node->way < FREE) getHght(node->next[node->way],hght + 1);	
	else{
		return	hght + 1;
	}
}


void rfrshHght(ndptr node){
	switch(node->way){
		case RIGHT:
			node->hght = -getHght(node->next[RIGHT],0);
			rfrshHght(node->next[RIGHT]);
		break;
		case LEFT:
			node->hght = getHght(node->next[LEFT],0);
			rfrshHght(node->next[LEFT]);
		break;
		case BOTH:
			node->hght = getHght(node->next[LEFT],0) - getHght(node->next[RIGHT],0);
			rfrshHght(node->next[RIGHT]);
			rfrshHght(node->next[LEFT]);
		break;
		//unecesarry FREE case, node is already created with 0 hght 
	}
}
*/
int getBal(ndptr node){
	if (node->way == FREE) return 0;
	if(node->way == LEFT) return node->next[LEFT]->hght + 1;
	if(node->way == RIGHT) return -node->next[RIGHT]->hght - 1;
	return (node->next[LEFT]->hght - node->next[RIGHT]->hght);
}
void correcHght(ndptr node,int hght){
	//printf("\ncorrecHght:----------\n");
	//printf("setting %d hght as %d hght\n", node->dat, hght);
	//printf("next node hght %d vs current hght %d bal = %d\n",node->fthr->hght,hght+1,getBal(node));
	node->hght = hght;
	hght++;
	if(node->fthr->dat != -100 && hght > node->fthr->hght ) correcHght(node->fthr,hght); 
}
void rfrshHght(ndptr node){
	//printf("current node: %d, node->way = %s\n",node->dat, TEMP[node->way]);
	node->hght = -1;
	if(node->way == RIGHT) rfrshHght(node->next[RIGHT]);
	if(node->way == LEFT) rfrshHght(node->next[LEFT]);
	if(node->way == BOTH){
		rfrshHght(node->next[RIGHT]);
		rfrshHght(node->next[LEFT]);
	}
	if(node->way == FREE) {
		//printf("\nrfrshHght:--------\n");
		//printf("found leaf: %d\n",node->dat);
		correcHght(node,0);
	}
}

rtt detec(ndptr node, int mode ,ndptr *A, ndptr *B){
	ndptr last = node;
	int hold,hold_;
	//printf("\nDETEC:----------\n");
	if(mode){
		while(1){
			if(node->dat == -100){
				//printf("OK!\n");
				return OK;	
			}  
			hold = getBal(node);
			//printf("%d (%d) ",node->dat,hold);
			if(hold == 2 || hold == -2) break;
			last = node;
			node = node->fthr;
		}
	//printf("\nsomething is wrong..\n%d ->hght = %d and  %d ->hght = %d\n",node->dat,hold,last->dat,getBal(last));
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
			if(node->dat == -100){
				//printf("OK!\n");
				return OK;	
			}  
			hold = getBal(node);
			//printf("%d (%d) ",node->dat,hold);
			if(hold == 2 || hold == -2){
				if(last == node->next[LEFT]) last = node->next[RIGHT]; 
				else last = node->next[LEFT];
				break;	
			}
			last = node;
			node = node->fthr;
		}
	printf("\nsomething is wrong..\n%d ->hght = %d and  %d ->hght = %d\n",node->dat,hold,last->dat,getBal(last));
	hold_ = getBal(last);
	*A = node;
	*B = last;

	if(hold > 1 && hold_ >= 0){
		printf("RR rotation needed\n");
		return RR;	
	if(hold < -1 && hold_ <= 0 ){
	} 
		printf("LL rotation needed\n");
		return	LL;
	} 
	if(hold > 1 && hold_ < 0){
		printf("RL rotation needed\n");
		return RL;	
	} 
	if(hold < -1 && hold_ <= 0){
		printf("LR rotation needed\n");
		return LR;
	} 	
	}
}

ndptr rotate(ndptr A, ndptr B,rtt rtte, tree *t){
	{
		int i = (int)rtte;
		printf("trying to rotate: %c%c\n", TEMP_AVL[rtte][0],TEMP_AVL[rtte][1]);	
	}

	switch(rtte){
		case LL:
			{
			printf("\n\nINSIDE LL---------------\n");
			printf("A = %d and B = %d\n",A->dat,B->dat);

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
				if (A->way == BOTH){
					printf("A->way was: BOTH, setting to RIGHT\n");
					A->way = RIGHT;
				}
				if (A->way == LEFT){
					printf("A->way was: LEFT, setting to FREE\n");
					A->way = FREE;
				}
				if (B->way == FREE){
					printf("B->way was: FREE, setting to RIGHT\n");
					B->way = RIGHT;
				}
				if (B->way == LEFT){
					printf("B->way was: LEFT, setting to BOTH\n");
					B->way = BOTH;
				}
			} 
			//

			B->next[RIGHT] = A;
			if(B->fthr->dat == -100) t->root = B;
			printfTree(B);
			printf("END\n\n");
			return B;
			}
		break;

		case RR:
			{
			printf("\n\nINSIDE RR---------------\n");
			printf("A = %d and B = %d\n",A->dat,B->dat);
			printf("B->way =  %s\n",TEMP[B->way]);
			
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
				if (A->way == BOTH){
					printf("A->way was: BOTH, setting to LEFT\n");
					A->way = LEFT;
				}
				if (A->way == RIGHT){
					printf("A->way was: RIGHT, setting to FREE\n");
					A->way = FREE;
				}
				if (B->way == FREE){
					printf("B->way was: FREE, setting to LEFT\n");
					B->way = LEFT;
				}
				if (B->way == RIGHT){
					printf("B->way was: RIGHT, setting to BOTH\n");
					B->way = BOTH;
				}
			}
			B->next[LEFT] = A;
			if(B->fthr->dat == -100) t->root = B;
			printfTree(B);
			printf("END\n\n");
			return B;
			}
		break;

		case RL:
			{
			printf("\n\nINSIDE RL---------------\n");
			ndptr C = B->next[RIGHT];
			printf("A = %d ,B = %d ,C = %d\n",A->dat,B->dat,C->dat);
			
			rotate(B,C,RR,t);
			rotate(A,C,LL,t);
			printf("END\n\n");
			return C;
			}
		break;
		
		case LR:
			{
			printf("\n\nINSIDE LR---------------\n");
			ndptr C = B->next[LEFT];
			printf("A = %d ,B = %d, C = %d\n", A->dat,B->dat,C->dat);
			
			rotate(B,C,LL,t);
			rotate(A,C,RR,t);
			printf("END\n\n");
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
	//printf("trying to append %d",key);
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
	//printf("	appending: %d\n",newNd->dat);
	return	1;
}

void dltnode(tree *t,node * die){
	t->size--;
	printf("tring to delete %d", die->dat);
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
	printf("	deletion sucessful!\n");
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
	//printfTree(a.root);
	
}


