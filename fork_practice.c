/*CS 3305a Assignment#1
Name: Shuaijie Xu
ID: 251187747
Due Date:September 27,2021*/
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
int main(int argc, char **argv)
{
	pid_t pid1,pid2,pid3;
	pid1 = fork();
	wait(NULL);
	if(pid1<0)//fail
	{
		printf("Not successful\n");
	}
	else if(pid1>0)//parent
	{
		//run after child_1 terminated
		pid2=fork();
		wait(NULL);
		if(pid2<0)
		{
			printf("Not successful\n");
		}
		else if(pid2>0)//parent(run after all child processes completed)
		{
			printf("child_1 and child_2 are completed and parent process is terminating...\n");
		}
		else//child_2
		{
			printf("parent (PID %d) created child_2 (PID %d)\n",getppid(),getpid());
			pid3 = fork();
			wait(NULL);
			if(pid3<0)
			{
				printf("Not successful\n");
			}
			else if(pid3>0)
			{
				;
			}
			else//child_2_1
			{
				printf("child_2 (PID %d) created child_2.1 (PID %d)\n",getppid(),getpid());			    
				printf("child_2.1 (PID %d) is calling an external program external_program.out and leaving child_2\n",getpid());
				char pid_text[20];
				sprintf(pid_text,"%d",getpid());
				execl(argv[1],"",strcat(pid_text," for child_2.1"),NULL);//argv[0]="./assignment.out" argv[1]="directory/external_program.out"

			}	
		}
	}
	else//child_1
	{
		printf("parent (PID %d) created child_1 (PID %d)\n",getppid(),getpid());
		printf("parent (PID %d) is waitting for child_1 (PID %d) to complete before creating child_2\n",getppid(),getpid());
		printf("child_1 (PID %d) is calling an external program external_program.out and leaving parent\n",getpid());
		char pid_text[20];
		sprintf(pid_text, "%d", getpid());
		execl(argv[1],"",strcat(pid_text," for child_1"),NULL);
	}	
	return 0;
}
