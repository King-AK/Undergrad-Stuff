#include <stdio.h>
/*
VJ Davey
This program was made to complete exercise 2.2.11 in the textbook. 
Its purpose is to find all full period multipliers for a given large prime number.
*/
long gcd( long a, long b)
{
	if(b == 0)
		return a;
	else
		return gcd(b, a % b);
}
void findOtherMultipliers(long a, long m)
{
	int i = 1;
	int full_period_count = 0;
	int modulus_compatible_count = 0;
	long x = a;
	const long q = m / a;
  	const long r = m % a;

	while( x != 1)
	{
		if((m % x < m / x))
		{
			full_period_count++;
			if(gcd(i, m - 1) == 1)
			{
				modulus_compatible_count++;
				//printf("%d, ", x);
			}
				
		}
		i++;
		x = a * (x%q) - r * (x/q);
	}
	printf("\nthere are %d numbers that are full period multipliers",full_period_count);
	printf("\nthere are %d numbers that are full period modulus compatible multipliers",modulus_compatible_count);


}
int isMultiplier(long long a, long long m)
{
	int p = 1; // p - counter, 
	long long x = a; //x - initial seed
	const long q = m / a;
  	const long r = m % a;
	while(x != 1)
		{
			p++;
			x = a * (x%q) - r * (x/q);
		}
	if( p == (m-1))
		return 1;
	else
		return 0;
}



int main(void)
{
	long long prime;
	long long a = 0;
	int found = 0;
	printf("Give me a large prime number (BEWARE OF OVERFLOW)...   \n");
	scanf("%d",&prime);
	while(found != 1)
		{
			a++;
			found = isMultiplier(a, prime);
		}
	printf("\n %d is the first full period multiplier\n",a);
	findOtherMultipliers(a,prime);
	printf("\nDone");
	return 0;
}
