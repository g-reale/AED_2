#define WALL (ndptr) 100
#define FAIL -1
#define LEFT 0
#define RIGHT 1
#define FREE 2
#define SUCESS 7

#include <stdio.h>
#include <stdlib.h>

///Node "Class"////////////////////////////////
typedef struct nde{
	int dat;
	struct nde * next[2];
}node;

typedef node * ndptr;

node crtNode(int key, ndptr left, ndptr right){
	node rtrn;
	rtrn.dat = key;
	rtrn.next[0] = left;
	rtrn.next[1] = right; 
	//printf("Creating Node, the key is %d, left is %d and right is %d\n",key,(int)left,(int)right);
	return rtrn;
}
//////////////////////////////////////////////
//Path "Class"////////////////////////////////

int findPth(node * lost){
	int p = FAIL;
	if(lost->next[0] != WALL && lost->next[1] != WALL) p = FREE; 
	if(lost->next[0] != WALL && lost->next[1] == WALL) p = LEFT;
	if(lost->next[0] == WALL && lost->next[1] != WALL) p = RIGHT;
	
	return p;
}
//////////////////////////////////////////////

typedef struct tre{
	node * nodes;
	int size;
}tree;

tree crtTree(int root){
	tree rtrn;
	rtrn.nodes = (ndptr) malloc(1 * sizeof(node));
	rtrn.nodes[0] = crtNode(root,WALL,WALL);
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

	if(dir == SUCESS) return FAIL;
	else{
		t->size++;
		t->nodes = realloc(t->nodes,t->size * sizeof(node));
		t->nodes[t->size-1] = crtNode(key,WALL,WALL);
		father->next[dir] = &t->nodes[t->size-1];
	
	return dir;
	}
}

void printfTree(tree t){
	int i;
	printf("the tree size is: %d \n \n",t.size);
	for( i = 0; i < t.size; i++){
		printf("my dat: %d, me: %ld, my left: %ld, my right: %ld\n",t.nodes[i].dat,(long int)&t.nodes[i],(long int)&t.nodes[i].next[0],(long int)&t.nodes[i].next[1]);
	}
}

int main(){
	tree a;
	int b;
	a = crtTree(1);
	printfTree(a);
	//printf("\n\n%ld		%ld\n\n",(long int)srchTree(1,&a.nodes[0],&b),(long int)&a.nodes[0]);
	//printf("\n%d\n",b);
	apndTree(&a,2);
	apndTree(&a,-2);
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