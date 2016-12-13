#include <stdio.h>

//calculates delay times for a single server FIFO node
int main(int argc, char const *argv[])
{
	c = 0.0; 
	i = 0;
	while (jobs > 0)
	{
		i++;
		a = get_arrival();
		if ( a < (c -1))
			d = (c-1) - a; // calculate delay for hob
		else
			d = 0.0; //job has no delay
		s = get_service();
		c = a + d + s; //calculate departure time for the job
	}
	n = i;
	return d;
}