#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

size_t getFilesize(const char* filename) {
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}

void file_based_mapping(const char *filename){
  // size of the file that will be inserted into a specific location in the memory.
  size_t filesize=getFilesize(filename);
  printf("The filesize is %d\n", filesize);

  // fd object that is to be written into memory
  int fd=open(filename, O_RDONLY, 0);
  assert(fd!=-1);  // reading the file did not fail.

  // we set the data present in the fd object to any region of our memory.
  void *ptr = mmap(NULL, filesize,  PROT_READ, MAP_PRIVATE, fd, 0);
  assert(ptr!=MAP_FAILED);

  write(1, ptr, filesize);

  int rc=munmap(ptr, filesize);  // freeing the memory
  assert(rc==0);
  close(fd);
}

void anonymous_mapping(){
  // we will be mapping a multiple of the page size
  // The page size can be obtained via the getpagesize function
  size_t pagesize=getpagesize();
  printf("The pagesize is %d\n", pagesize);

  // for anonymous mapping, the value of fd will be -1
  char* region = mmap((void*)(pagesize*(1<<20)), pagesize, PROT_READ | PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, 0);
  assert(region!=MAP_FAILED);

  strcpy(region, "The quick brown fox jumps over the fence");
  printf("The contents of the region is: %s\n", region);

  // free memory
  int rc=munmap(region, pagesize);
  assert(rc==0);
}


int main(int argc, char **argv){
  file_based_mapping(argv[1]);
  anonymous_mapping();
  return 0;
}
