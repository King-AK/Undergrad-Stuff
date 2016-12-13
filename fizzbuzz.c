/*
 FIZZ BUZZ program from CodeEval - VJ Davey
Your program should accept a file as its first argument. 
The file contains multiple separated lines; each line contains 3 numbers that are space delimited. 
The first number is the first divider (X), the second number is the second divider (Y), 
and the third number is how far you should count (N). 
You may assume that the input file is formatted correctly and the numbers are valid positive integers. 

*/
#include <stdio.h>
#include <stdlib.h>
void fizzbuzz(int x, int y, int n)
{
	int i;
	for(i=0; i <= n; i++)
	{
		if((i%x == 0)&&(i%y != 0)){
			printf("F ");
		}else if((i%y == 0) && (i%x != 0)){
			printf("B ");
		}else if(i%(x*y) == 0){
			printf("FB ");
		}else{
			printf("%d ", i);
		}
	}
	printf("\n");
}

int main(int argc, char const *argv[])
{ 
	/*TODO: Write a way to take a file as input.
		-for each line in the file, we need to parse it by spaces 
		-numbers are in between the spaces, and are the arguments for fizzbuzz() in order
		-each "\n" in the file is a new time to run fizzbuzz()

	PARSING + NUMBER IDENTIFICATION
	*/
	int x,y,n;
	while!EOF {
		//parse the current line
		x=first int;
		y=second int;
		x=third int;
		fizzbuzz(x,y,n);
	}
}