/* ------------------------------------------------------------------------- 
 * IDF Generation
 * Name            : idf.c  (Single Server Queue, version 3)
 * Author          : VJ Davey
 * Language        : ANSI C
 * Latest Revision : 11-01-15
 * ------------------------------------------------------------------------- 
 */

#include <stdio.h>
#include <math.h>
#include "rngs.h"                      /* the multi-stream generator */

#define A 1
#define N 90
double PDF(long x)
{
  return 2*((float)N-x)/((float)N *((float)N-1));
} 
double CDF(long x)
{
  if(x==1)
    return PDF(1);
  return CDF(x-1)+PDF(x);
}
long IDF(double u)
{
  long x = A;
  while(CDF(x) <= u)
    x++;
  return x;
}
long generateX(double u)
{
  SelectStream(0);
  return IDF(u);
}



  int main(void)
{
  PlantSeeds(0);
  printf("Generator program for HW7 Sim\n GOAL is to generate a value of X with one call to Random()\n");
  printf("Values will range from %d to %d\n", A, N);
  long i;
  for(i=A; i<N;i++)
    printf("When x = %d \t... Pr(x) == %f\t ...F(x) == %f \n", i, PDF(i), CDF(i));
  printf("NOTE that F(x) returns the upper bound\n");
  double u= Random();
  long x = generateX(u);
  printf("With u set to %f, we get the value %d from our set \n",u,x);
  return (0);
}
