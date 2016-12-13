#include <stdio.h>
/*
VJ Davey
This program was made to complete exercise 2.2.11 in the textbook. 
Its purpose is to find all full period multipliers for a given large prime number.
*/
long long gcd( long long a, long long b)
{
	if(b == 0)
		return a;
	else
		return gcd(b, a % b);
}
void findOtherMultipliers(long long a, long long m)
{
	int i = 1;
	long long full_period_count = 0;
	long long modulus_compatible_count = 0;
	long long x = a;
	long long special = 0;
	const long long q = m / a;
  	const long long r = m % a;
  	printf("The 10 smallest full period modulus compatible numbers are:\n");
	while( x != 1)
	{
		if((m % x < m / x))
		{
			full_period_count++;
			if(gcd(i, m - 1) == 1)
			{
				modulus_compatible_count++;
				if (modulus_compatible_count<11){printf("%lld, ", x);}//shows 10 smallest
				if (x >= 100000 && special == 0) {special = x;}//finding our special number
			}
				
		}
		i++;
		x = a * (x%q) - r * (x/q);
	}
	printf("\nthere are %lld numbers that are full period multipliers",full_period_count);
	printf("\nthere are %lld numbers that are full period modulus compatible multipliers",modulus_compatible_count);
	if(special !=0)printf("\nOur special modulus compatible multiplier a is %lld",special);


}
int isMultiplier(long long a, long long m)
{
	int p = 1; // p - counter, 
	long long x =  a; //x - initial seed
	const long long q = m / a;
  	const long long r = m % a;
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
	long long a = 1;
	int found = 0;
	printf("Give me a large prime number (BEWARE OF OVERFLOW)...   \n");
	scanf("%lld",&prime);
	printf("\n%lld was inputted...\n",prime);
	while(found != 1 && a<prime)
		{
			a++;
			found = isMultiplier(a, prime);
		}
	printf("\n %lld is the first full period multiplier\n",a);
	findOtherMultipliers(a,prime);
	printf("\nDone");
	return 0;
}
