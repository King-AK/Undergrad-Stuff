/* ------------------------------------------------------------------------- 
 *  Adjustment of SSQ4.c base code used to answer problem 4 in the take home final
 *
 *  Problem 4 describes a system with 1 server and is a FINITE HORIZON simulation.
 *  The server is initially idle, and there is a nonstationary Poisson arrival process
 *  Arrival rate is a piecewise-linear spline
 *  Service Time Distribution is Erlang
 *
 * Name            : finalproblem4.c  (Single Server Queue, version 4)
 * Author          : VJ Davey
 * Language        : ANSI C
 * Latest Revision : 11-08-15
 * ------------------------------------------------------------------------- 
 */

#include <stdio.h>
#include <math.h>
#include "rngs.h"                      /* the multi-stream generator */
#include "rvgs.h"                      /* random variate generators  */
#include "rvms.h" //Need this for truncation technique

#define START         0.0              /* initial time                   */
#define STOP       2000.0              /* terminal (close the door) time */
#define REPLICATIONS 1

//global vars for arrival rates, times, slopes, knots, etc.
double arrivalRates[7] = {(0.5),(0.5),(0.9),(0.9), (1.2), (0.9), (0.5)};
double times[7] = {0, 200, 600, 1000, 1200, 1500, 2000};
int knots = 7;
double slopes[6];
double cumRates[7];


   double Min(double a, double c)
/* ------------------------------
 * return the smaller of a, b
 * ------------------------------
 */
{ 
  if (a < c)
    return (a);
  else
    return (c);
} 

   double getCumulativeEventRate(int j)
{
  //returns the cumulative event rate in a recursive fasion
  if(j==0)
    return 0;
  else
    return getCumulativeEventRate(j-1) + (0.5) * (arrivalRates[j] + arrivalRates[j-1]) * (times[j] - times[j-1]);
   

}
   double GetNonStationaryArrival(int rep, int j)
/* ---------------------------------------------
 * generate the next arrival time, using the spline defined in the exam's pdf file
 * along with algorithm 7.5.4, as a nonstationary Poisson proccess. We do not 
 * need algorithm 7.5.4 exactly as it is written since we are only interested in 
 * new arrival times rather than the number of arrivals
 *-------------------
 */ 
{
  static int n = 0; //repition counter
  static double arrival = START;
  static double y = 0;

  SelectStream(0);

  //rep safety counter sanity... reset everything in between repititions
  if(rep > n)
  {
    arrival = START;
    y = START;
    n = rep;
  }
  //Nonstationary Arrival as described on Page 336 of textbook WHY ISNT THIS WORKING?
  y += Exponential(1.0); 
  while( (cumRates[j+1] < y) && (j < knots) ) {
      j++;
  }
  //use proper inverse function described on page 335 of text depending on the slope--double check to verify accuracy
  if(slopes[j] == 0)
  {
    arrival += times[j] + ((y - cumRates[j]) / arrivalRates[j]);
  }else{
    //printf("slope is %f\n",slopes[j]);
    arrival += times[j] + ( (2*(y - cumRates[j])) / (arrivalRates[j] + sqrt(pow(arrivalRates[j], 2) + (2 * slopes[j]*(y - cumRates[j])) )) );
  }
  return (arrival);

} 

   double GetService()
/* --------------------------------------------
 * generate the next service time with rate 1
 * --------------------------------------------
 */ 
{
  SelectStream(1);
  return (Erlang(4, 0.25));
}  



   int main(void)
{
  //GOAL: finite horizon simulation of a M/M/1 server with nonstationary Poisson arrival
  PlantSeeds(0);//call outside replication loop to prevent any overlap between replications
  int n;//replication counter
  double node[REPLICATIONS][7] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0}; //array meant to hold transient statistics regarding number in the node
  
  //spline set up
  int i =0;
  for(i = 0; i < knots-1; i++)
  {
    slopes[i] = (arrivalRates[i+1] - arrivalRates[i] ) / (times[i+1] - times[i]);
  }

  for(i = 0; i < knots; i++)
  {
    cumRates[i] = getCumulativeEventRate(i);
  }


  /*
    we can share a structure between the replications as long as we clear it out 
    between replications
  */
  struct {
      double arrival;                 /* next arrival time                   */
      double completion;              /* next completion time                */
      double current;                 /* current time                        */
      double next;                    /* next (most imminent) event time     */
      double last;                    /* last arrival time                   */
  } t;
  struct {
      double node;                    /* time integrated number in the node  */
      double queue;                   /* time integrated number in the queue */
      double service;                 /* time integrated number in service   */
  } area      = {0.0, 0.0, 0.0};
  long index  = 0;                  /* used to count departed jobs         */
  long number = 0;                  /* number in the node                  */
  double sum_node = 0.0;
  int j;
  
  //file IO for estimate usage
  FILE *a = fopen("knot1.txt", "w");
  FILE *b = fopen("knot2.txt", "w");
  FILE *c = fopen("knot3.txt", "w");
  FILE *d = fopen("knot4.txt", "w");
  FILE *e = fopen("knot5.txt", "w");
  FILE *f = fopen("knot6.txt", "w");

  //MAIN REPLICATION LOOP
  for(n = 0; n < REPLICATIONS; n++)
  {
    index  = 0;                  /* used to count departed jobs         */
    number = 0;                  /* number in the node                  */
    //j should keep track of the knot 
    j = 0;
    
    t.current    = START;           /* set the clock                         */
    t.arrival    = GetNonStationaryArrival(n,j);    /* schedule the first arrival            */
    t.completion = INFINITY;            /* we close the door at this time        */

    //blank area values
    area.node =    0.0;
    area.queue =   0.0;
    area.service = 0.0;
    //blank static variables

    //terminal condition is time 2000
    while (t.arrival < STOP) {
      //process either an arrival or a completion
      t.next          = Min(t.arrival, t.completion);  /* next event time   */
      if (number > 0)  {                               /* update integrals  */
        area.node    += (t.next - t.current) * number;
        area.queue   += (t.next - t.current) * (number - 1);
        area.service += (t.next - t.current);
      }
      t.current       = t.next;                    /* advance the clock */

      if((t.current > times[j]))
      {
          node[n][j] = area.node; 
          printf("j is %d\n", j);
      }

      //determine the next event
      if (t.current == t.arrival)  {               /* process an arrival */
        number++;
        t.arrival     = GetNonStationaryArrival(n,j);
        printf("j is %d\t arrival is %6.2f\n", j, t.arrival);
        if (t.arrival > STOP)  {
          t.last      = t.current;
          t.arrival   = INFINITY;
        }
        if (number == 1)
          t.completion = t.current + GetService();
      }
      else 
      {                                        /* process a completion */
        index++;
        number--;
        if (number > 0)
          t.completion = t.current + GetService();
        else
          t.completion = INFINITY;
      }
    }
    fprintf(a,"%f\n", n, (node[n][1] / t.current));
    fprintf(b,"%f\n", n, (node[n][2] / t.current));
    fprintf(c,"%f\n", n, (node[n][3] / t.current));
    fprintf(d,"%f\n", n, (node[n][4] / t.current));
    fprintf(e,"%f\n", n, (node[n][5] / t.current));
    fprintf(f,"%f\n\n", n, (node[n][6] / t.current));
    //output the average in the node each time to a seperate .txt file for use with estimate program


    //need to find average num in node for different time knots
    //consider printing a table 
  }
  fclose(a);
  fclose(b);
  fclose(c);
  fclose(d);
  fclose(e);
  fclose(f);

  return (0);
}
