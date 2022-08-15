#include <stdio.h>
#include <stdlib.h>

#define ADRS 1
#define COST 0

#define INT_MAX 2147483647

#define LOCK 0
#define OPEN 1


#define OFFSET(var)({var + 1024;})
#define UFFSET(var)({var - 1024;})

typedef struct node{
	int * links[2];
	int paths;
	int me;
}nd;
typedef nd * ndptr;

typedef struct graph{
	ndptr nodes;
	int size;
}gph;
typedef gph * gphptr;

nd crtNode(int me){
	nd rtnr = {{malloc(sizeof(int)),malloc(sizeof(int))},0,me};
	return rtnr;
}

void prtNode(nd n){
	printf("NODE{\n");
	printf("\tint me: %d\n",n.me);
	printf("\tint paths: %d\n",n.paths);
	
	printf("\n\tint ** links{\n\t\t");
	int aux; 
	for ( aux = 0; aux < n.paths; ++aux)
	{
		printf("[%d](%d,%d) ",aux,n.links[ADRS][aux],n.links[COST][aux]);
	}
	printf("\n\t}\n}");
}

gph crtGraph(){
	gph rtnr = {malloc(sizeof(nd)),0}; 
	return rtnr;
}

void insGraph(gphptr g){
	g->size++;
	g->nodes = (ndptr)realloc(g->nodes,sizeof(nd) * g->size);

	g->nodes[g->size - 1] = crtNode(g->size-1);
}

void linkNode(gphptr g, int bgn, int end, int cost){
	ndptr value = &g->nodes[bgn];
	value->paths++;
	
	value->links[ADRS] = (int*)realloc(value->links[ADRS],sizeof(int) *  value->paths);
	value->links[COST] = (int*)realloc(value->links[COST],sizeof(int) *  value->paths);

	value->links[ADRS][value->paths -1] = end;
	value->links[COST][value->paths -1] = cost;

}

void prtGraph(gphptr g){
	int aux;
	printf("GRAPH SIZE: %d\n",g->size);
	printf("ELEMENTS:\n");
	for(aux = 0; aux < g->size; aux++){
		printf("\n[%d] ",aux);
		prtNode(g->nodes[aux]);
	}
}

int relax(gphptr g, int * cost, int * open, int do_not){
	int * dis[2] = {cost,open}; 
	int aux, small = -1, done = 0; 
	
	for(aux = 0; aux < g->size; aux++){
		if(dis[OPEN][aux] == OPEN){
			if(dis[COST][small] > dis[COST][aux] || small == -1) small = aux;
		}
		else done++;
	}
	if(done == g->size) return 0;
	dis[OPEN][small] = LOCK;

	// printf("\nduring: ");
	// for (aux = 0; aux < g->size; aux++) printf(" (%d , %d) ",dis[COST][aux],dis[OPEN][aux]);
	// printf("\nchosen node: ");
	// prtNode(g->nodes[small]);
	
	ndptr val = &g->nodes[small];
	

	for(aux = 0; aux < val->paths; aux++){
		if(val->links[COST][aux] + dis[COST][small] < dis[COST][val->links[ADRS][aux]] &&
		   val->links[COST][aux] <= do_not ){
			dis[COST][val->links[ADRS][aux]] = val->links[COST][aux] + dis[COST][small];
		}
	}
	
	return 1;
}

void Diikstra(gphptr g, int tgt, int do_not){
	int dis[2][g->size],aux;
	
	for (aux = 0; aux < g->size; aux++) dis[COST][aux] = INT_MAX,dis[OPEN][aux] = OPEN;
	dis[COST][tgt] = 0;	
	// printf("before: ");
	// for (aux = 0; aux < g->size; aux++) printf("(%d , %d) ",dis[COST][aux],dis[OPEN][aux]);

	while(relax(g, dis[COST],dis[OPEN], do_not));//gambiarra	

	// printf("\nafter: ");
	for (aux = 0; aux < g->size; aux++) printf("%d ",dis[COST][aux]);
}

int main(){
	gph test = crtGraph();
	
	int hold,R,V;
	scanf("%d",&V);
	scanf("%d",&R);

	scanf("%d",&hold);
	while(hold){
		insGraph(&test);
		hold--;
	}
	int bgn,end,cost;
	scanf("%d",&hold);
	while(hold){
		scanf("%d",&bgn);
		scanf("%d",&end);
		scanf("%d",&cost);
		linkNode(&test,bgn,end,cost);
		hold--;
	}

	// prtGraph(&test);
	// printf("\n");
	Diikstra(&test,V,R);

	return 1;
}