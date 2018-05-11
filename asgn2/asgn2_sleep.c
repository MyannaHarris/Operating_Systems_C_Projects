#include <stdio.h>
int main(int argc, char* argv[])
{
 if(argc == 3)
 {
  while(1)
  {
   printf("%s\n", argv[1]);
   sleep(atoi(argv[2]));
  }
 }
 else
 {
  printf("Need 2 arguments: string int\n");
 }
}
