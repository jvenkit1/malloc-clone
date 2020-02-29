# malloc-clone
Simple malloc clone

# Goal
Understand the idea behind malloc and build a simple clone to allocate and free memory.

# Todo:
1. Play around with mmap, sbrk and free
2. Write a first fit allocator for allocating memory.

# Program:
To run the program, compile via the gcc compiler: `gcc mmap-example.c -o mmap-example` <br/>
We will be doing a file based memory mapping and hence, provide the file, `testfile.txt` as a command line argument.
`./mmap-example testfile.txt`

# Readings:
1. mmap:
`void* mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset)`.<br/>

mmap function is used to map memory, allocate devices/files to memory.<br/>

Calling a function with the above arguments, causes all memory pages starting from the address specified by the pointer addr, continuing for up to len bytes, to be mapped from the object described by the object fd. The location of object fd starts from an offset.<br/>
** If offset or len is not a multiple of the pagesize, the mapped region may extend past the specified range, and any extension is zero filled. <br/>
Note:<br/>
If MAP_FIXED flag is true, the mapping of the request to location addr is fixed. Thus any previously existing data will be overwritten. If false, the requested memory will be mapped to any available space, starting from addr

Page protection schemes: Specified by the parameter prot
Following are the protection schemes available:<br/>
a) PROT_NONE := Pages may be accessed.<br/>
b) PROT_READ := Pages may be read.<br/>
c) PROT_WRITE := Pages may be written.<br/>
d) PROT_EXEC := Pages may be executed.<br/>

Any page protection scheme can be overwritten by the function `int mprotect(void *addr, size_t len, int prot)`.<br/>

Flags:
Unix provides flags to dictate mapping of data:<br/>
a) MAP_ANON: Performs anonymous mapping. Anon mapping is usually done by a program requesting space. This requested space is first provided in a zero filled virtual memory, which is provisioned on a copy on write basis.<br/>
b) MAP_FIXED: This flag specifies whether memory has to be overwritten at the specified address within the mmap function.<br/>
c) MAP_FILE: Default mapping type.<br/>
d) MAP_SHARED: Modifications are shared.(taken from man pages. Will be updated)<br/>

mmap returns a pointer to the allocated region in the memory.<br/>

Note:
Function mmap2 - <br/> `void *mmap2(void *addr, size_t length, int prot, int flags, int fd, off_t pgoffset);`
 allows ability to map large files(up to 2^44 bytes), since the offset is specified in 4096-byte units, instead of bytes.<br/>

# Observations:
1. Since mmap first maps address onto a virtual memory, following which it does a copy-on-write to the physical page, any out of bounds reference will yield in a zero value.
