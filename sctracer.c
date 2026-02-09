#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include<linux/ptrace.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <unistd.h>
#include <time.h>
#include <string.h>

/* Uses ptrace API
* Can read or write to another process's  memory or registers, monitor the signals recieved by the process,
* stop and start the process when a system call occurs , and of course kill the process.
* At the bare minimum need to use PTRACE_TRACEME, PTRACE_SYSCALL, PTRACE_PEEKUSER, PTRACE_SETOPTIONS requests
* and PTRACE_O_TRACESYSGOOD option.

* Read up on the man page for ptrace, also for waitpid, and getpid

* Strongly recommended to use fork to create new process, The child process should call ptrace(PTRACE_TRACEME) * before calling exec*. Once the child process has completed, yourprogram will write the results into an 
* output file. Each system call should be on its own line. 
*/
int main(int argc, char *argv[])
{
	int sysCount[500] = {0};
	pid_t child = fork();
	if(child == 0)
	{	
	//Allow myself(the child) to be traced)
	ptrace(PTRACE_TRACEME);
	
	//Stop myself--allow the parent to get ready to trace me
	kill(getpid(),SIGSTOP);
	
	
	
	//child process now is working in the testprogram
	//execvp(cmd_args[0],cmd_args);
	execvp(argv[1],&argv[1]);
	
	perror("execvp failed");
	exit(1);
	
	
	}
	else
	{
		//this is the parent
		int status, syscall_num;
		
		
		
		//wait for the child to stop itself
		waitpid(child,&status,0);
		
		//this option makes it easier to distinguish normal traps from 
		//system calls
		ptrace(PTRACE_SETOPTIONS,child,0,PTRACE_O_TRACESYSGOOD);
		
		do{
			
			//tells kernel to let the child proccess to continue running
			//also stop the child again as soon as it eners or exits a system call
			ptrace(PTRACE_SYSCALL,child,0,0);
			
			//waiting for the child to change
			waitpid(child, &status,0);
			
			//if child has finished execution and exited normally, the loop must stop
			if(WIFEXITED(status))
			{
				break;
			}
			
			
			//psi will be filled by the kernel with data about wether the process is entering or
			//exiting a system call, which system call it is, and what the arguments are.
			struct ptrace_syscall_info psi;
			//tells the kernel to populate the structure with all relevant metadata about the
			//current system call
			ptrace(PTRACE_GET_SYSCALL_INFO,child,sizeof(struct ptrace_syscall_info), &psi);
		
		
		
		
			switch(psi.op)
			{
				//occurs when child is about to execute the system call but hasnt
				//done the work yet
				case PTRACE_SYSCALL_INFO_ENTRY:
			
					syscall_num = psi.entry.nr;
					printf("It's an entry! (%d)\n", syscall_num);
				
					sysCount[syscall_num]++;
                			break;
                		//occurs after the system call has finished.
          	 		case PTRACE_SYSCALL_INFO_EXIT:
               				printf("It's an exit! (%d)\n", syscall_num);
                			break;
        
            			default:
                			printf("Something else.\n");
			}	
			//wait until the process is stopped or bit 7 is set in
			//the status 
			//checks if child is in a stopped state
		}while(!(WIFSTOPPED(status) && WSTOPSIG(status & 0x80)));
		
		
		
		printf("My child called system call #%d.\n",syscall_num);
        
        	
        	//Tells kernel to resume the stopped child process and run until it exits or
        	//receives a signal
        	ptrace(PTRACE_CONT, child, NULL, NULL);
        	//The parent process pauses and waits for the child to officially terminate.
        	waitpid(child, NULL, 0);
        	
	}
	
	FILE *f;
	f = fopen(argv[2],"w"); // "w" specifies that we are writing to this file
	if(f == NULL)
	{
		perror("Failed to open output file");
		exit(1);
	}
	
	for(int i = 0; i<500;i++)
	{
		if(sysCount[i]>0)
		{
			fprintf(f, "%d\t%d\n",i,sysCount[i]);
		}
	}
	
	fclose(f);
	
	
	

	
}
