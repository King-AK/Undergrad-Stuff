#include <stdio.h>
#include <math.h>

//A few programs made while reading Concrete Mathematics
 
unsigned long cyclicshiftLeft(unsigned long number)
{
	//determine size of number, l -- 0.1 used for safety with number that are powers of two
	int l = ceil(log2(number+.01));
	//shift numbers
	unsigned long result = number << 1 | number >> 31;
	if(32 > l)
	{
		//NOTE : remember the bit string used to run from 0 to l-1, with shift, this becomes 0 to l
		//find out what bit at position l is
		int bit = (result >>(l))&1;
		//change 0th bit to lth bit
		result ^= (-(bit) ^ result) & (1<<0);
		//clear lth bit
		result &= ~(1<<(l));
	}
	return result;
}

void Josephus()
{
	//cyclic shift only applicable because power of two...every other person dies
	printf("How many people are there?\n");
	unsigned long number; scanf("%ld", &number);
	unsigned int position = cyclicshiftLeft(number);
	printf("To survive a circle where every other person is killed, Josephus must sit in position %ld in the circle!\n", position);
}
long twoPowered(long n)
{
	return (n==0 ? 0 : (2 << n-1));
}
void TowerofHanoi()
{	
	printf("how many disks are there?\n");
	long long disks; scanf("%lld", &disks);
	long long moves = disks == 0 ? 0 : (2 << (disks-1))-1;
	printf("It will take at least %lld moves to complete a game with %lld disks!\n",moves, disks);

}

int main()
{
	//create a simple menu that allows the user various options
	while(1)
	{
	printf("CONCRETE MATH PROBLEMS...\n\
select one...\n\
\t[1] Tower of Hanoi\n\
\t[2] Power of two\n\
\t[3] Josephus Problem\n");
	int input; scanf("%d", &input);
	if(input == 1)
		TowerofHanoi();
	else if(input == 2)
	{
		printf("Enter a number...\n");
		long number; scanf("%ld",&number);
		printf("2 to the %ld power is %ld\n", number, twoPowered(number));
	}
	else if(input == 3)
		Josephus();

	else
		printf("Invalid selection...\n");
    }
}