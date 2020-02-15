#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include "fileSort.h"


int fileType; //0 = numbers, 1 = letters
int errorNumber;  //this is set to value of errno for displaying later
int* numArray;
char** strArray;

int* readFile(int, char*);
int parseString(char*, char, int);

//will parse a given string by a given delimitor
int parseString(char* str, char delim, int size){
  
  return -1;  
}

//will read in all charaters of a file, parse them by commas, and add them to a linked list
int* readFile(int arguments, char* fileName){
  int bytesToRead = 2000;
  if(arguments <=1){
    printf("ERROR: insuficient arguments.\n");
    return NULL;
  }
  int fd = open(fileName, O_RDONLY);
  errorNumber = errno;
  if(fd<0){
    printf("ERROR: unable to open file[%s]. Errno: %d\n", fileName, errorNumber);
    return NULL;
  }

  char * buffer  = (char*)malloc(sizeof(char)*(bytesToRead +1));
  errorNumber=errno;
  if(buffer==NULL){
    printf("ERROR: unable to malloc.Errno: %d\n", errorNumber);
    return NULL;
  }


  int bytesRead = -2;
  do{
    
    memset(buffer, '\0', (bytesToRead+1));

    bytesRead = 0;
    while(bytesRead < bytesToRead){
      printf("%d\n", bytesRead);
      bytesRead = read(fd, buffer+bytesRead, bytesToRead-bytesRead);
      errorNumber = errno;
      if(bytesRead==0)
	break;

      if(bytesRead<0){
	printf("ERROR: Unable to read bytes from file. Errno: %d", errorNumber);
	return NULL;
      }
    }
    //in place of this print statement, add each token to linked list
    printf("%s\n", buffer);
  }while(bytesRead != 0);

  
  return NULL;
}


int main (int argc, char** argv){

  readFile(argc, argv[1]);


  return 0;
}
