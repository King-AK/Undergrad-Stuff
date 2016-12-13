#include <stdio.h>
//VJ Davey
//counts the number of spaces in a given string

int main()
{
	int count = 0;
	char c;
	c = getchar();
	while(c!='\n')
	{
		if(c == 32) {count++;} //32 is ASCII code for whitespace
		c = getchar();
	}
	printf("There are %d spaces in that string.\n", count);
	return 0;

}