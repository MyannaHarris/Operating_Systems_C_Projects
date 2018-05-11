/*
Class: 346 01
Assignment: assignment 4
Team Leader: Myanna Harris mharris5
9-18-15
Creates two threads
One makes a file full of prime numbers to n
The second counts the characters in the source code
gcc asgn4.c -lpthread
 ./a.out	source_file	prime_file		n
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* create_prime(void*);
int check_prime(int);
void* read_the_files(void*);
void check_args(int);

struct param1
{
  int num;
  char* prime_file;
  void* tid;
};

struct param2
{
  char* source_file;
  char* prime_file;
  void* tid;
};

typedef struct param1 params1;
typedef struct param2 params2;

int main(int argc, char* argv[])
{
  check_args(argc);

  pthread_t t1, t2;
  params1 p1;
  params2 p2;
  int n = atoi(argv[3]);
  int status;

  int tid = pthread_self();

  p1.num = n;
  p1.prime_file = argv[2];
  p1.tid = (void*)tid;
  status = pthread_create(&t1, NULL, create_prime, (void*)&p1);
  if (status != 0)
  {
    printf("Error in thread 1:  %d\n",status);
    exit(-1);
  }

  tid = t1;

  p2.source_file = argv[1];
  p2.prime_file = argv[2];
  p2.tid = (void*)tid;
  status = pthread_create(&t2, NULL, read_the_files, (void*)&p2);
  if (status != 0)
  {
    printf("Error in thread 2:  %d\n",status);
    exit(-1);
  }

  sleep(5);
  printf("Leaving main thread\n");
  pthread_exit(NULL);
  return 0;
}

void check_args(int num)
{
  if(num != 4)
  {
    printf("Needs 3 arguments\n");
    exit(1);
  }
}

void* create_prime(void* params)
{
  // Wait for main to exit

  printf("Hello from thread 1\n");
  printf("Waiting for main thread to exit\n");
  printf("\n");
  params1* p = (params1*)params;
  pthread_t tid = (pthread_t)p->tid;
  pthread_join(tid,NULL);
  printf("Thread 1 received exit signal from main thread\n");
  printf("\n");

  // Write primes to file

  FILE* primef = fopen(p->prime_file, "w");

  int primeN;
  int num = 1;
  int tempNum = 2;
  while(num <= p->num)
  {
    if(check_prime(tempNum) == 1)
    {
      primeN = tempNum;
      num++;
    }
    tempNum++;
  }
  putc(primeN, primef);

  fclose(primef);

  // Exit

  printf("Thread 1 has finished its life work\n");
  printf("Leaving thread 1\n");
  printf("\n");
  pthread_exit(NULL);
}

int check_prime(int num)
{
  int i;
  for(i=2; i<num; i++)
  {
    if(num%i==0)
    {
      return 0;
    }
  }
  return 1;
}

void* read_the_files(void* params)
{
  int numChar = 0;
  int numPrime;
  params2* p = (params2*)params;
  pthread_t tid = (pthread_t)p->tid;

  // Count characters in source code

  printf("Thread 2 will now start counting characters in source code\n");
  printf("\n");

  FILE* sourcef = fopen(p->source_file, "r");

  while(getc(sourcef) != EOF)
  {
    numChar++;
  }

  fclose(sourcef);

  // Wait for first thread to finish

  printf("Hello from thread 2\n");
  printf("Waiting for thread 1 to exit\n");
  printf("\n");
  pthread_join(tid,NULL);
  printf("Thread 2 received exit signal from thread 1\n");
  printf("Thread 2 will now start real work\n");
  printf("\n");

  // Read nth prime number

  FILE* primef = fopen(p->prime_file, "r");
  numPrime = getc(primef);
  fclose(primef);

  printf("Prime number: %i\n", numPrime);
  printf("Number of characters: %i\n", numChar);
  printf("\n");

  // Exit

  printf("Thread 2 has finished its life work\n");
  printf("Leaving thread 2\n");
  printf("\n");
  pthread_exit(NULL);
}
