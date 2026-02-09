#include <stdlib.h>
#include <stdio.h>

//argc = argument count, an integer representing the toal number of items on the command line, including the name
//of the program itself

//argv = argument vector, an array of strings containing those actual items
int main(int argc, char * argv[])
{
	if(argc >2) return 0; // basically if the argument count is less than 2 , exit the program

	//malloc something
	//atoi is ASCII to integer
	/*
	int num_allocs = atoi(argv[1]);
	int num_frees = atoi(argv[2]);
	
	//Just to produce a general leak
	for(int i = 0; num_allocs >i; i++)
	{
		void* prtr = malloc(64);
	}
	*/
	
	void *a = malloc(128);
	void *b = malloc(64);
	void *c = malloc(32);
	void *d = calloc(5, sizeof(b));
	a = realloc(a,100);
	//free(a);
	//free(b);
	//free(c);
	
	
	
	return 0;
}
