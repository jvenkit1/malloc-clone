#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

int main(){
  int inc_space=10;
  printf("Incrementing allocated space by %d bytes\n", inc_space);
  void *p=sbrk(0);
  printf("Current break is %d\n", p);
  return 0;
}
