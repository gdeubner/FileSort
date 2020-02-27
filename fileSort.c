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
int insertionSort(void*,int (*comparator)(void*,void*));
int quickSort(void*,int (*comparator)(void*,void*));
void split(void*,int,int,int (*comparator)(void*,void*));

//recursive function that quicksorts given array
void split(void* arr,int lo,int hi,int(*comparator)(void*,void*)){
  if(fileType==1){
    char** array=(char**)arr;
    int i;
    int j;
    char* temp;
    int pivot;
    if(lo<hi){
    i=lo;
    j=hi;
    pivot=lo; //first element is pivot
    while(i<j){
      while(comparator(array[i],array[pivot])==1&&i<hi){ //if array[i] comes before array[j] alphabetically
	i++;
      }
      while(comparator(array[j],array[pivot])<1&&j>lo){ //if array[i] comes after array[j] alphabetically
	j--;
      }
      if(i<j){ //swaps hi and lo index elements
	  temp=array[i];
	  array[i]=array[j];
          array[j]=temp;
        }
      }
    temp=array[pivot]; //putting pivot in right place
      array[pivot]=array[j];
      array[j]=temp;
      split(array,lo,j-1,comparator);
      split(array,j+1,hi,comparator);
      return;
     }
  } else{
    int** array=(int**)arr;
    int i;
    int j;
    int* temp;
    int pivot;
    if(lo<hi){
    i=lo;
    j=hi;
    pivot=lo;
    while(i<j){
      while(comparator(array[i],array[pivot])<1&&i<hi){ //if array[i] comes before array[j]
	i++;
      }
      while(comparator(array[j],array[pivot])==1&&j>lo){ //if array[i] comes after array[j]
	j--;
      }
      if(i<j){ //swaps hi and lo index elements
	  temp=array[i];
	  array[i]=array[j];
          array[j]=temp;
        }
      }
      temp=array[pivot]; //putting pivot in right place
      array[pivot]=array[j];
      array[j]=temp;
      split(array,lo,j-1,comparator);
      split(array,j+1,hi,comparator);
      return;
     }
  }
 }
//"Shell" for quick sorting
int quickSort(void* toSort,int (*comparator)(void*,void*)){
  if(fileType==1){ //if filetype is strings
  split(toSort,0,arraySize-1,comparator);
  printArray(toSort,fileType);
  } else { //if filetype is ints
    int**array=(int**)toSort;
    //int** numArray=malloc(sizeof(int*)*arraySize);
    //int i;
    //for(i=0;i<arraySize;i++){
    // numArray[i]=atoi(array[i]);
    //}
    split(array,0,arraySize-1,comparator);
    printArray(array,fileType);
  }
  return 1;
}
int insertionSort(void* toSort,int (*comparator)(void*,void*)){
  int i,j;
  if(fileType==0){//if filetype is ints
    //char** sarray=(char**)toSort;
    //int** array=malloc(sizeof(int*)*arraySize);
    //for(i=0;i<arraySize;i++){
    //array[i]=atoi(sarray[i]); //creates proper integer array
    //}
    int** array=(int**)toSort;
    for(i=1;i<arraySize;i++){
      int* cmp=array[i];
	j=i-1;
	while(j>=0&&comparator(array[j],cmp)==1){//while element at index j is greater than element we are inserting
	  array[j+1]=array[j];//moves element at index j ahead
	  j--;
	}
	array[j+1]=cmp;
    }
    printArray(array,fileType);
  } else{
    char** array=malloc(sizeof(char*)*arraySize);
    array=(char**)toSort;
    for(i=1;i<arraySize;i++){
      char* cmp=array[i];
      j=i-1;
      while(j>=0&&comparator(array[j],cmp)<1){ //while element at index j is alphabetically ahead element we are inserting
	array[j+1]=array[j]; //moves element at index j ahead one
	j--;
      }
      array[j+1]=cmp;
    }
    printArray(array,fileType);
  }
  return 1;
}
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
      if(isdigit(ptr->str[0])>0||ptr->str[0]=='-')
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
  if(fileType==1){
    char** array = malloc(sizeof(char*)*arraySize);
    while(ptr!=NULL){
      array[count]=ptr->str;
      prev = ptr;
      ptr = ptr->next;
      free(prev);
      count++;
    }
    return array;
  } else{
    int** array=malloc(sizeof(int*)*arraySize);
    while(ptr!=NULL){
      long temp=atoi(ptr->str);
      array[count]=(int*)temp;
      prev=ptr;
      ptr=ptr->next;
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
  long a = (long)TOKENA;
  long b = (long)TOKENB;
  if(a<0&&b>=0)
    return -1;
  if(b<0&&a>=0)
    return 1;
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
    head =  parseString(buffer, delim, head, brokenToken, lastCharIsDelim);
    if(bytesRead!=0){
      brokenToken = head->str;
      node* temp = head;
      head = head->next;
       free(temp);
      numNodes--;
    }
    else{
      brokenToken=NULL;
    }
  }while(bytesRead != 0);
  close(fd);
  return head;
}
int main (int argc, char** argv){

  if(argc!=3||argv[1][0]!='-'){
    printf("Fatal Error: Please enter two properly formatted commands\n");
    return 0;
  }
  char command=argv[1][1];
  if(command!='q'&&command!='i'){
    printf("Fatal Error: Please enter two properly formatted commands\n");
    return 0;
  }
  node* head = readFile(argc, argv[2]);
  //printLL(head);
  setFileType(head);
  void* array=makeArray(head);
  if(fileType==1){
    if(command=='q'){
    quickSort(array,compareChar);
    }else{
    insertionSort(array,compareChar);
    
    }
  }
  if(fileType==0)
    if(command=='q'){
    quickSort(array,compareInt);
    }else{
    insertionSort(array,compareInt);
    }
  return 0;
}
