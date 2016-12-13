#include <stdio.h>
//VJ Davey

int main()
{
	//assuming the user is entering a number under 10,000 , return the square of the input
	//done with use of scanf(), could be done with getchar() and then subtracting 48 from each ascii val corresponding to the integer entered


	int num;
	printf("Give me a nonegative number below ten thousand. I'll tell you its square!\n");
	scanf("%d",&num);
	printf("The square of that number is %d\n", num*num);
	return 0;
}