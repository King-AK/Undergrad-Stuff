#include <stdio.h>

long fibonacci(x)
{
	if(x==0)
		return 0;
	if(x==1)
		return 1;
	return fibonacci(x-1)+fibonacci(x-2);
}



int main(){
	long number;
	printf("How many fibonacci numbers u want fam?\n");
	scanf("%d",&number);
    long i;
    for(i=1;i<=number;i++)
    	printf("number %d fibonacci number is... \t %d\n",i, fibonacci(i));
}