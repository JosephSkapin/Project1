#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define MAX_NODES 10000



//Setting up linked list data structure
typedef struct node {
	void *ptr; //pointer to address
	size_t size;
	struct node *next;
	
}node_t;

//static node_t pool[MAX_NODES];
//static int node_ptr = 0;
//static node_t *head = NULL;

//Initializes list before run time? I think thats how constructor works
__attribute__((constructor)) static void init_list(void)
{
	//void *(* original_malloc)(size_t size) = dlsym(RTLD_NEXT,"malloc");
	original_malloc = dlsym(RTLD_NEXT,"malloc");
	original_free = dlsym(RTDL_NEXT, "free");
	original_calloc = dlsym(RTLD_NEXT,"calloc");
	original_realloc = dlsym(RTLD_NEXT,"realloc");
	
	
	//set up linked list
	//head = (node_t*)original_malloc(sizeof(node_t));
	node_t *head = NULL;
	
}


//shim file deals with actual memory allocation and memory leaks
//a memory leak is defined as a call to malloc, callod or ralloc that returns a pointer that is never
//freed during the course of the programs execution (via free or realloc)

//implement these functions
void free (void *ptr)
{
	//fix this
	//void (*original_free)(void *ptr) = dlsym(RTLD_NEXT,"free");
	
	//original_free(ptr);
	
	write(2,"free found\n",12);
}


void *malloc(size_t size)
{
	//void *(* original_malloc)(size_t size) = dlsym(RTLD_NEXT,"malloc");
	void *ptr = original_malloc(size);
	node_t *newNode = (node_t*)original_malloc(sizeof(node_t));
	node_t *curr = head; //helps us locate where the new node needs to go
	node_t *prev = curr;
	newNode->ptr = ptr;
	newNode->size = size;
	while(curr != NULL) // walk through linked list, curr finds the end, prev is the point where we insert next
	{
		prev = curr;
		curr = curr->next;
	}
	prev->next = newNode;
	newNode->next = NULL;
	
	
	malloc_count++;
	
	
	write(2,"malloc found\n",14);
	
	
	
	return ptr;

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
	//void *(*original_calloc)(size_t count, size_t size) = dlsym(RTLD_NEXT,"calloc");
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
