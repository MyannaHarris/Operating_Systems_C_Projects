/*
CLass: CPSC 346 01
Assignment: assignment 3
Team Leader: Myanna Harris
9-12-15
Takes 2 file names as arguments. 
Reads the first one backwards into the second one, character by charatcer.
*/

#include<stdio.h>
#include<stdlib.h>

void check_args(int argc);
FILE* my_open(char* fileName, char* mode);
int isEmpty(FILE* checkFile);
void make_file(char* fileName);

int main(int argc, char* argv[])
{
 check_args(argc);
 FILE* firstFile = my_open(argv[1], "r");

 int empty = 0;
 if (firstFile != NULL)
 {
  empty = isEmpty(firstFile);
 }
 if (firstFile == NULL || (empty == 1))
 {
  if(empty == 1)
  {
   fclose(firstFile);
  }
  make_file(argv[1]);
  firstFile = fopen(argv[1], "r");
 }

 FILE* secondFile = my_open(argv[2], "w");

 fseek(firstFile, 0, SEEK_END);

 while (ftell(firstFile) > 0)
 {
  fseek(firstFile, -1, SEEK_CUR);
  char ch = getc(firstFile);
  putc(ch, secondFile);
  fseek(firstFile, -1, SEEK_CUR);
 }

 fclose(secondFile); 
 fclose(firstFile); 
}

//Check that there are 2 arguments
void check_args(int argc)
{
 if (argc != 3)
 {
  printf("There must be two command line arguments\n");
  exit(1);
 } 
}

//Check that the arguments are openable files
FILE* my_open(char* fileName, char* mode)
{
 FILE* tempFile;
 if((tempFile = fopen(fileName, mode)) == NULL)
 {
  return NULL;
 }
 return tempFile;
}

int isEmpty(FILE* checkFile)
{
 fseek(checkFile, 0, SEEK_END);
 if (ftell(checkFile) == 0)
 {
  return 1;
 }
 rewind(checkFile);
 return 0;
}

void make_file(char* fileName)
{
 FILE* tempFile = fopen(fileName, "w");
 putc('a', tempFile);
 putc('b', tempFile);
 putc('\n', tempFile);
 putc('c', tempFile);
 putc('d', tempFile);
 fclose(tempFile);
}
