#ifndef FILESORT_H
#define FILESORT_H


char* LEGAL_CHARS = "-abcdefghijklmnopqrstuvwxyz1234567890";
char* ALL_NUMS = "-1234567890";

int comparator(void*, void*);

//node for storing each string token from the file
typedef struct NODE{
  char* str;
  struct NODE* next;
  
}node;

#endif
