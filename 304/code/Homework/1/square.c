#include <stdio.h>

//VJ Davey 
//Program meant to accept an input of integer n
//print the square of 1,2,3,...n
//print the sum of the squares computed in the above line

//to be saved as square.c

int main()
{
	int in;
	printf("Please enter a positive integer number : \n");
	scanf("%d", &in);
	int total = 0;
	for(int i=1; i <= in; i++ )
	{
		printf("The square of %d is %d\n", i, i*i);
		total += i*i;
	}
	printf("The sum of the squares is %d\n", total);
	return 0;
}