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
* PROT_NONE := Pages may be accessed.<br/>
* PROT_READ := Pages may be read.<br/>
* PROT_WRITE := Pages may be written.<br/>
* PROT_EXEC := Pages may be executed.<br/>

Any page protection scheme can be overwritten by the function `int mprotect(void *addr, size_t len, int prot)`.<br/>

Flags:
Unix provides flags to dictate mapping of data:<br/>
* MAP_ANON: Performs anonymous mapping. Anon mapping is usually done by a program requesting space. This requested space is first provided in a zero filled virtual memory, which is provisioned on a copy on write basis.<br/>
* MAP_FIXED: This flag specifies whether memory has to be overwritten at the specified address within the mmap function.<br/>
* MAP_FILE: Default mapping type.<br/>
* MAP_SHARED: Modifications are shared.(taken from man pages. Will be updated)<br/>

mmap returns a pointer to the allocated region in the memory.<br/>

Note:
Function mmap2 - <br/> `void *mmap2(void *addr, size_t length, int prot, int flags, int fd, off_t pgoffset);`
 allows ability to map large files(up to 2^44 bytes), since the offset is specified in 4096-byte units, instead of bytes.<br/>

2. sbrk:
`void* sbrk(int incr)` <br/>

This function increments the current block by amount 'incr'. Usage varies between sbrk() and brk() functions.<br/> brk() sets the break or the lowest address of a process's data segment(which is unitialized data) to addr.<br/>
Memory is allocated in multiples of page size. If the requested memory to be allocated extends beyond the required page size, the entire remaining page is automatically reserved.<br/>

The current value of program break is returned by `sbrk(0)`. Program break is basically address of the first region beyond the current end of the data region. Thus, if the break value increases, the amount of available space increases. <br/>

Return Value: <br/>
brk() returns 0 if the operation is successful. sbrk() being a wrapper over brk(), returns the location of the break point, i.e end of last allocated memory region.<br/>
brk() returns -1 if the operation fails, upon which sbrk() also returns a -1 and sets the global error variable(errno).

Note:
* sbrk() assumes that the heap is contiguous.
* brk() and sbrk() were removed in POSIX.1-2001

# Observations:
1. Since mmap first maps address onto a virtual memory, following which it does a copy-on-write to the physical page, any out of bounds reference will yield in a zero value.
