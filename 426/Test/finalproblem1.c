/* ------------------------------------------------------------------------- 
 *  Adjustment of SSQ4.c base code used to answer problem 1 in the take home final
 *
 *  Problem 1 describes a system with 4 servers, each with infinite capacity. Server 1
 * feeds either into servers 2 or 3. Servers 2 and 3 either release jobs from the system or feed
 * them to server 4. Server 4 can only finish jobs and release them from the system.
 *
 *
 *
 *
 * Name            : finalproblem1.c  (Single Server Queue, version 4)
 * Author          : VJ Davey
 * Language        : ANSI C
 * Latest Revision : 11-27-15
 * ------------------------------------------------------------------------- 
 */

#include <stdio.h>
#include <math.h>
#include "rngs.h"                      /* the multi-stream generator */
#include "rvgs.h"                      /* random variate generators  */
#include "rvms.h"                      //Need this for truncation technique

#define START         0.0            
#define LAST        99999           
#define REPLICATIONS  1

//area structures for node1, node2, node3, node4, and the entire system
struct {
    double node;                    /* time integrated number in the node  */
    double queue;                   /* time integrated number in the queue */
    double service;                 /* time integrated number in service   */
    long number;        //number is not time integrated. it is a variable record of number in the node
    long completions;   //number of total job completions for the node
} area_Node1,area_Node2,area_Node3,area_Node4,area_SYSTEM;



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
  double NextEvent(double a, double b, double c, double d, double e, double f, 
    double g, double h)
{
  return Min(Min(Min(Min(Min(Min(Min(a,b),c),d),e),f),g),h);

}

   double GetArrivalNode1(int rep)
/* ---------------------------------------------
 * generate the next arrival time, with rate 4.0
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
  arrival += Exponential(0.25);
  return (arrival);
} 
//Arrivals for all other nodes are the result of the throughput from the node preceeding it in the diagram
   double GetServiceNode1()
/* --------------------------------------------
 * generate the next service time with rate 1/0.16
 * --------------------------------------------
 */ 
{
  SelectStream(1);
  return (Erlang(4, 0.04));
}  
   double GetServiceNode2()
/* --------------------------------------------
 * generate the next service time with rate 1/0.3
 * --------------------------------------------
 */ 
{
  SelectStream(2);
  return (Erlang(6, 0.05));
}  
   double GetServiceNode3()
/* --------------------------------------------
 * generate the next service time with rate 1/0.5
 * --------------------------------------------
 */ 
{
  SelectStream(3);
  return (Erlang(5, 0.10));
}  
   double GetServiceNode4()
/* --------------------------------------------
 * generate the next service time with rate 1/0.24
 * --------------------------------------------
 */ 
{
  SelectStream(4);
  return (Erlang(3, 0.08));
}  

  double GetFeedback( double beta )
{
  SelectStream(7);
  if(Random() < beta)
        return(1);
  else
        return(0);

}



   int main(void)
{
  //system variables and time structure
  long index;                 /*number of jobs that have gone through system*/
  int n;
  struct {

    //------------------------
    //LIST OF POSSIBLE EVENTS. We can have either an arrival or a completion at each node...
    //------------------------
    double arrival_Node1;                 /* next arrival time                   */
    double completion_Node1;              /* next completion time                */
    double last_Node1;                    /* last arrival time                   */
    
    double arrival_Node2;                 /* next arrival time                   */
    double completion_Node2;              /* next completion time                */
    double last_Node2;                    /* last arrival time                   */
    
    double arrival_Node3;                 /* next arrival time                   */
    double completion_Node3;              /* next completion time                */
    double last_Node3;                    /* last arrival time                   */
    
    double arrival_Node4;                 /* next arrival time                   */
    double completion_Node4;              /* next completion time                */ 
    double last_Node4;                    /* last arrival time                   */   

    //----------------------
    // Other important magical time variables...
    //----------------------
    double current;                 /* current time                        */
    double next;                    /* next (most imminent) event time     */
  } t;

  //-------------------------------------------
  //-------------------------------------------
  //--------------------------------------------
 
  PlantSeeds(0);
  //file IO for estimate usage
  FILE *f = fopen("systemwait.txt", "w");
  for(n = 0; n < REPLICATIONS; n++)
  {
    //from run to run, blank all the EVERYTHING
    index = 0;
    area_Node1.node = 0.0;
    area_Node1.queue = 0.0;
    area_Node1.service = 0.0;
    area_Node1.number = 0.0;
    area_Node1.completions = 0.0;
    area_Node2.node = 0.0;
    area_Node2.queue = 0.0;
    area_Node2.service = 0.0;
    area_Node2.number = 0.0;
    area_Node2.completions = 0.0;
    area_Node3.node = 0.0;
    area_Node3.queue = 0.0;
    area_Node3.service = 0.0;
    area_Node3.number = 0.0;
    area_Node3.completions = 0.0;
    area_Node4.node = 0.0;
    area_Node4.queue = 0.0;
    area_Node4.service = 0.0;
    area_Node4.number = 0.0;
    area_Node4.completions = 0.0;
    area_SYSTEM.node = 0.0;
    area_SYSTEM.queue = 0.0;
    area_SYSTEM.service = 0.0;
    area_SYSTEM.number = 0.0;
    area_SYSTEM.completions = 0.0;

    t.current    = START;           /* set the clock                         */
    t.arrival_Node1    = GetArrivalNode1(n);    /* schedule the first arrival to Node 1   */

    //set everything else to infinity to begin with for out NE simulation...
    t.completion_Node1 = INFINITY;        
    t.arrival_Node2    = INFINITY;
    t.completion_Node2 = INFINITY;
    t.arrival_Node3    = INFINITY;
    t.completion_Node3 = INFINITY;
    t.arrival_Node4    = INFINITY;
    t.completion_Node4 = INFINITY;

  //This while loop simulates the whole system through the completion of a lot of jobs  
  //number of jobs completed is our terminal condition
    while (index < LAST) 
    {
      //---------------------------------------------------------------------
      //NEXT EVENT CLOCK MAGIC
      //---------------------------------------------------------------------
      //find minimum next event time for all possible options (arrivals and completions at each node)
      t.next          = NextEvent(t.arrival_Node1,t.completion_Node1,t.arrival_Node2,t.completion_Node2,
        t.arrival_Node3, t.completion_Node3, t.arrival_Node4, t.completion_Node4);

      
      if (area_Node1.number > 0)  {                               // update integrals  
        area_Node1.node    += (t.next - t.current) * area_Node1.number;
        area_Node1.queue   += (t.next - t.current) * (area_Node1.number - 1);
        area_Node1.service += (t.next - t.current);
      }
      if (area_Node2.number > 0)  {                               // update integrals  
        area_Node2.node    += (t.next - t.current) * area_Node2.number;
        area_Node2.queue   += (t.next - t.current) * (area_Node2.number - 1);
        area_Node2.service += (t.next - t.current);
      }
      if (area_Node3.number > 0)  {                               // update integrals  
        area_Node3.node    += (t.next - t.current) * area_Node3.number;
        area_Node3.queue   += (t.next - t.current) * (area_Node3.number - 1);
        area_Node3.service += (t.next - t.current);
      }
      if (area_Node4.number > 0)  {                               // update integrals  
        area_Node4.node    += (t.next - t.current) * area_Node4.number;
        area_Node4.queue   += (t.next - t.current) * (area_Node4.number - 1);
        area_Node4.service += (t.next - t.current);
      }
      //SYSTEM STATS
      if (area_SYSTEM.number > 0)  {                               // update integrals  
        area_SYSTEM.node    += (t.next - t.current) * area_SYSTEM.number;
        area_SYSTEM.queue   += (t.next - t.current) * (area_SYSTEM.number - 1);
        area_SYSTEM.service += (t.next - t.current);
      }

      t.current       = t.next;                    /* advance the clock */

      //--------------------------------------
      // FINDING OUT NEXT EVENTS... a bunch of else ifs to follow....
      //--------------------------------------

   //-----------------NODE 1-------------------------------------
      //ARRIVAL 
      if (t.current == t.arrival_Node1)  {  
        
        area_Node1.number++; //increase the number at node 1
        area_SYSTEM.number++;//the number in the system has increased

        t.arrival_Node1   = GetArrivalNode1(n);//generate the next arrival time
        t.last_Node1 = t.current;

        if (area_Node1.number == 1)//If a job exists at this node, we must generate a service time for it
          t.completion_Node1 = t.current + GetServiceNode1();//calculate completion of job at node 1
      }

      //COMPLETION
      else if(t.current == t.completion_Node1) {
        //go to Node 2 with Pr == 0.6
        if (GetFeedback(0.6) == 1)
          t.arrival_Node2 = t.completion_Node1;
        else//job is fed to node 3 with Pr ==0.4
          t.arrival_Node3 = t.completion_Node1;

        area_Node1.number--;//decrease number at node 1 by 1
        area_Node1.completions++;
        if (area_Node1.number > 0)//if we had other jobs in the queue at this node, calculate a new completion time for the next job up
          t.completion_Node1 = t.current + GetServiceNode1();
        else//set to infinity so we dont end up in inifinite loop
          t.completion_Node1 = INFINITY;
      }
   //-----------------NODE 2-------------------------------------
      //ARRIVAL. Note: only node 1 can generate next arrivals. Other nodes are dependent on one another.
      else if (t.current == t.arrival_Node2)  {
        area_Node2.number++; 

        t.last_Node2 = t.current;
        t.arrival_Node2 = INFINITY;//clear out Node 2's arrival. A completion at Node1 will replace it as needed
        
        if (area_Node2.number == 1)
          t.completion_Node2 = t.current + GetServiceNode2();
      }
      //COMPLETION
      else if(t.current == t.completion_Node2) {
        //exit system with Pr == 0.2
        if (GetFeedback(0.2) == 1)
        {
          index++;//a job has been completed
          area_SYSTEM.number--;
        }else{//job is fed to node 4 with Pr ==0.8
          t.arrival_Node4 = t.completion_Node2;
        }

        area_Node2.number--;
        area_Node2.completions++;
        if (area_Node2.number > 0)
          t.completion_Node2 = t.current + GetServiceNode2();
        else
          t.completion_Node2 = INFINITY;
      }
   //-----------------NODE 3-------------------------------------
      //ARRIVAL
      else if (t.current == t.arrival_Node3)  {
        area_Node3.number++; 
   
        t.last_Node3 = t.current;
        t.arrival_Node3 = INFINITY;
        if (area_Node3.number == 1)
          t.completion_Node3 = t.current + GetServiceNode3();
      }
      //COMPLETION
      else if(t.current == t.completion_Node3) {
        //exit systemwith Pr == 0.3
        if (GetFeedback(0.3) == 1)
        {
          index++;
          area_SYSTEM.number--;
        }else{//job is fed to node 4 with Pr ==0.7
          t.arrival_Node4 = t.completion_Node3;
        }

        area_Node3.number--;
        area_Node3.completions++;
        if (area_Node3.number > 0)
          t.completion_Node3 = t.current + GetServiceNode3();
        else
          t.completion_Node3 = INFINITY;
      }
   //-----------------NODE 4-------------------------------------
      //ARRIVAL
      else if (t.current == t.arrival_Node4)  {

        area_Node4.number++; 
        t.last_Node4 = t.current;
        t.arrival_Node4 = INFINITY;
        if (area_Node4.number == 1)
          t.completion_Node4 = t.current + GetServiceNode4();
      }
      //COMPLETION
      else if(t.current == t.completion_Node4) {

        index++;//a completion at node 4 always exits the system
        area_SYSTEM.number--;
        area_Node4.number--;
        area_Node4.completions++;
        if (area_Node4.number > 0)
          t.completion_Node4 = t.current + GetServiceNode4();
        else
          t.completion_Node4 = INFINITY;
      }

    //---------------Error handling------------------------------
      else{
        printf("Error occured...pls troubleshoot\n");
        break;
      }
    }
    fprintf(f, "%f\n", (area_SYSTEM.node / index));
  }
      
  fclose(f);
  //print terminal statistics for the last run. an external file will hold data for many runs
  printf("\nOn the LAST RUN... for %ld jobs\n", index);
  
  printf("   utilization of system............. = %6.2f\n", area_SYSTEM.service / t.current);
  printf("   utilization at Node 1............. = %6.2f\n", area_Node1.service / t.current);
  printf("   utilization at Node 2............. = %6.2f\n", area_Node2.service / t.current);
  printf("   utilization at Node 3............. = %6.2f\n", area_Node3.service / t.current);
  printf("   utilization at Node 4............. = %6.2f\n\n", area_Node4.service / t.current);

  printf("   average # in the system .......... = %6.2f\n",area_SYSTEM.node / t.current);
  printf("   average # in the node at Node 1... = %6.2f\n", area_Node1.node / t.current);
  printf("   average # in the node at Node 2... = %6.2f\n", area_Node2.node / t.current);
  printf("   average # in the node at Node 3... = %6.2f\n", area_Node3.node / t.current);
  printf("   average # in the node at Node 4... = %6.2f\n\n", area_Node4.node / t.current);

  printf("   average wait in the system........ = %6.2f\n",area_SYSTEM.node / index);
  printf("   average wait at Node 1............ = %6.2f\n", area_Node1.node / area_Node1.completions);
  printf("   average wait at Node 2............ = %6.2f\n", area_Node2.node / area_Node2.completions);
  printf("   average wait at Node 3............ = %6.2f\n", area_Node3.node / area_Node3.completions);
  printf("   average wait at Node 4............ = %6.2f\n\n", area_Node4.node / area_Node4.completions);
 
   //Used for verification of hand calculations
  printf("   average interarrival time at Node 1 = %6.2f\n", t.last_Node1 / area_Node1.completions);
  printf("   average interarrival time at Node 2 = %6.2f\n", t.last_Node2 / area_Node2.completions);
  printf("   average interarrival time at Node 3 = %6.2f\n", t.last_Node3 / area_Node3.completions);
  printf("   average interarrival time at Node 4 = %6.2f\n", t.last_Node4 / area_Node4.completions);
  
  return (0);
}

