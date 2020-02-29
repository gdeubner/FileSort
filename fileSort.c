#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include "fileSort.h"

char delim = ',';
int arraySize = 0;
int fileType = -1; //0 = numbers, 1 = letters
int errorNumber;  //this is set to value of errno for displaying later
int* numArray;  //for holding an array of numbers
char** strArray;  // for holding an array of chars


void sortFile(char command, void* array, int fileType){
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
  free(array);
}
int errorCheck(int argc, char** argv){
  if(argc!=3||argv[1][0]!='-'){
    printf("Fatal Error: Please enter two properly formatted commands\n");
    exit(0);
  }
  if(argv[1][1]!='q'&&argv[1][1]!='i'){
    printf("Fatal Error: Please enter two properly formatted commands\n");
    exit(0);
  }
  return 0;
}

//recursive function that quicksorts given array
void split(void* arr,int lo,int hi,int(*comparator)(void*,void*)){
  if(fileType==1){
    char** array=(char**)arr;
    int i, j, pivot;
    char* temp;
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
    int i, j, pivot;
    int* temp;
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
    split(array,0,arraySize-1,comparator);
    printArray(array,fileType);
  }
  return 1;
}
int insertionSort(void* toSort,int (*comparator)(void*,void*)){
  int i,j;
  if(fileType==0){//if filetype is ints
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
    //char** array=malloc(sizeof(char*)*arraySize);
    char** array=(char**)toSort;
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
      free(prev->str);
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
      free(prev->str);
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
  free(str);
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
node* parseString(char* str, char delim, node* head){
  int position = 0;
  int lastDelim = 0;//position right after last deliminator. starts at beginning of string
  int isFirstToken = 1;  //1 if token being read in is the first token read in, 0 otherwise
  while(str[lastDelim]!= '\0'){  //iterates until reaches end of string passed
    if(str[position]==delim||str[position]=='\0'){
      node* nodePtr;
      if(head==NULL || head->delimTerm==1){ //first node in head or last node was complete (terminated with delim)
	nodePtr = malloc(sizeof(node));
	char* strTemp = malloc((sizeof(char)*(position-lastDelim)+1));
	strncpy(strTemp, str+lastDelim, position-lastDelim);
	nodePtr->next = head;
	nodePtr->str = cleanStr(strTemp);
	head = nodePtr;
      }else{  //last node in head was incomplete (will add the contents of the next token to the last)
	nodePtr = head;
	char* strTemp = malloc(sizeof(char)*(position-lastDelim + strlen(head->str)));
	strncpy(strTemp, head->str, strlen(head->str));
	strncpy(strTemp+strlen(head->str), str+lastDelim, position-lastDelim);
	free(head->str);
	head->str = strTemp;
      }
      lastDelim = position+1;
      if(str[position]==',')
	nodePtr->delimTerm = 1;
      else
	nodePtr->delimTerm = 0;	
    }
    position++;  
  }
  return head;
}
//will read in all charaters of a file, parse them by commas, and add them to a linked list
node* readFile(int arguments, char* fileName){
  int bytesToRead = 2000;   
  int fd = open(fileName, O_RDONLY);
  errorNumber = errno;
  if(fd<0){
    printf("Fatal Error: File %s does not exist. Errno: %d\n", fileName, errorNumber);
    return NULL;
  }
  char * buffer  = NULL;
  int mallocCount = 1;
  while(buffer==NULL&&mallocCount<4){
    buffer = (char*)malloc(sizeof(char)*(bytesToRead));
    errorNumber=errno;
    if(buffer==NULL){
      printf("Warning: Unable to malloc. Attempt number: %d Errno: %d\n", mallocCount, errorNumber);
      mallocCount++;
    }
  }
  if(mallocCount>=4){
    printf("Fatal Error: Malloc was unsuccessful. Errno: %d\n", errorNumber);
    exit(0);
  }
  int totalBytesRead = 0; //all bytes read 
  int bytesRead = -2;  //bytes read in one iteration of read()
  node* head = NULL;
  int numNodes = 0;
  int sizeOfFile=0;
  do{   //reads set num of bytes, converts bytes to a LL and repeats until end of file
    memset(buffer, '\0', (bytesToRead));
    bytesRead = 0;
    totalBytesRead = 0;
    while(bytesRead < bytesToRead){ // reads in first bytesToRead worth of bytes from file
      bytesRead = read(fd, buffer+totalBytesRead, bytesToRead-totalBytesRead);
      errorNumber = errno;
      totalBytesRead+=bytesRead;
      if(bytesRead==0)
	break;
      if(bytesRead<0){
	printf("Fatal Error: Unable to read bytes from file. Errno: %d", errorNumber);
	exit(0);
      }
    }
    sizeOfFile+=strlen(buffer);
    head = parseString(buffer, delim, head);  //converts bytes read to a LL
  }while(bytesRead != 0);
  free(buffer);
  close(fd);
  if(sizeOfFile==0)
    printf("Warning: file is empty\n");
  return head;
}
int main (int argc, char** argv){
  errorCheck(argc, argv);
  char command=argv[1][1];  
  node* head = readFile(argc, argv[2]);
  setFileType(head);
  void* array=makeArray(head);
  sortFile( command, array, fileType);
  return 0;
}
