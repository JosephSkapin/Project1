#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>



int main(int argc, char *argv[])
{
	/*
	const char *msg = "Testing\n";
	write(1,msg,strlen(msg)); 
	*/
	
	for(int i = 0; i<5; i++)
	{
		write(1,"Counting...\n",12);
	}

	
	return 0;
}
