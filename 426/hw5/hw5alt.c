/* ------------------------------------------------------------------------- 
 * This program is an edit on - an extension of program ssq2.c - simulates a single-server 
 * FIFO service node using Exponentially distributed interarrival times and 
 * Uniformly(for problem 2) and Exponentially distributed service times (i.e. a M/U/1 queue). 
 *
 * Mr. K's snowball stand
 * Two workers - Fran and Bill
 *
 * Name              : hw5.c  (Single Server Queue, version 2)
 * Author            : VJ Davey
 * Language          : ANSI C 
 * Latest Revision   : 10-19-15
 * ------------------------------------------------------------------------- 
 */

#include <stdio.h>
#include <math.h>                                             
#include "rngs.h"

#define LAST         1000000L                   /* number of jobs processed */ 
#define START        0.0                      /* initial time             */ 
#define MU           20 //switch here based on whether we are using Bill or Fran's average

   double Exponential(double m)                 
/* ---------------------------------------------------
 * generate an Exponential random variate, use m > 0.0 
 * ---------------------------------------------------
 */
{                                       
  return (-m * log(1.0 - Random()));     
}


   double Uniform(double a, double b)           
/* --------------------------------------------
 * generate a Uniform random variate, use a < b 
 * --------------------------------------------
 */
{                                         
  return (a + (b - a) * Random());    
}


   double GetArrival(void)
/* ------------------------------
 * generate the next arrival time
 * ------------------------------
 */ 
{       
  static double arrival = START;                                        

  arrival += Exponential(60); 
  return (arrival);
}

   double GetServiceExponential(void)
/* ------------------------------
 * generate the next service time -- Used for part a
 * ------------------------------
 */ 
{
  return (Exponential(MU));
}

   double GetServiceUniform(void)
/* ------------------------------
 * generate the next service time -- Used for part b
 * ------------------------------
 */ 
{
  return (Uniform(5, 35));
}


  int main(void)
{
  long   index     =     0;                     /* job index            */
  long   reject_count =  0;                     /*num of rejected jobs  */
  long   state =         0;                     /*the state of the system*/
  long   capacity;                              /*capacity of system     */
  double arrival   = START;                     /* time of arrival      */
  double service;                               /* service time-always changing*/

  printf("Enter the max line size before customers begin turning away ... \n");
  scanf("%ld",&capacity); //when state = capacity, we turn away customers...
  double departure[capacity];
  long k;
  for(k=0;k<capacity;k++)
    departure[k] = 0; 

  PlantSeeds(0); 

  /*Each slot in the array represents a place in line, and holds
  * the departure time for the job in that place in line.
  *
  * The job at position n of the array equates to the job of state n+1
  * An empty array equates to being in state 0
  */
  while (index < LAST) {
    //we look at the state. We have a few different options depending on the state...
    //starting off... we should always begin in state 0...
    /*long p; -- THIS IS FOR LOOKING AT THE NODE STATE
    printf("[");
    for(p=0; p<capacity;p++)
    {
      printf("%f, ", departure[p]);
    }
    printf("]\n");*/
    if(state == 0)
    {
      arrival = GetArrival();
      //service = GetServiceExponential();
      service = GetServiceUniform();
      departure[state] = arrival + service;
      state += 1;
    }else if(state >0){
      //printf("We are in state %d\n", state); -- FOR LOOKING AT NODE STATE
      arrival = GetArrival();
      long state_changes = 0;
      for(k=0; k<state; k++)
      {
        if(arrival > departure[k])
        {
          //our markovian state switches by one. erase the completed job.
          departure[k] = 0.0;
          state_changes += 1;
          index += 1;
        }
      }
      state = state - state_changes;
      //printf("we experienced %d state changes and completed %d jobs\n",state_changes, index); FOR LOOKING AT NODE STATE
      long i;
      //move all jobs that would still be there up to the front of the array
      //put our zeroes to the back
      for(i = 0; i < state_changes; i++)
      {
        double temp = departure[i];
        departure[i] = departure[state_changes+i];
        departure[state_changes+i] = temp;
      }
      //service = GetServiceExponential();
      service = GetServiceUniform();
      if(state == 0)
      {
        departure[state] = arrival + service;
        state += 1;
      }else if(state == capacity){
          //we have a rejection and do not change sate
          reject_count += 1;
      }else if((state > 0) && (state < capacity)){
        departure[state] = departure[state-1] + service;
        state += 1;
      }
    }
  }
        

  printf("\nfor %ld total jobs that approached the system...\n %d jobs were completed\n", index+reject_count, index);
  double prob_leave = (float) reject_count / (float) (reject_count + LAST);
  printf("(A) When the max number of people allowed in line is %d the\
  number of customers that leave to Ben and Jerry's is ... %ld\n\
  The probability that a customer leaves is %f\n", capacity, reject_count, prob_leave);

  //analyitcal probability
  double lambda_mu =  (1.0/60.0) / (1.0/MU) ;
  double probability_leave = (pow(lambda_mu,capacity) * (1 - lambda_mu)) / (1 - pow(lambda_mu, capacity+1));
  printf("analyitcal probability that a customer goes away is... %f \n", probability_leave );


  return (0);
}