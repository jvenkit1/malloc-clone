#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

const int MEMSIZE=300;

struct memoryChunk{
  int size;  // represents the size of each chunk
  int available;  // represents if the current chunk is available. 1 if available
  struct memoryChunk *next;
  struct memoryChunk *prev;
  int isEnd;
};

struct memoryChunk* initPartition(){
  struct memoryChunk* head=(struct memoryChunk*)malloc(sizeof(struct memoryChunk));
  head->size=MEMSIZE;
  head->available=1;
  head->next=NULL;
  head->prev=NULL;
  head->isEnd=1;
  // printf("\n%d-%d\n", head->next, head->prev);
  return head;
}

// findChunk returns a pointer to the object with size equalling the required size.
// a first fit linear search.
struct memoryChunk* findChunk(struct memoryChunk *head, int size){
  struct memoryChunk* ptr=head;
  while(ptr!=NULL){
    if(ptr->size>=size && ptr->available==1){
      return ptr;
    }
    ptr=ptr->next;
  }
  return NULL;
}

void displayNodes(struct memoryChunk* head){
  struct memoryChunk* ptr=head;
  while(ptr!=NULL){
    printf("%d -- %d\n", ptr->size, ptr->available);
    ptr=ptr->next;
  }
}

// splitChunk splits a given chunk into 2 - an allocated chunk of required size and the remaining free chunk.
// Input Type: Last available node
void splitChunk(struct memoryChunk *curr, int size){
  // ptr is the new free chunk of memory
  struct memoryChunk* ptr=(struct memoryChunk*)malloc(sizeof(struct memoryChunk));
  ptr->isEnd=1;
  ptr->available=1;
  ptr->size=curr->size-size;
  ptr->next=curr->next;
  ptr->prev=curr;

  if(ptr->next!=NULL){
    ptr->next->prev=ptr;
  }

  curr->isEnd=0;
  curr->size=size;
  curr->available=0;
  curr->next=ptr;

}

// mod_size: increases the allocation size of the current chunk. basically realloc
// return type: bool specifying if the operation succeeded.
int mod_size(struct memoryChunk* curr, int size){
  // first navigate to the end of the list, reach the free sized pool
  struct memoryChunk* ptr=curr;
  int chunkFound=0;
  int csize=curr->size;
  if(curr->size<size){
    // test this case
    splitChunk(curr, size);
  }
  else{
    // we have to increase size of the current alloc
    // set the current block as free and create a new block from the free pool
    while(ptr!=NULL){
      if(ptr->available==1 && ptr->size>=size){
        // split ptr to get a chunk of required size
        splitChunk(ptr, size);
        chunkFound=1;
        break;
      }
      ptr=ptr->next;
    }
    if(chunkFound==0){
      printf("Error ! Couldn't partition and create new chunk\n");
      return 0;
    }
    curr->available=1;
  }
  return 1;
}

int memalloc(struct memoryChunk* head, int size){
  struct memoryChunk* ptr=head;
  int splitSucceeded=0;
  while(ptr!=NULL){
    if(ptr->size>=size&&ptr->available==1){
      splitChunk(ptr, size);
      splitSucceeded=1;
      break;
    }
    ptr=ptr->next;
  }
  if(splitSucceeded==0){
    printf("Error occured while splitting chunks. Not enough memory !\n");
    return -1;
  }
  return 1;
}

// This function frees a chunk of size 'size'
int freemem(struct memoryChunk* head, int size){
  struct memoryChunk* ptr=head;
  while(ptr!=NULL){
    if(ptr->size==size){
      // found the required chunk
      ptr->available=1;
      return 1;
    }
    ptr=ptr->next;
  }
  printf("Couldn't find the required chunk.\n");
  return 0;
}

struct memoryChunk* findFirstUsedPointer(struct memoryChunk* head){
  struct memoryChunk* ptr=head;
  while(ptr!=NULL){
    if(ptr->available==0){
      return ptr;
    }
    ptr=ptr->next;
  }
  return NULL;
}

int mergeChunks(struct memoryChunk* head2, struct memoryChunk* head){
  // compact head2
  // combine chunks whose availability is 1
  struct memoryChunk* ptr=head;
  struct memoryChunk* ptr2=head2;
  struct memoryChunk* compactedNode=initPartition();
  int sum=0;
  while(ptr->next!=NULL){
    ptr=ptr->next;
  }
  // compaction
  while(ptr2!=NULL){
    sum+=ptr2->size;
    ptr2=ptr2->next;
  }
  // merging
  compactedNode->size=sum;
  ptr->next=compactedNode;
  compactedNode->prev=ptr;
}

int reorderChunks(struct memoryChunk* head){
  // take chunks whose availability is 1 and size is initialized, and add them to the end of the list.
  struct memoryChunk* ptr=head;
  struct memoryChunk* ptr2=head;
  struct memoryChunk* newNode=(struct memoryChunk*)malloc(sizeof(struct memoryChunk));
  struct memoryChunk* newListHead=newNode;
  struct memoryChunk* firstUsedPtr=findFirstUsedPointer(ptr2);
  newNode->available=1;
  while(ptr!=NULL){
    if(ptr->available==1){
      newNode->next=ptr;  // added ptr to newnode pointer
      if(ptr->next)
        ptr->next->prev=ptr->prev;
      if(ptr->prev){
        ptr->prev->next=ptr->next;
      }
      newNode->next->prev=newNode;
      newNode=newNode->next;
    }
    ptr=ptr->next;
  }
  printf("\nDisplaying Nodes now\n");
  displayNodes(newListHead->next);
  printf("\nDisplaying Nodes now\n");
  displayNodes(firstUsedPtr);

  printf("Compacting and displaying nodes\n");
  mergeChunks(newListHead->next, firstUsedPtr);
  displayNodes(firstUsedPtr);
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

2 functions:
1. memalloc() - allocates memory
2. freemem() - frees memory - performs general compaction as well.

While we allocate chunks here, physically a mapping table contains actual mappings to physical pages.

General compaction strategies include traversing the allocated memory as if its a graph, identifying the space still in use
and freeing up the rest.
*/
int main(int argc, char **argv){
  struct memoryChunk* head=initPartition();
  struct memoryChunk* ptr=head;
  struct memoryChunk* ptr2=head;
  memalloc(ptr, 30);
  memalloc(ptr, 40);
  memalloc(ptr, 25);
  memalloc(ptr, 55);
  memalloc(ptr, 15);
  displayNodes(head);
  printf("\nFreeing Memory Now\n");
  freemem(ptr2, 30);
  freemem(ptr2, 55);
  displayNodes(head);
  printf("\nReordering\n");
  reorderChunks(head);
  return 0;
}
