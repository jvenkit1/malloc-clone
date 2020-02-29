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


Refer to notes for explanation on the operations.
