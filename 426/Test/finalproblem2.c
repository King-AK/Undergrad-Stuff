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
#include "rvms.h"                      //Need this for truncation technique

#define START         0.0              /* initial time                   */
#define LAST         1000
#define REPLICATIONS  128
//structure for job items. Holds all the relevant information for each job. 
//based on information given in chapter 5 of textbook 
struct job{
    //data pertinent to each job
    double arrival;
    double service;
    //pointers to the next nodes
    struct job *next;    
} *head = NULL;
  

  void dequeue()
{
  struct job *temp;
  double a;
  double s;
  if(head == NULL)
  {
    printf("Error occured...nothing to dequeue\n");
    exit(1);
  }else{
    temp = head;
    head = temp->next;
    free(temp);
  }
}
//create void functions below for queue manipulation
  void enqueue( double a, double s)
{
  struct job *temp, *p;
  temp =(struct job*)malloc(sizeof(struct job));
  temp->arrival = a;
  temp->service = s;
  //priority magic here -- if our new service time is less than the head's, we got changes to make
  if(head == NULL || (s < head->service))
  {
    temp->next = head;
    head = temp;
  }else{
    p = head;
    while((p->next != NULL) && (p->next->service <= s))
      p = p->next;
    temp->next = p->next;
    p->next = temp;
  }
}
//FIFO queue manipulation, works just as if SSQ4 ran unedited.
  void enqueueFIFO(double a, double s)
{
   struct job *temp, *p;
   temp = (struct job*) malloc(sizeof(struct job));
   temp->arrival = a;
   temp->service = s;
   if(head==NULL){
      temp->next = NULL;
      head = temp;
   }else{
    p = head;
    while(p->next != NULL)
      p = p->next;
    temp->next = NULL;
    p->next = temp;
   }
}


  void blankList()
  {
    while(head!=NULL)
      dequeue();
  }

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
  //open 3 different files to keep track of statistics
  FILE *lbar = fopen("lbar.txt", "w");
  FILE *qbar = fopen("qbar.txt", "w");
  FILE *xbar = fopen("xbar.txt", "w");
  PlantSeeds(0);

  for(n=0; n <REPLICATIONS; n++)
  {
    //blank everything between runs
    area.node = 0.0;
    area.queue = 0.0;
    area.service = 0.0;
    index=0;
    number = 0;  
    blankList(); 
    
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
        //everytime we get a new arrival, we have to create a new job item with service and arrival time
        enqueue(t.arrival, GetService());
        //enqueueFIFO(t.arrival,GetService());
        //last arrival time
        t.last        = t.current;
        if (number == 1 && head != NULL) 
          //next completion time would be service of head plus current time
          t.completion = t.current + head->service;
      }
      else 
      {                                        /* process a completion */
        index++;
        number--;
        //dequeue the head node, make the next node the head if available
        dequeue();
        if (number > 0 && head!= NULL)
          t.completion = t.current + head->service;
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

  printf("\nfor the last run on %ld jobs\n", index);
  printf("   average interarrival time = %6.2f\n", t.last / index);
  printf("   average wait ............ = %6.2f\n", area.node / index);
  printf("   average delay ........... = %6.2f\n", area.queue / index);
  printf("   average service time .... = %6.2f\n", area.service / index);
  printf("   average # in the node ... = %6.2f\n", area.node / t.current);
  printf("   average # in the queue .. = %6.2f\n", area.queue / t.current);
  printf("   utilization ............. = %6.2f\n", area.service / t.current);

  return (0);
}
