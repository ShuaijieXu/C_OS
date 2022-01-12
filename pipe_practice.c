/*Name: Shuaijie Xu
ID: 251187747
Date:2021-10-04
Assignment2  #2*/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>
int main(int argc, char **argv)
{
        pid_t pid1;
        int port[2];
        int status;
        status = pipe(port);
        pid1 = fork();
        if(pid1 < 0)//unsuccessful
        {
                printf("Unsuccessful fork()\n");
        }
        else if(pid1 > 0)//parent
        {
                char X[50];
                strcpy(X, argv[1]);//read first arguemnt to X
                printf("parent (PID %d) created a child(PID %d)\n",getpid(),pid1);//pid1 contain child pid
                printf("parent (PID %d) receives X = \"%s\" from the user\n",getpid(),X);//using \ to esacape so that the printf() can print ""
                write(port[1],X,strlen(X));//write to pipe
                printf("parent (PID %d) writes X = \"%s\" to the pipe\n",getpid(),X);
                wait(NULL);

                char Z2[50];
                read(port[0],&Z2,strlen(argv[1])+strlen(argv[2])+strlen(argv[3])+2);
                printf("parent (PID %d) reads concatenated result from the pipe(Z' = \"%s\")\n",getpid(),Z2);
        }
        else//child
        {
                char temp[50];
                char Y[50];
                char Y2[50];
                char Z[50];
                char Z2[50];
                strcpy(Y,argv[2]);//read second arugment to Y
                strcpy(Z,argv[3]);//read third arugment to Z
                printf("child (PID %d) receives Y = \"%s\" and Z = \"%s\" from the user\n",getpid(),Y,Z);
                strcat(Y," ");//append a space to Y
                strcat(Y,Z);//concatenates Y and Z to
                strcpy(Y2,Y);//generate Y'(Y2=Y')
                printf("child (PID %d) concatenates Y and Z to generate Y' = \"%s\"\n",getpid(),Y2);
                read(port[0],&temp,strlen(argv[1]));//read the X to temp
                printf("child (PID %d) reads X form pipe = \"%s\"\n",getpid(),temp);
                strcat(temp," ");
                strcat(temp,Y2);
                strcpy(Z2,temp);//generate Z'(Z2=Z')
                printf("child (PID %d) concatenates X and Y' to generate Z' = \"%s\"\n",getpid(),Z2);
                write(port[1],Z2,strlen(Z2));
                printf("child (PID %d) write Z' into the pipe\n",getpid());
        }
}