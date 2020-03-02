#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

struct memoryChunk{
  int size;  // represents the size of each chunk
  int available;  // represents if the current chunk is available. 1 if available
  struct memoryChunk *next;
  struct memoryChunk *prev;
  bool isEnd;
};

// findChunk returns a pointer to the object with size equalling the required size.
// a first fit linear search.
struct memoryChunk* findChunk(memoryChunk *head, int size){
  memoryChunk* ptr=head;
  while(ptr!=NULL){
    if(ptr->size>=size && ptr->available==1){
      return ptr;
    }
    ptr=ptr->next;
  }
  return NULL;
}

// splitChunk splits a given chunk into 2 - an allocated chunk of required size and the remaining free chunk.
// Input Type: Last available node
void splitChunk(memoryChunk *curr, int size){
  // ptr is the new free chunk of memory
  struct memoryChunk* ptr;
  ptr->isEnd=true;
  ptr->available=1;
  ptr->size=curr->size-size;
  ptr->next=curr->next;
  ptr->prev=curr;

  if(ptr->next!=NULL){
    ptr->next->prev=ptr;
  }

  curr->isEnd=false;
  curr->size=size;
  curr->available=0;
  curr->next=ptr;

}

// mod_size: increases the allocation size of the current chunk. basically realloc
// return type: bool specifying if the operation succeeded.
bool mod_size(memoryChunk* curr, int size){
  // first navigate to the end of the list, reach the free sized pool
  memoryChunk* ptr=curr;
  if(curr->size<size){
    splitChunk(curr, size);
  }
  else{
    // we have to increase size of the current alloc
  }
}

/* Goal:
The idea is to mimic memory allocation. memoryChunk is a data structure that represents the free space in the memory.
This structure is a doubly linked list and contains information on all the data that has been linked.

Each object will have the following data members:
1. Size --  Represents the size of the free chunk
2. Available --  Represents if the current chunk is available.
3. Next Pointer -- Represents the next available space in memory
4. Prev Pointer --  Points to the previous allocated node
5. isEnd --  a boolean type object that represents whether we have reached the end of allocated space.

How Memory Allocation works:
Malloc is used to allocate memory of specified size. We pass along the size of data to be allocated as an argument.
Calloc is used for purposes similar to malloc, with the only differences being that calloc allocates continuous memory.

tl;dr -- both are just system calls to mmap.

While we allocate chunks here, physically a mapping table contains actual mappings to physical pages.

General compaction strategies include traversing the allocated memory as if its a graph, identifying the space still in use
and freeing up the rest.
*/
int main(int argc, char **argv){

}
