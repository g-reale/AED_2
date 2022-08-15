#include <stdio.h>
#include <stdlib.h>

#define LEFT 0
#define RIGHT 1
#define FREE 2
#define LOCKED 3
#define SUCESS 7

//TEMP///
char TEMP[8][10] = {"LEFT","RIGHT","FREE","LOCKED","~","~","~","SUCESS"};
///

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//NODE DEFINITIONS
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct nde{
	int dat;
	struct nde * next[2];
}node;

typedef node * ndptr;

ndptr WALL;

node crtNode(int key){
	node rtrn;
	rtrn.dat = key;
	rtrn.next[0] = WALL;
	rtrn.next[1] = WALL; 
	return rtrn;
}

int findPth(ndptr lost){
	int p = LOCKED;
	if(lost->next[0] != WALL && lost->next[1] != WALL) p = FREE; 
	if(lost->next[0] != WALL && lost->next[1] == WALL) p = LEFT;
	if(lost->next[0] == WALL && lost->next[1] != WALL) p = RIGHT;
	
	return p;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//TREE DEFINITIONS 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tre{
	ndptr nodes;
	int size;
}tree;

tree crtTree(int root){
	tree rtrn;
	rtrn.nodes = (ndptr) malloc(1 * sizeof(node));
	rtrn.nodes[0] = crtNode(root);
	rtrn.size = 1;
	//printf("Creating tree, node[0] is %d and size is %d\n",rtrn.nodes[0].dat,rtrn.size);
	return rtrn;
}

ndptr srchTree(int key, ndptr road, int * status){
	if(road->dat == key){
		*status = SUCESS;//sucesso ! 
		return road;//retornando a chave encontrada
	}
	else{
		int semaphore = findPth(road);
		int way;
		
		//vê o caminho
		if(road->dat < key) way = RIGHT;//tenho que ir para a direita!
		else way = LEFT;//tenho que ir para a esquerda!
		printf("srchTree: \n");
		printf("my way is: %s because i`m worth %d and this node is %d by the way more info on this node:\n",TEMP[way],key,road->dat);	
		printf("left: %d right: %d road status: %s adress: %ld \n\n\n",road->next[0]->dat,road->next[1]->dat,TEMP[semaphore],(long int)road);	

		//tenta atrevessar
		if(semaphore == FREE || semaphore == way) srchTree(key,road->next[way],status);//atrevessei!
		else{//o semáfaro ta fechado 
			*status = way;//falha 
			return road;//retornando o endereço mais próximo
		}
	}
}

int apndTree(tree * t, int key){
	int dir;
	ndptr father = srchTree(key,&t->nodes[0],&dir);

	if(dir == SUCESS) return LOCKED;
	else{
		t->size++;
		t->nodes = realloc(t->nodes,t->size * sizeof(node));
		t->nodes[t->size-1] = crtNode(key);
		father->next[dir] = &t->nodes[t->size-1];
		printf("\ntree apended: father is %d, son is %d, son confirmation is %d\n",father->dat,father->next[dir]->dat,t->nodes[t->size-1].dat);
		printf("the son stays on the %s of the father\n",TEMP[dir]);
		int temp; 
		for(temp = 0; temp <t->size; temp++){
			printf("%d ",t->nodes[temp].dat);
		}
		printf("\n\n");
	return dir;
	}
}

void printfTree(tree t){
	int i;
	printf("the tree size is: %d\n",t.size);
	printf("linear: ");
	for( i = 0; i < t.size; i++){
		printf("%d ",t.nodes[i].dat);
	}
	printf("\n\nHierachy:	\n");
	for( i = 0; i < t.size; i++){
		printf("my dat: %d, me: %ld, my left: %ld, my right: %ld\n",t.nodes[i].dat,(long int)&t.nodes[i],(long int)t.nodes[i].next[0],(long int)t.nodes[i].next[1]);
	}
	printf("\n\nFriendly view:\n");
	for( i = 0; i < t.size; i++){
		printf("%d: %d , %d\n",t.nodes[i].dat,t.nodes[i].next[0]->dat,t.nodes[i].next[1]->dat);
	}
}

int main(){
	//temp
	WALL = malloc(sizeof(node));
	WALL->dat = -1;
	//temp

	int buff = -1;
	scanf("%d",&buff);
	tree a = crtTree(buff);
	
	printf("before anithing else: %d\n\n\n",a.nodes[0].dat);

	while(1){
		scanf("%d",&buff);
		if(-1 < buff) apndTree(&a,buff);
		else break;
	}
	printf("wall is: %ld\n",(long int)WALL); 

	printfTree(a);
	/*
	int dat

		while(-1 < dat){
			scanf("%d",&dat);
		}
		while(-1 < dat){
			scanf("%d",&dat);
		}
		scanf("%d",&dat);

	return 0;
	*/
}