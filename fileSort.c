#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include "fileSort.h"

//need to figure out how to combine tokens split when reading in pice of file!!!

char delim = ',';
int arraySize = 0;
int fileType = -1; //0 = numbers, 1 = letters
int errorNumber;  //this is set to value of errno for displaying later
int* numArray;  //for holding an array of numbers
char** strArray;  // for holding an array of numbers

void printArray(void*, int);
int setFileType(node*);
void* makeArray(node*);
int compareChar(void*, void*);
int compareInt(void*, void*);
char* cleanStr(char*);
void printLL(node*);
node* parseString(char* buffer, char delim, node* head, char* brokenToken, int lastCharIsDelim);
node* readFile(int, char*);

//takes array of char* or int, ft=fileType, 1 for char* and 0 for int
void printArray(void* array, int ft){
  if(ft==1){// strings
    char** strArr = (char**)array;
    int i;
    for(i=0; i<arraySize; i++){
      printf("%s\n",strArr[i]);      
    }
  }
  else if(ft == 0){//integers 
    int** strInt = (int**)array;
    int i;
    for(i=0; i<arraySize; i++){
      printf("%d\n",strInt[i]);
    }
  }
  printf("\n");
}

//sets fileType to 1 for letters or 0 for integers. sets and returns -1 on error.
int setFileType(node* head){
  node* ptr = head;
  while(ptr!=NULL){
    if(strlen(ptr->str)==0)
      ptr = ptr->next;
    else{
      if(isdigit(ptr->str[0]>0))
	 fileType = 0;
      else
	fileType = 1;
      break;
    }
  }
    return fileType;
} 
void* makeArray(node* head){
  node* ptr = head;
  node* prev = NULL;
  while(ptr!=NULL){
    arraySize++;
    ptr = ptr->next;
  }
  ptr = head;
  int count = 0;
  if(fileType==1){//letters
    char** array = malloc(sizeof(char*)*arraySize);
    while(ptr!=NULL){
      array[count] = ptr->str;
      prev = ptr;
      ptr = ptr->next;
      free(prev);
      count++;
    }
    return array;
  }
  else{//numbers
    int* array = malloc(sizeof(int)*arraySize);
    while(ptr!=NULL){
      array[count] = atoi(ptr->str);
      prev = ptr;
      ptr = ptr->next;
      free(prev);
      count++;
    }
    return array;
  }
}
//return 1 if A>B, -1 if B>A, and 0 if A=B
int compareChar(void* TOKENA, void* TOKENB){
  char* a = (char*)TOKENA;
  char* b = (char*)TOKENB;
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
int compareInt(void* TOKENA, void* TOKENB){
  int* a = (int*)TOKENA;
  int* b = (int*)TOKENB;
  if(a>b)
    return 1;
  if(b>a)
    return -1;
  return 0;
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
//last error, if last char is a deliminator, it merges the last token with the next token
node*  parseString(char* str, char delim, node* head, char* brokenToken, int lastCharIsDelim){
  int position = 0;
  int lastDelim = 0;//position right after last deliminator. starts at beginning of string
  int isFirstToken = 1;  //1 if token being read in is the first token read in, 0 otherwise

    printf("parsed\n");
  while(str[lastDelim]!= '\0'){  //iterates until reaches end of string passed
    if(str[position]==delim||str[position]=='\0'){
      char* strTemp = malloc((sizeof(char)*(position-lastDelim)+1));
      node* newNode = malloc(sizeof(node));
      strncpy(strTemp, str+lastDelim, position-lastDelim);
      newNode->next = head;
      newNode->str = cleanStr(strTemp);
      head = newNode;
      lastDelim = position+1;
      if(brokenToken!=NULL && isFirstToken==1 && lastCharIsDelim==1){
	isFirstToken=0;
	char* strTemp2 = malloc(sizeof(char)*(strlen(strTemp)+strlen(brokenToken)));
	strncpy(strTemp2, brokenToken, strlen(brokenToken));
	strncpy(strTemp2+strlen(brokenToken), strTemp, strlen(strTemp));
	free(newNode->str);
	newNode->str = strTemp2;
	printf("Whole str: %s\n", strTemp2);
      }
      lastDelim = position+1;
    }
    position++;  
  }
  return head;
}
//will read in all charaters of a file, parse them by commas, and add them to a linked list
node* readFile(int arguments, char* fileName){
  int bytesToRead = 5;
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
  char * buffer  = (char*)malloc(sizeof(char)*(bytesToRead));
  errorNumber=errno;
  if(buffer==NULL){
    printf("ERROR: unable to malloc.Errno: %d\n", errorNumber);
    return NULL;
  }
  int totalBytesRead = 0; //all bytes read 
  int bytesRead = -2;  //bytes read in one iteration of read()
  char* brokenToken = NULL;
  node* head = NULL;
  int numNodes = 0;
  int lastCharIsDelim = 0;
 do{
    memset(buffer, '\0', (bytesToRead));
    bytesRead = 0;
    totalBytesRead = 0;
    while(bytesRead < bytesToRead){
      bytesRead = read(fd, buffer+totalBytesRead, bytesToRead-totalBytesRead);
      errorNumber = errno;
      totalBytesRead+=bytesRead;
      if(bytesRead==0)
	break;
      if(bytesRead<0){
	printf("ERROR: Unable to read bytes from file. Errno: %d", errorNumber);
	return NULL;
      }
    }
    if(buffer[strlen(buffer)-1]==delim)
      lastCharIsDelim = 1;
    else
      lastCharIsDelim = 0;
    printf("Buffer: %s\n", buffer);
    head =  parseString(buffer, delim, head, brokenToken, lastCharIsDelim);
    //printLL(head);
    printf("bytesRead = %d\n", bytesRead);
    if(bytesRead!=0){
      brokenToken = head->str;
      node* temp = head;
      head = head->next;
       free(temp);
      numNodes--;
      printf("Broken token: %s\n", brokenToken);
    }
    else{
      brokenToken=NULL;
    }
  }while(bytesRead != 0);
  close(fd);
  return head;
}
int main (int argc, char** argv){
  node* head = readFile(argc, argv[1]);
  //printLL(head);
  setFileType(head);
  char** array = (char**) makeArray(head);
  printArray(array, fileType);
  printArray(array, fileType);
  return 0;
}
