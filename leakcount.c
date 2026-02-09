#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/* When leakcount runs, it will execute the specified program using fork+exec*, popen, or system.
* Can use any of those	
* Leakcount needs to set the LD_PRELOAD environment variable to the path of your shim library
*/


//int setenv(const char *name, const char *value, int overwrite); This is how setenv operates, makes environment variable.

int main(int argc, char *argv[])
{
	setenv("LD_PRELOAD","./mem_shim.so",1);
	
	//printf("Hey leakcount is running bud\n");
	/* Example for execvp --> int execvp(const char *file, char 8const argv[]);
	 in this case argv entry 1 is used for the first argument and the second argument takes the adress
	 of the first entry in argv[].
	 So the first argument is taking in the file we are inputing by doing ./leakcount ./testprogram
	 so ./test program is being passed into the first argument as the file we want to execute
	 the second input to execvp is a list of arguments to be made available to the new program we want to execute.
	*/
	execvp(argv[1], &argv[1]);

	
	
	//want to use exec + fork, or evecvpe

	
}
