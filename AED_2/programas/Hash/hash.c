#include <stdlib.h>
#include <stdio.h>

typedef struct node{
	struct node * next;
	int dat; 
}nd;
typedef nd * ndptr;

ndptr crtNd(int dat){
	ndptr rtrn = malloc(sizeof(nd));
	nd format = {NULL,dat};
	*rtrn = format;

	return rtrn;
}

int is_last(ndptr n){return n->next == NULL;}



#define HEAD 0
#define TAIL 1

typedef struct linkedList{
	ndptr ends[2];
}ll;
typedef ll * llptr;

int is_empty(llptr l){return l->ends[0] == NULL && l->ends[1] == NULL;}

int is_almost_empty(llptr l){return l->ends[0] == l->ends[1];}

int is_first(ndptr n, llptr l){return l->ends[HEAD] == n;}


ll crtLl(){
	ll rtrn = {{NULL,NULL}};
	return rtrn;
}

void insNd(llptr l,int dat){
	ndptr ins = crtNd(dat);

	if(is_empty(l)) l->ends[HEAD] = ins;
	else l->ends[TAIL]->next = ins;
		
	l->ends[TAIL] = ins;
}

int srchLl(llptr l, int key){

	ndptr hold = l->ends[HEAD];
	do{
		if(hold->dat == key) return 1;
		else hold = hold->next;
	}while(hold != l->ends[TAIL]);

	return 0;
}


void prtLl(ll l){
	ndptr hold = l.ends[HEAD];
	if(is_empty(&l)){
		return;
	}
	while(1){
		printf(" %d",hold->dat);
		if(is_last(hold)) return;
		hold = hold->next;
	}
}



typedef struct hash{
	ll * items;
	int size;
}hsh;
typedef hsh * hsptr;

hsh crtHash(int size){
	hsh rtrn = {malloc(sizeof(ll) * size),size};
	int aux;
	for(aux = 0; aux < size; aux++){
		rtrn.items[aux] = crtLl();
	}
	return rtrn;
}

int gtAdrs(hsptr h, int key){return key % h->size;}

void insHash(hsptr h, int key){
	insNd(&h->items[gtAdrs(h,key)],key);}

void rmNd(hsptr h, int key){
	llptr l = &h->items[gtAdrs(h,key)];
	
	if(is_empty(l)) return;
	else if(is_almost_empty(l) && l->ends[HEAD]->dat == key){
		free(l->ends[HEAD]);
		*l = crtLl(); 
	}
	else{
		ndptr hold = l->ends[HEAD];
		ndptr prev = hold;
		
		while(1){
			if(hold->dat == key){
	
				if(is_first(hold,l)){
				 	l->ends[HEAD] = hold->next;
				}
				else if(is_last(hold)){
					l->ends[TAIL] = prev;
					prev->next = NULL;
				}
				else{
					prev->next = hold->next;	
				}
				free(hold);
				return;
			}
			else{
				if(hold->next == NULL) break;
				prev = hold;
				hold = hold->next;
			}
		}
		printf("Valor não encontrado\n");	
	}
}

void prtHash(hsh h){
	int aux;
	for(aux = 0 ; aux < h.size ; aux++){
		prtLl(h.items[aux]);
		printf("\n");
	}
}


int main(){
	int hold;
	scanf("%d",&hold);

	hsh h = crtHash(hold);
	
	while(1){
		scanf("%d",&hold);
		if(hold < 0) break;
		insHash(&h,hold);
	}
	
	scanf("%d",&hold);
	rmNd(&h,hold);
	
	scanf("%d",&hold);
	printf("[%d]",hold);
	prtLl(h.items[hold]);

}