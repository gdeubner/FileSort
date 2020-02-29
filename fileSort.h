#ifndef FILESORT_H
#define FILESORT_H


char* LEGAL_CHARS = "-abcdefghijklmnopqrstuvwxyz1234567890";
char* ALL_NUMS = "-1234567890";

int comparator(void*, void*);

//node for storing each string token from the file
typedef struct NODE{
  char* str;
  struct NODE* next;
  int delimTerm;
}node;

int errorCheck(int argc, char** argv);
void printArray(void*, int);
int setFileType(node*);
void* makeArray(node*);
int compareChar(void*, void*);
int compareInt(void*, void*);
char* cleanStr(char*);
void printLL(node*);
node* parseString(char* buffer, char delim, node* head);
node* readFile(int, char*);
int insertionSort(void*,int (*comparator)(void*,void*));
int quickSort(void*,int (*comparator)(void*,void*));
void split(void*,int,int,int (*comparator)(void*,void*));



#endif
