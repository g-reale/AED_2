#include <stdio.h>
#include <stdlib.h>

/*		 TEMP		*/
char TEMP[4][20] = {"LEFT","RIGHT","FREE","BOTH"};
/*		 TEMP		*/


/*		NODE DEFINITIONS		*/
typedef enum {
	LEFT, RIGHT, FREE, BOTH
} path;

typedef struct nde{
	int dat;
	struct nde * next[2];
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

void dltnode(tree * t, node  *die){//falta realocar a arvore
	ndptr son;
}

ndptr srchTree(tree * t,int key, path * turn){
	ndptr srch = &t->nodes[0]; 

	while(1){
		*turn = BOTH;
		if(srch->dat == key) break;
		
		if(srch->dat < key) *turn = RIGHT;
		else *turn = LEFT;

		if(srch->way == *turn || srch->way == BOTH) srch = srch->next[*turn];
		else break; 
	}

	return srch;
}

int apndTree(tree * t,int key){
	path status;
	ndptr father = srchTree(t,key,&status);

	if(status == BOTH) return 0;

	t->size++;
	t->nodes = (ndptr)realloc(t->nodes, t->size * sizeof(node));
	t->nodes[t->size-1] = crtNode(key);

	//gambiarra
	if(t->size-1 == 1) father = &t->nodes[0];
	//

	father->next[status] = &t->nodes[t->size-1];

	if(father->way == FREE) father->way = status;
	else father->way = BOTH;

	printf("appending: father is %d, son is: %d, apending on: %s, father is ocupied on %s\n",father->dat,father->next[status]->dat,TEMP[status],TEMP[father->way]);
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
		printf("my dat: %d, me: %ld, my left: %ld, my right: %ld\n",t.nodes[i].dat,(long int)&t.nodes[i]%1000,(long int)t.nodes[i].next[0]%1000,(long int)t.nodes[i].next[1]%1000);
	}
	/*printf("\n\nFriendly view:\n");
	for( i = 0; i < t.size; i++){
		printf("%d: %d , %d\n",t.nodes[i].dat,t.nodes[i].next[0]->dat,t.nodes[i].next[1]->dat);
	}
	*/
}

int main(){

	int buff;
	scanf("%d",&buff);
	tree a = crtTree(buff);

	printf("before anithing else: %d\n\n\n",a.nodes[0].dat);

	while(1){
		scanf("%d",&buff);
		if(-1 < buff) apndTree(&a,buff);
		else break;
	}
	
	printfTree(a);

}


