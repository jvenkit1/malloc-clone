#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

size_t getFilesize(const char* filename) {
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}

int main(int argc, char **argv){
  // The page size can be obtained via the getpagesize function
  size_t pagesize=getpagesize();
  printf("The pagesize is %d\n", pagesize);

  // size of the file that will be inserted into a specific location in the memory.
  size_t filesize=getFilesize(argv[1]);
  printf("The filesize is %d\n", filesize);

  // fd object that is to be written into memory
  int fd=open(argv[1], O_RDONLY, 0);
  assert(fd!=-1);  // reading the file did not fail.

  // we set the data present in the fd object to any region of our memory.
  void *ptr = mmap(NULL, filesize,  PROT_READ, MAP_PRIVATE, fd, 0);
  assert(ptr!=MAP_FAILED);

  write(1, ptr, filesize);

  int rc=munmap(ptr, filesize);  // freeing the memory
  assert(rc==0);
  close(fd);
  return 0;
}
