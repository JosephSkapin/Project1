# Project1
Name: Joseph Skapin


This project contains two main programs, the first is consists of two parts, one is leakcount.c which executes a specified program using execvp. The other part is mem_shim.so which will act as the interceptor for calls to malloc
calloc realloc, and free. The result is a report detailing how many leaks were ecountered after the main program finishes.
The second main program is called sctracer.c which uses the ptrace api to keep track of and record the system calls done during the programs execution. The results are written inside of a text file once finished.

KNOWN PROBLEMS:
mem_shim.c 's current implementation of the free function has a worst case scenario where if you choose to free
nodes at the end of the linked list then the program dramatrically slows down putting this case as O(N^2) complexity. During testing, freeing 100,000 nodes from back to front took 10 seconds to finish. 


DESIGN:
A linked list is implemented in the shim library as the method to keep track of the leaks.
leakcount sets an environment variable with LD_PRELOAD in order to set up the link between leakcount and the shim 
library, then leakcount executes the program passed in. LD_PRELOAD makes sure our shim library is considered first before lib.c
mem_shim.c takes advantage of constructors and destructors in my implementation of the shim library. The constructor is used to set up the link to the lib.c functions malloc, free, calloc and realloc before main() runs. 
This is only possible since our program must be dynamically linked, so ld.so is available to us and makes our call
to dlsym in the constructor possible.
The destructor implemented in mem_shim.c is used to print the leak report after main() finishes in the testprogram.
It loops through the linked list and prints out the size of the nodes that are left in the linked list. The loop also sums up the number of leaks aswell as the total number of bytes in separate variables.
After the loop finishes, it prints out the TOTAL leak summary. If no leaks are detected, only the Total summary will be printed.
Malloc is designed to insert every new node at the end of the linked list by keeping a global variable named tail which is set to the end of the linked list. There is also a global variable called head which is the front of the linked list.
The free function essentially loops through the linked list in order to find the correct node to remove and free. As noted in the KNOWN PROBLEMS section, if the test program calls to free nodes at the end of the linked list, the speed of the program dramatically slows down.

sctracer: 
parent uses fork() to create a child process.
The child process calls ptrace(PTRACE_TRACEME) and stops itself with SIGSTOP before calling exevp to allow the parent to properly attach.
The parent process sets the PTRACE_O_TRACESYSGOOD option to distinguish which calls to look for, which are the system calls.
A do-while loop use PTRACE_SYSCALL to stop the child every time a system call occurs the in the child process and exit. 
The system call numbers are collected using PTRACE_GET_SYSCALL_INFO and then recorded inside of an array.
After the array has all the system calls recorded, the program writes the output into a .txt file.




REFERENCE MATERIALS:
https://phoxis.org/2011/04/27/c-language-constructors-and-destructors-with-gcc/
https://www.man7.org/linux/man-pages/man3/dlsym.3.html
https://man7.org/linux/man-pages/man2/ptrace.2.html
https://man7.org/linux/man-pages/man8/ld.so.8.html
Project1 Assignment PDF.




