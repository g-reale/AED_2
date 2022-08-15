#include <stdio.h>
#include <stdlib.h>

int main(){
	int a;
	scanf("%d",&a);
	printf("%d",a-100);

	while(a > -1){
		scanf("%d",&a);
		printf(",%d", a - 100);
	}
	return 0;
}