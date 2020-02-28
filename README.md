# malloc-clone
Simple malloc clone

# Goal
Understand the idea behind malloc and build a simple clone to allocate and free memory.

# Todo:
1. Play around with mmap, sbrk and free
2. Write a first fit allocator for allocating memory.

# Program:
To run the program, compile via the gcc compiler: `gcc mmap-example.c -o mmap-example`
We will be doing a file based memory mapping and hence, provide the file, `testfile.txt` as a command line argument.
`./mmap-example testfile.txt`

# Readings:
1. mmap:
mmap can map memory or allocate devices/files to memory.
Function signature is : `void* mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset)`.

Calling a function with the above arguments, causes all memory pages starting from the address specified by the pointer addr, continuing for up to len bytes, to be mapped from the object described by the object fd. The location of object fd starts from an offset.
** If offset or len is not a multiple of the pagesize, the mapped region may extend past the specified range, and any extension is zero filled.

prot specifies the protection schemes used.

Note:
If MAP_FIXED flag is true, the mapping of the request to location addr is fixed. Thus any previously existing data will be overwritten. If false, the requested memory will be mapped to any available space, starting from addr


Page protection schemes:
Following are the protection schemes available:
a) PROT_NONE := Pages may be accessed.
b) PROT_READ := Pages may be read.
c) PROT_WRITE := Pages may be written.
d) PROT_EXEC := Pages may be executed.

Any page protection scheme can be overwritten by the function `int mprotect(void *addr, size_t len, int prot)`.

Flags:
Unix provides flags to dictate mapping of data:
a) MAP_ANON: Performs anonymous mapping. Anon mapping is usually done by a program requesting space. This requested space is first provided in a zero filled virtual memory, which is provisioned on a copy on write basis.
b) MAP_FIXED: This flag specifies whether memory has to be overwritten at the specified address within the mmap function.
c) MAP_FILE: Default mapping type.
d) MAP_SHARED: Modifications are shared.(taken from man pages. Will be updated)
