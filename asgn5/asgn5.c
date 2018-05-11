/*
Class: 346 01
Assignment: assignment 5
Team Leader: Myanna Harris mharris5
9-28-15
Uses Peterson's algorithm to protect shared memory and critical sections
gcc asgn5.c
 ./a.out
 ./a.out	time_parent, time_child, time_parent_non_cs, time_child_non_cs
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>

void parent(int, int, int*, int*, int*);
void child(int, int, int*, int*, int*);
void cs(char, int);
void non_cs(int);

void main(int argc, char* argv)
{
  int timeP = 5;
  int timeC = 5;
  int nonCsP = 5;
  int nonCsC = 5;

 //check for proper arguments

  if(argc != 1 && argc != 5)
  {
    printf("Needs 0 or 4 arguments\n");
    exit(1);
  }

  if(argc == 4)
  {
    timeP = argv[1];
    timeC = argv[2];
    nonCsP = argv[3];
    nonCsC = argv[4];
  }

  int*   turn;
  int*   proc0;
  int*   proc1;
  int    shmid1, shmid2, shmid3;

 //create a shared memory segment
  shmid1 = shmget(0,1,0777 | IPC_CREAT);
  shmid2 = shmget(1,1,0777 | IPC_CREAT);
  shmid3 = shmget(2,1,0777 | IPC_CREAT);

 //attach it to the process, cast its address, and 
  turn = (int*)shmat(shmid1,0,0); 
  proc0 = (int*)shmat(shmid2,0,0); 
  proc1 = (int*)shmat(shmid3,0,0);

 //fork here
  if (fork() == 0)
    child(timeC, nonCsC, turn, proc0, proc1);
  else 
    parent(timeP, nonCsP, turn, proc0, proc1);
}

void parent(int timeP, int nonCsP, int* turn, int* proc0, int* proc1)
{
 int i;
 for (i = 0; i < 10; i++)
  {
   *proc0 = 1;
   *turn = 0;

   //protect this
   while(*turn == 0 && *proc1 == 1)
   {
   }
   cs('p', timeP);
   *proc0 = 0;
   non_cs(nonCsP); 
  }
}

void child(int timeC, int nonCsC, int* turn, int* proc0, int* proc1)
{
 int i;
 for (i = 0; i < 10; i++)
  {
   *proc1 = 1;
   *turn = 1;

   //protect this
   while(*turn == 1 && *proc0 == 1)
   {
   }
   cs('c', timeC);
   *proc1 = 0;
   non_cs(nonCsC); 
  }
}

void cs(char process, int time_crit)
{
 if (process == 'p')
  {
   printf("parent in critical sction\n");
   sleep(time_crit);
   printf("parent leaving critical section\n");
  }
 else
  {
   printf("child in critical sction\n");
   sleep(time_crit);
   printf("child leaving critical section\n");
  }
}

void non_cs(int time_non)
{
 sleep(time_non);
}
