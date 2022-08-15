#include <stdlib.h>
#include <stdio.h>

#define M 2
#define MM M * 2

#define INTMAX  2147483647
#define INTMIN -2147483648
#define EMPTY -2
#define EXCEPTION MM + 2

typedef enum{
	false,true
}bool;

typedef struct page{
	int keys[MM];
	struct page * pages[MM + 3];//gambiarra de maiiiiiisss
	struct page * fthr;

	int index;
	bool dad;
	bool son;
}pg;

typedef pg * pgptr;

pg plchldr = {{EMPTY},{NULL},NULL,EMPTY,true,true};

void prtArr(int arr[], int size){
int aux;
	for(aux = 0; aux < size; aux++){
		if(arr[aux] == INTMIN || arr[aux] == INTMAX) printf(" S");
		else if(arr[aux] == EMPTY) printf("EMPTY\nturning off..."), exit(0);
		else printf(" %d",arr[aux]);
	}
}


int is_full(pgptr p){return p->index == MM;}

int is_empty(pgptr p){return p->index == 0;}

int has_son(pgptr p){return p->son;}

int has_fthr(pgptr p){return p->dad;}

int realSize(pgptr p){return p->index + 2;}

int pseudoVal(pgptr p, int pos){return pos > -1 && p->index > pos ? p->keys[pos + 1] : EMPTY;}

int pseudoPos(int pos){return pos + 1;}

//deslocmento do array inteiro 
void shiftArr(pgptr p, int empty, int final){
	int dir = empty > final ? -1 : 1;

	while(empty != final){
		p->keys[empty] = p->keys[empty + dir];
		p->pages[empty] = p->pages[empty + dir];
		empty += dir;
	}
	p->keys[final] = EMPTY;
	p->pages[final] = &plchldr;
}


void prtPage(pgptr p){
	printf("PAGE{\n\tFLAGS{\n\t\thas_son:%d\n\t\thas_fthr:%d\n\t\tindex: %d\n\t}\n\tKEYS{\n\t\t",has_son(p),has_fthr(p),p->index);
	prtArr((int *)p->keys,realSize(p));
	printf("\n\t}\n\tPAGES{\n\t\t");
	int aux;
	for(aux = 1; aux < MM + 3; aux++) p->pages[aux] == &plchldr ? printf(" E"):printf(" %ld",(long int)p->pages[aux]);
	printf("\n\t}\n}\n");
}

//criação de página formatada
pgptr crtPage(){
	pgptr rtrn = (pgptr)malloc(sizeof(pg)); 
	pg format = {{INTMIN,INTMAX},
				 {&plchldr,&plchldr,&plchldr,&plchldr,&plchldr,&plchldr,&plchldr}
				 ,NULL,0,0,0};
	*rtrn = format;
	return rtrn;
}

pgptr setSon(pgptr fthr, pgptr son, int pos){
	fthr->pages[pos] = son;
	fthr->son = true;

	son->fthr = fthr;
	son->dad = true;
}

//algoritimo de busca na página, retorna 0 em caso de falha ou 1 em caso de sucesso
//o endereço, ou pseudoendereço é inserido em status
int srchPage(pgptr p, int key, int * status){

	int mim = 0;
	int max = p->index + 1;
	int current;

	while(mim <= max){
		current = (max - mim)/2 + mim; 
		if(p->keys[current] == key){
			*status = current;
			 return 1;
		}

		else if(p->keys[current] < key) mim = current + 1;
		else if(key < p->keys[current]) max = current - 1; 
	}

	*status = max;
	return 0;
}


//tentativa de inserção simples 
int tryInsPg(pgptr p, int key){
	if(is_full(p)) return 0;
	
	int status;
	if(!srchPage(p,key,&status)){
		status++,p->index++;

		shiftArr(p,realSize(p) - 1,status);
		p->keys[status] = key;

	}
	// prtPage(p);
}

int selecRemove(pgptr p, bool dir){
	int rtrn = pseudoVal(p, (p->index - 1) * (!dir) );

	if(dir) shiftArr(p,1,realSize(p)-1);
	else p->keys[p->index] = INTMAX;

	p->index--;
	return rtrn;
}



void e_xception(pgptr p, pgptr newPg){
	int val = pseudoVal(p,p->pages[EXCEPTION]->index -1);
	int aux;
	pgptr save = p->pages[EXCEPTION];

	for(aux = pseudoPos(0); aux < EXCEPTION  ; aux++){
		if(val < pseudoVal(p,newPg->pages[aux]->keys[0])){
			int final = EXCEPTION;

			while(aux != final){
				p->pages[final] = p->pages[final -1];
				final--;
			}
			p->pages[aux] = save;	
			break;
		} 
	}
	for(aux = pseudoPos(3); aux <= pseudoPos(5); aux++){
		setSon(newPg,p->pages[aux],pseudoPos(aux - pseudoPos(3)));
		p->pages[aux] = &plchldr;
	}

}



//inserção complexa parcial
pgptr explodePg(pgptr p, int key){
	// printf("original page:\n\n");
	// prtPage(p);

	int status;
	srchPage(p,key,&status);
	pgptr newPg = crtPage();
	
	tryInsPg(newPg,pseudoVal(p,2));
	tryInsPg(newPg,pseudoVal(p,3));

	p->index -= 2;
	p->keys[realSize(p) - 1] = INTMAX;
	

	int carry;
	if(pseudoVal(p,status) != EMPTY){
		tryInsPg(p,key);
		carry = selecRemove(p,0);
	}
	else{
		tryInsPg(newPg,key);
		carry = selecRemove(newPg,1);
	}

	if(has_son(p)){
		e_xception(p,newPg);
	}

	if(has_fthr(p)){
		
		if(!tryInsPg(p->fthr,carry)){
			p->fthr->pages[EXCEPTION] = newPg;
			return explodePg(p->fthr,carry);
		}
		else{
			srchPage(p->fthr,carry,&status);	
			setSon(p->fthr,p,status);
			setSon(p->fthr,newPg,status + 1);
			return p->fthr;
		}

	}
	else{ 
		pgptr newRoot = crtPage();
		tryInsPg(newRoot,carry);

		setSon(newRoot,p,pseudoPos(0));
		setSon(newRoot,newPg,pseudoPos(1));
		
		// printf("\n\nold pg:\n\n");
		// prtPage(p);
		// printf("\n\nnew pg:\n\n");
		// prtPage(newPg);
		// printf("\n\nnew root:\n\n");
		// prtPage(newRoot); 
	
		return newRoot;
	}
}


// pgptr explodePg(pgptr p, int key){

// }

// pgptr appendPg(pgptr p, int key){
// 	if(!tryInsPg(p,key)){
// 		explodePg();
		
// 	}
// 	return p;
// }


typedef struct bTree{
	pgptr root;
}bt;
typedef bt * btptr;


//criação de árvore formatada
btptr crtTree(int key){
	btptr rtrn = malloc(sizeof(bt));
	rtrn->root = crtPage();
	tryInsPg(rtrn->root,key);
	
	return rtrn;
} 

void prtTree(pgptr root){
	printf("\n=======================================================================\n");
	prtPage(root);
	if(has_son(root)){
		int aux;

		for(aux = pseudoPos(0); aux <= pseudoPos(root->index); aux++){
				printf("\nSON:%d\n",aux);
				prtPage(root->pages[aux]);
		}
		
		printf("\n=======================================================================\n");
		for(aux = pseudoPos(0); aux <= pseudoPos(root->index); aux++){
			prtTree(root->pages[aux]);	
		}
	}
	else{
		printf("\n=======================================================================\n");
	}
}

//algoritimo de busca na arvore 
pgptr srchTree(pgptr bgn,int key){
	int status;

	while(1){
		if(has_son(bgn)){
			srchPage(bgn,key,&status);
			bgn = bgn->pages[status + (bgn->keys[status] < key) ]; 
		}
		else break;
	}
	return bgn;
}

pgptr insertTree(pgptr bgn, int key){
	pgptr home = srchTree(bgn,key);
	if(!tryInsPg(home,key)){
		return explodePg(home,key);
	}
	else return bgn;
}


int main(){
	int hold;
	scanf("%d",&hold);
	btptr t = crtTree(hold);

	while(1){
		scanf("%d",&hold);
		if(hold < 0)break;
		t->root = insertTree(t->root,hold);
	}
	
	int dummy;
	scanf("%d",&hold);
	pgptr result = srchTree(t->root,hold);
	
	printf("%d\n",t->root->index);
	if(srchPage(result,hold,&dummy)){
		printf("%d",result->index);
	}
	else printf("Valor nao encontrado");
}





