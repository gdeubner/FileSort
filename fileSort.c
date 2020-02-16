#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include "fileSort.h"

//need to figure out how to combine tokens split when reading in pice of file!!!

int fileType = 1; //0 = numbers, 1 = letters
int errorNumber;  //this is set to value of errno for displaying later
int* numArray;
char** strArray;

int comparator(void*, void*);
char* cleanStr(char*);
void printLL(node*);
node* parseString(char*, char, node*);
int* readFile(int, char*);


//return 1 if A>B, -1 if B>A, and 0 if A=B
int comparator(void* tokenA, void* tokenB){
  if(fileType==1){
    char* a = (char*)tokenA;
    char* b = (char*)tokenB;
    int position = 0;
    while(a[position]!='\0'&&b[position]!='\0'){
      if((int)a[position]<(int)b[position]){
	return 1;
      }
      else if((int)a[position]>(int)b[position]){
	return -1;
      }
      position++;
    }
    if(a[position]=='\0'&b[position]!='\0')
      return -1;
    if(a[position]!='\0'&&b[position]=='\0')
      return 1;
    return 0;
  }
  else{
    int a = *((int*)tokenA);
    int b = *((int*)tokenB); 
    if(a>b)
      return 1;
    if(a<b)
      return -1;
    return 0;
  }
}



//removes all characters from a string that are not lowercase letters or numbers
char* cleanStr(char* str){
  char* clStr = malloc(sizeof(char)*strlen(str));
  memset(clStr,'\0', strlen(str));
  int i = -1;
  int numCharsInclStr = 0;
  for(i = 0; i<strlen(str); i++ ){
    if(strchr(LEGAL_CHARS, str[i])!=NULL){//current char is a letter or number
      clStr[numCharsInclStr] = str[i];
      numCharsInclStr++;
    }
  }
  return clStr;
}

void printLL(node* head){  //for testing purposes
  node* ptr = head;
  while(ptr!=NULL){
    printf("%s-", ptr->str);
    ptr = ptr->next;
  }
  printf("\n");
}

//will parse a given string by a given delimitor
node* parseString(char* str, char delim, node* head){
  int position = 0;
  int lastDelim = 0;//position right after last deliminator. starts at beginning of string

  while(str[lastDelim]!= '\0'){
    if(str[position]==delim||str[position]=='\0'){
      if(position!=lastDelim){
      char* strTemp = malloc((sizeof(char)*position-lastDelim)+1);
      node* newNode = malloc(sizeof(node));
      strncpy(strTemp, str+lastDelim, position-lastDelim);
      newNode->next = head;
      newNode->str = cleanStr(strTemp);
      head = newNode;
      lastDelim = position+1;
      }
      else
	lastDelim = position+1;
    }
    position++;  
  }
  printLL(head);
  return head;
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
    node* head = NULL;
    //in place of this print statement, add each token to linked list
    printf("%s\n", buffer);
    parseString(buffer, ',', head);


  }while(bytesRead != 0);

  
  return NULL;
}


int main (int argc, char** argv){
  char* a = "hello";
  char* b = "helloo";
  printf("%d\n", comparator(a, b));
  //readFile(argc, argv[1]);


  return 0;
}
