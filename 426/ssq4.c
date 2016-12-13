/* ------------------------------------------------------------------------- 
 * This program is a next-event simulation of a single-server FIFO service
 * node using Exponentially distributed interarrival times and Erlang 
 * distributed service times (i.e., a M/E/1 queue).  The service node is 
 * assumed to be initially idle, no arrivals are permitted after the 
 * terminal time STOP, and the node is then purged by processing any 
 * remaining jobs in the service node.
 *
 * Name            : ssq4.c  (Single Server Queue, version 4)
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
#define STOP      20000.0              /* terminal (close the door) time */
#define INFINITY_SSQ4   (100.0 * STOP)      /* must be much larger than STOP  */
#define MIN_SERVICE_TIME   0.0 //Minimum service time for use with truncation or acceptance rejection
#define MAX_SERVICE_TIME   3.0 //Maximum service time for use with truncation or acceptance rejection

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


   double GetArrival()
/* ---------------------------------------------
 * generate the next arrival time, with rate 1/2
 * ---------------------------------------------
 */ 
{
  static double arrival = START;

  SelectStream(0); 
  arrival += Exponential(2.0);
  return (arrival);
} 


   double GetService()
/* --------------------------------------------
 * generate the next service time with rate 2/3
 * --------------------------------------------
 */ 
{
  SelectStream(1);
  return (Erlang(5, 0.3));
}  

   double GetServiceViaAcceptanceRejection()
/* --------------------------------------------
 *  Created with technique described on PG 258 of textbook
 * --------------------------------------------
 */ 
{
  SelectStream(1);
  double d = Erlang(5, 0.3);
  while((d < MIN_SERVICE_TIME) || (d > MAX_SERVICE_TIME))
    d = Erlang(5, 0.3);
  return d;
}

   double GetServiceViaCosntrainedInversion()
/* --------------------------------------------
 *  Created with technique described on PG 311/312 of textbook
 * --------------------------------------------
 */ 
{
  SelectStream(1);
  //Find left and right tail truncation probabilities
  double alpha = cdfErlang(5, 0.3, MIN_SERVICE_TIME);
  double beta  = 1.0 - cdfErlang(5, 0.3, MAX_SERVICE_TIME);
  double u = Uniform(alpha, 1.0 - beta);
  return idfErlang(5, 0.3, u);
}



   int main(void)
{
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
  
  
  PlantSeeds(0);
  t.current    = START;           /* set the clock                         */
  t.arrival    = GetArrival();    /* schedule the first arrival            */
  t.completion = INFINITY_SSQ4;        /* the first event can't be a completion */

  
  while ((t.arrival < STOP) || (number > 0)) {
    t.next          = Min(t.arrival, t.completion);  /* next event time   */
    if (number > 0)  {                               /* update integrals  */
      area.node    += (t.next - t.current) * number;
      area.queue   += (t.next - t.current) * (number - 1);
      area.service += (t.next - t.current);
    }
    t.current       = t.next;                    /* advance the clock */

    if (t.current == t.arrival)  {               /* process an arrival */
      number++;
      t.arrival     = GetArrival();
      if (t.arrival > STOP)  {
        t.last      = t.current;
        t.arrival   = INFINITY_SSQ4;
      }
      if (number == 1)
        t.completion = t.current + GetService();
        //t.completion = t.current + GetServiceViaAcceptanceRejection();
        //t.completion = t.current + GetServiceViaCosntrainedInversion();
    }

    else {                                        /* process a completion */
      index++;
      number--;
      if (number > 0)
        t.completion = t.current + GetService();
        //t.completion = t.current + GetServiceViaAcceptanceRejection();
        //t.completion = t.current + GetServiceViaCosntrainedInversion();
      else
        t.completion = INFINITY_SSQ4;
    }
  } 

  printf("\nfor %ld jobs\n", index);
  printf("   average interarrival time = %6.2f\n", t.last / index);
  printf("   average wait ............ = %6.2f\n", area.node / index);
  printf("   average delay ........... = %6.2f\n", area.queue / index);
  printf("   average service time .... = %6.2f\n", area.service / index);
  printf("   average # in the node ... = %6.2f\n", area.node / t.current);
  printf("   average # in the queue .. = %6.2f\n", area.queue / t.current);
  printf("   utilization ............. = %6.2f\n", area.service / t.current);

  return (0);
}
