#include <stdio.h>
#include <stdlib.h>

/*		 TEMP		*/
char TEMP[4][20] = {"LEFT","RIGHT","FREE","BOTH"};
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
}node;

typedef node * ndptr;

node crtNode(int key){
	node rtrn;
	rtrn.dat = key;
	rtrn.way = FREE; 
	return rtrn;
}

/*		NODE DEFINITIONS		*/


/*		TREE DEFINITIONS		*/
typedef struct tre{
	ndptr root;
	int size;
}tree;

tree crtTree(int root){
	tree rtrn;
	rtrn.root = (ndptr) malloc(1 * sizeof(node));
	*(rtrn.root) = crtNode(root);
	
	rtrn.root->fthr = (ndptr) malloc(1 * sizeof(node));;
	rtrn.root->fthr->dat = -100;
	rtrn.root->fthr->way = RIGHT;
	
	rtrn.size = 1;
	return rtrn;
}

void dltnode(node * die){
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
				free(srch);
			}
		break;
		
		case FREE:
			{
				if(die->fthr->way < FREE) die->fthr->way = FREE;
				else die->fthr->way = die->dat < die->fthr->dat;
				free(die);
			}
		break;
	
		default:
			{
				path turn = die->dat > die->fthr->dat;
				die->fthr->next[turn] = die->next[die->way];
				die->next[die->way]->fthr = die->fthr;
				free(die);
			}
		break;
	}
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
	return	1;
}

int getHght(ndptr node, int hght){
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

void printfTree(ndptr begin){
	if(begin->way == BOTH){
		printf("---------------------------------------\n");
		printf("\tfather: %d \nLEFT: %d \tRIGHT: %d\n",begin->dat,begin->next[LEFT]->dat,begin->next[RIGHT]->dat);
		printf("---------------------------------------\n\n");
		printfTree(begin->next[LEFT]);
		printfTree(begin->next[RIGHT]);
	}
	else if(begin->way < FREE){
		printf("---------------------------------------\n");
		printf("\tfather: %d \n%s: %d \n",begin->dat,TEMP[begin->way],begin->next[begin->way]->dat);
		printf("---------------------------------------\n\n");
		printfTree(begin->next[begin->way]);
	}
	else{
		printf("---------------------------------------\n");
		printf("\tfather: %d\n",begin->dat);
		printf("---------------------------------------\n\n");
	}
}

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
		int l = 0; 
		int	r = 0; 
		int	b = 0;

		if(a.root->way == LEFT || a.root->way == BOTH){
			l = getHght(a.root->next[LEFT],0);	
		}
		if(a.root->way == RIGHT || a.root->way == BOTH){
			r = getHght(a.root->next[RIGHT],0);	
		}
		
		b = l > r ? l : r;
		printf("%d, %d, %d\n",b,l,r);	
	}


	while(1){
		scanf("%d",&buff);
		if(-1 < buff){
			if(!apndTree(&a,buff,&hold)){
				dltnode(hold);
			}
		}
		else break;
	}

	scanf("%d",&buff);

	path exists;
	hold = srchTree(&a,buff,&exists);
	if(hold->dat == buff){
		int l = 0; 
		int	r = 0; 
		int	b = 0;

		if(hold->way == LEFT || hold->way == BOTH){
			l = getHght(hold->next[LEFT],0);	
		}
		if(hold->way == RIGHT || hold->way == BOTH){
			r = getHght(hold->next[RIGHT],0);	
		}
		
		b = l > r ? l : r;

		printf("%d, %d, %d\n",b,l,r);	

	}
	else printf("Valor nao encontrado");
}


