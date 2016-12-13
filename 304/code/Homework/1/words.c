#include <stdio.h>
//VJ Davey

int main()
{
	//counts the number of words in a string by incrementing the count everytime a white space is entered which
	//doesnt follow a whitespace, eg only whitespaces following <visible> characters

	int count = 0;//wordcount
	char c; //current char
	char p=32; //previous char
	c = getchar();
	while(c!='\n')
	{
		if(c != 32 && p== 32) {count++;} //32 is ASCII code for whitespace
		p = c;
		c = getchar();

	}
	printf("There are %d words in that string.\n", count);

	return 0;
}