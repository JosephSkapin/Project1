#include <stdlib.h>
#include <stdio.h>

int main()
{

	//malloc something
	
	void *a = malloc(128);
	void *b = malloc(64);
	void *c = calloc(1, sizeof(b));
	free(a);
	//free(b);
	
	
	return 0;
}
