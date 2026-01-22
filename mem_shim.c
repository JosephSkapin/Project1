#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

//shim file deals with actual memory allocation and memory leaks
//a memory leak is defined as a call to malloc, callod or ralloc that returns a pointer that is never
//freed during the course of the programs execution (via free or realloc)

//implement these functions
void free (void *ptr)
{
	//fix this
	//void (*original_free)(void *ptr) = dlsym(RTLD_NEXT,"free");
	
	//original_free(ptr);
	
	//write(2,"free found\n",12);
}

void *malloc(size_t size)
{
	void *(* original_malloc)(size_t size) = dlsym(RTLD_NEXT,"malloc");
	void *ptr = original_malloc(size);
	
	write(2,"malloc found\n",14);

}

//void return type
			   /* Void pointers (void *) is a generic pointer,
			   * when malloc returns it is handing you a raw starting address for a block of memory
			   * without telling the compiler what kind of data will be stored there.
			   * So basically we are using a void pointer here because we are tracking all types of 
			   * memory allocation, same goes for calloc and realloc
			   */
void *calloc(size_t count, size_t size)
{
	void *(*original_calloc)(size_t count, size_t size) = dlsym(RTLD_NEXT,"calloc");
	void *ptr = original_calloc(count,size);
	
	write(2,"calloc found\n",14);
	
}					 //Calloc takes 2 parameters, 1: the number of elements you want to allocate
					// 2. the size of each individual element in bytes
					
void *realloc(void *ptr, size_t size)
{
	
}
					//Realloc: First parameter Takes in a pointer to a memory block that needs to be 
					// resized. This pointer must have previously been returned by malloc
					//calloc or realloc.
					//2nd parameter is the new size for the memory block in bytes
					//The new size can be smaller or larger than the original.
					
			

//How to call the original version of malloc
//#include <dlfcn.h>
//void *(*original_malloc) (size_t size);
//original_malloc = dlsym(RTLD_NEXT, "malloc");
//void *ptr = original_malloc(17);
/*
int main()
{
	
	void *(*original_malloc) (size_t size);
	original_malloc = dlsym(RTLD_NEXT,"malloc");
	void *ptr = original_malloc(17);
	
	printf("LEAK %p",ptr);
}
*/
