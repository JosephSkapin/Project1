#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>





//Setting up linked list data structure
typedef struct node {
	void *ptr; //pointer to address
	size_t size;
	struct node *next;
	
}node_t;


static node_t *head = NULL;
//Function prototypes to implement
void *(*original_malloc)(size_t size);
void  (*original_free)(void *);
void *(*original_calloc)(size_t, size_t);
void *(*original_realloc)(void *, size_t);

//Sets up links to lib.c respective functions
__attribute__((constructor)) static void init(void)
{
	
	original_malloc = dlsym(RTLD_NEXT,"malloc");
	original_free = dlsym(RTLD_NEXT, "free");
	original_calloc = dlsym(RTLD_NEXT,"calloc");
	original_realloc = dlsym(RTLD_NEXT,"realloc");
	
}


//shim file deals with actual memory allocation and memory leaks
//a memory leak is defined as a call to malloc, calloc or ralloc that returns a pointer that is never
//freed during the course of the programs execution (via free or realloc)

//implement these functions

//Is given a pointer to an address, so go through linked list and compare
// the passed in pointer to the pointer stored in the linked list structure
//Once found...free the pointer in the linked list
void free(void *ptr)
{
	if(ptr ==NULL || head == NULL) return;
	node_t *curr =head;
	node_t *prev = curr;
	while(curr->ptr != ptr)
	{
		prev = curr;
		curr = curr->next;
	}
	//re arrange list to things connect
	if(curr == head)
	{
		head = curr->next;
	}
	else
	{
		prev->next = curr->next;
	}
	
	
	original_free(curr);
	original_free(ptr);
	
	write(2,"free found\n",12);
}


void *malloc(size_t size)
{
	
	void *ptr = original_malloc(size);
	if(ptr == NULL) return NULL;
	
	node_t *newNode = (node_t*)original_malloc(sizeof(node_t));
	newNode->ptr = ptr;
	newNode->size = size;
	
	if(head != NULL)
	{
		node_t *curr = head; 
		node_t *prev = curr;
		// walk through linked list, curr finds the end, prev is the point where we insert next
		while(curr != NULL) 
		{
			prev = curr;
			curr = curr->next;
		}
	
		prev->next = newNode;
	
		newNode->next = NULL;
		write(2,"Node Added\n",12);
	}
	else //Case for empty list
	{
		head = newNode;
		write(2,"First Node Added\n",17);
	}
	
	
	
	write(2,"malloc found\n",14);
	
	
	
	return ptr;

}

//void return type
/* Void pointers (void *) is a generic pointer,
* when malloc returns it is handing you a raw starting address for a block of memory
* without telling the compiler what kind of data will be stored there.
* So basically we are using a void pointer here because we are tracking all types of 
* memory allocation, same goes for calloc and realloc
*
*
//Calloc takes 2 parameters, 1: the number of elements you want to allocate
// 2. the size of each individual element in bytes	
*/		   
void *calloc(size_t count, size_t size)
{
	
	
	
	write(2,"calloc found\n",14);
	
}					 

/*Realloc: First parameter Takes in a pointer to a memory block that needs to be 
resized. This pointer must have previously been returned by malloc
calloc or realloc.2nd parameter is the new size for the memory block in bytes
The new size can be smaller or larger than the original.
*/				
void *realloc(void *ptr, size_t size)
{
	
}
					
					
			

//How to call the original version of malloc
//#include <dlfcn.h>
//void *(*original_malloc) (size_t size);
//original_malloc = dlsym(RTLD_NEXT, "malloc");
//void *ptr = original_malloc(17);

//destructor prints out our leak report
void __attribute__((destructor)) leak_report()
{
	size_t leakSum = 0; //keeps track of total number of leaked bytes
	int leakCount = 0; //Counts the number of memory leaks
	fprintf(stderr,"----Memory Leak Report----\n");
	node_t *curr = head; //starts at head of linked list
	if(curr == NULL) fprintf(stderr,"No Leaks\n"); //if head is Null then there is no memory Leaks to report
		
	while(curr != NULL)
	{
		
		fprintf(stderr,"LEAK\t%zu \n",curr->size);
		leakSum += curr->size;
		leakCount++;
		curr = curr->next;
	}
	
	fprintf(stderr, "TOTAL\t%d\t%zu\n",leakCount,leakSum);
}






