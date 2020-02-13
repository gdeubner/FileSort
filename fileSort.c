#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>



int* numArray;

int* readFile(int, char**);



int* readFile(int argc, char** argv){
  if(argc <=1){
    printf("ERROR: insuficient arguments.\n");
    return NULL;
  }
  int fd = open(argv[1], O_RDONLY);
  char * buffer  = (char*)malloc(sizeof(char)*101);
  if(buffer==NULL){
    printf("ERROR: unable to malloc.\n");
    return NULL;
  }
    
  
  printf("%s\n", fd);
  
  return NULL;
}


int main (int argc, char** argv){

  numArray = readFile(argc, argv);


  return 0;
}
