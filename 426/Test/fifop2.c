/* ------------------------------------------------------------------------- 
 * Modification of SSQ4.c for the final exam to match the specifications of problem 2
 *
 * Name            : finalproblem2.c  (Single Server Queue, version 4)
 * Author          : VJ Davey
 * Language        : ANSI C
 * Latest Revision : 12-09-15
 * ------------------------------------------------------------------------- 
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "rngs.h"                      /* the multi-stream generator */
#include "rvgs.h"                      /* random variate generators  */
#include "rvms.h" //Need this for truncation technique

#define START         0.0              /* initial time                   */
#define LAST         1000
#define REPLICATIONS  128

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


   double GetArrival(int rep)
/* ---------------------------------------------
 * generate the next arrival time, with rate 10
 * ---------------------------------------------
 */ 
{
  static double arrival = START;
  static int n = 0;
  if(rep > n){
    arrival = START;
    n = rep;
  }
  SelectStream(0); 
  arrival += Exponential(0.1);
  return (arrival);
} 


   double GetService()
/* --------------------------------------------
 * generate the next service time
 * --------------------------------------------
 */ 
{
  SelectStream(1);
  return (Erlang(4, 0.02));
}  


   int main(void)
{
  //time management structure
  struct {
    double arrival;                 /* next arrival time                   */
    double completion;              /* next completion time                */
    double current;                 /* current time                        */
    double next;                    /* next (most imminent) event time     */
    double last;                    /* last arrival time                   */
  } t;

  //statistics gathering structure
  struct {
    double node;                    /* time integrated number in the node  */
    double queue;                   /* time integrated number in the queue */
    double service;                 /* time integrated number in service   */
  } area;
  long index ;                  /* used to count departed jobs         */
  long number;                  /* number in the node                  */
  int n;
  //open 3 different files
  FILE *lbar = fopen("lbar.txt", "w");
  FILE *qbar = fopen("qbar.txt", "w");
  FILE *xbar = fopen("xbar.txt", "w");
  PlantSeeds(0);

  for(n=0; n <REPLICATIONS; n++)
  {
    //blank area, index, and number
    area.node = 0.0;
    area.queue = 0.0;
    area.service = 0.0;
    index=0;
    number = 0;  
    
    t.current    = START;           /* set the clock                         */
    t.arrival    = GetArrival(n);    /* schedule the first arrival            */
    t.completion = INFINITY;        /* the first event can't be a completion */
    
    while (index <  LAST) {
      t.next          = Min(t.arrival, t.completion);  /* next event time   */
      if (number > 0)  {                               /* update integrals  */
        area.node    += (t.next - t.current) * number;
        area.queue   += (t.next - t.current) * (number - 1);
        area.service += (t.next - t.current);
      }
      t.current       = t.next;                    /* advance the clock */

      if (t.current == t.arrival)  {               /* process an arrival */
        number++;
        t.arrival     = GetArrival(n);
        //last arrival time
        t.last        = t.current;
        if (number == 1) 
          //next completion time would be service of head plus current time
          t.completion = t.current + GetService();
      }
      else 
      {                                        /* process a completion */
        index++;
        number--;
        //dequeue the head node, make the next node the head if available
        if (number > 0)
          t.completion = t.current + GetService();
        else
          t.completion = INFINITY;
      }
    } 
    fprintf(lbar, "%f\n", (area.node / t.current));
    fprintf(qbar, "%f\n", (area.queue / t.current));
    fprintf(xbar, "%f\n", (area.service / t.current));
  }
  fclose(lbar);
  fclose(qbar);
  fclose(xbar);

  printf("\nfor %d runs on %ld jobs\n", REPLICATIONS, index);
  printf("   average interarrival time = %6.2f\n", t.last / index);
  printf("   average wait ............ = %6.2f\n", area.node / index);
  printf("   average delay ........... = %6.2f\n", area.queue / index);
  printf("   average service time .... = %6.2f\n", area.service / index);
  printf("   average # in the node ... = %6.2f\n", area.node / t.current);
  printf("   average # in the queue .. = %6.2f\n", area.queue / t.current);
  printf("   utilization ............. = %6.2f\n", area.service / t.current);

  return (0);
}
