/*Name: Shuaijie Xu
ID: 251187747
Date: 2021-10-11
CS-3305A Assignment #3*/
#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include<sys/types.h>
#include<sys/wait.h>

int port[2];//file descriptor for creating a pipe

void *sum(void *thread_id)
{
        int X;
        int Y;
        read(port[0],&X,sizeof(X));//read from pipe to X
        read(port[0],&Y,sizeof(Y));//read from pipe to Y
        printf("thread(TID %d) read X = %d and Y = %d from the pipe\n",*(int *)thread_id,X,Y);

        int S = X + Y;
        write(port[1],&S,sizeof(S));
        printf("thread(TID %d) write X + Y = %d to the pipe\n",*(int *)thread_id,S);
}

void *odd_even(void *thread_id)
{
        int S;
        read(port[0],&S,sizeof(S));
        printf("thread(TID %d) read X + Y = %d from the pipe\n",*(int *)thread_id,S);

        if(S % 2 == 0)//even
        {
                printf("thread(TID %d) identifies X + Y = %d as an even number\n",*(int *)thread_id,S);
        }
        else//odd
        {
                printf("thread(TID %d) identifies X + Y = %d as an odd number\n",*(int *)thread_id,S);
        }

        write(port[1],&S,sizeof(S));
        printf("thread(TID %d) write X + Y = %d to the pipe\n",*(int *)thread_id,S);
}

void *digit_count(void *thread_id)
{
        int S;
        read(port[0],&S,sizeof(S));
        printf("thread(TID %d) reads X + Y = %d from the pipe\n",*(int *)thread_id,S);

        int count=0;
        int temp = S;//store S in a variable so that we can use it later
        while(temp!=0)//count how many digit
        {
                count++;
                temp/=10;
        }
        printf("thread(TID %d) identifies X + Y = %d as a %d digit number\n",*(int *)thread_id,S,count);
}

int main(int argc, char **argv)
{
        printf("parent(PID %d) receives X = %s and Y = %s from the user\n",getpid(),argv[1],argv[2]);
        int X = atoi(argv[1]);//convert user input from string into integer
        int Y = atoi(argv[2]);//convert user input from string into integer

        pipe(port);
        write(port[1],&X,sizeof(X));
        write(port[1],&Y,sizeof(Y));
        printf("parent(PID %d) writes X = %d and Y = %d to the pipe\n",getpid(),X,Y);

        pthread_t thread_1;
        pthread_t thread_2;
        pthread_t thread_3;
        int thread_1_id=100;
        int thread_2_id=101;
        int thread_3_id=102;

        pthread_create(&thread_1,NULL,sum,&thread_1_id);
        pthread_join(thread_1,NULL);

        pthread_create(&thread_2,NULL,odd_even,&thread_2_id);
        pthread_join(thread_2,NULL);

        pthread_create(&thread_3,NULL,digit_count,&thread_3_id);
        pthread_join(thread_3,NULL);

        return 0;
}
