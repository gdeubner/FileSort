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

void* makeArray(node*);
int comparator(void*, void*);
char* cleanStr(char*);
void printLL(node*);
node* parseString(char*, char, node*, char*);
node* readFile(int, char*);

void* makeArray(node* head){
  node* ptr = head;
  node* prev = NULL;
  int size = 0;
  while(ptr!=NULL){
    size++;
    ptr = ptr->next;
  }
  ptr = head;
  int count = 0;
  if(fileType==1){//letters
    char** array = malloc(sizeof(char*)*size);
    while(ptr!=NULL){
      array[count] = ptr->str;
      prev = ptr;
      ptr = ptr->next;
      free(prev);
    }
    return array;
  }
  else{//numbers
    int* array = malloc(sizeof(int)*size);
    while(ptr!=NULL){
      array[count] = atoi(ptr->str);
      prev = ptr;
      ptr = ptr->next;
      free(prev);
    }
    return array;
  }
}

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
//NEED TO DEAL WITH BROKENtOKENS!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//Need to handle empty first token
//Need to exclude empty last token
node*  parseString(char* str, char delim, node* head, char* brokenToken){
  int position = 0;
  int lastDelim = 0;//position right after last deliminator. starts at beginning of string
  int isFirstToken = 1;  //1 if token being read in is the first token read in, 0 otherwise
  while(str[lastDelim]!= '\0'){
    if(str[position]==delim||str[position]=='\0'){
      if(position!=lastDelim){
	char* strTemp = malloc((sizeof(char)*(position-lastDelim)+1));
	node* newNode = malloc(sizeof(node));
	strncpy(strTemp, str+lastDelim, position-lastDelim);
	newNode->next = head;
	newNode->str = cleanStr(strTemp);
	head = newNode;
	lastDelim = position+1;
	if(brokenToken!=NULL && isFirstToken==1){///////////////////////////////////
	  isFirstToken=0;
	  char* strTemp2 = malloc(sizeof(char)*(strlen(strTemp)+strlen(brokenToken)));
	  strncpy(strTemp2, brokenToken, strlen(brokenToken));
	  strncpy(strTemp2+strlen(brokenToken), strTemp, strlen(strTemp));
	  free(newNode->str);
	  newNode->str = strTemp2;
	}
      }
      else
	lastDelim = position+1;
    }
    position++;  
  }
  return head;
}

//will read in all charaters of a file, parse them by commas, and add them to a linked list
node* readFile(int arguments, char* fileName){
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
  do{
    
    memset(buffer, '\0', (bytesToRead));
    bytesRead = 0;

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

    
    //in place of this print statement, add each token to linked list
    // printf("FILE:%s\n", buffer);
    head =  parseString(buffer, ',', head, brokenToken);
    
    if(bytesRead!=0){
      brokenToken = head->str;
      node* temp = head;
      head = head->next;
      free(temp);
      numNodes--;
    }

  }while(bytesRead != 0);

  printLL(head);
  close(fd);
  return head;
}


int main (int argc, char** argv){
  void* array =  makeArray(readFile(argc, argv[1]));
  

  return 0;
}
