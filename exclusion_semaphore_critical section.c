/*Name:Shuaijie Xu
    ID: 251187747
    Date:2021-11-18 */
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
pthread_mutex_t lock;
int client_num=0;
int account_num=0;
int money_amount=0;
int balance[255];
int balanceController=0;
void print()
{
	for(int i=0;balance[i]!='\0';i++)
	{
		printf("a%d b %d\n",i+1,balance[i]);
	}
}
void *withdraw_money()
{
	pthread_mutex_lock(&lock);
	/**********Critical Section Starts****************/
	if(money_amount <= balance[account_num-1])
	{
		balance[account_num-1] = balance[account_num-1] - money_amount;
	}
	/************Critical Section Ends**************/
	pthread_mutex_unlock(&lock);
}
void *deposit_money()
{
	pthread_mutex_lock(&lock);
	/***********Critical Section Starts******************/
	balance[account_num-1] = balance[account_num-1] + money_amount;	
	/************Critical Section Ends***************/
	pthread_mutex_unlock(&lock);//exit
}
int getAmountOfClient(FILE *fp,char* buff)
{
	int x=0;
	while(fgets(buff,255,(FILE*)fp)!=NULL)
	{
		for(int i=0;buff[i]!='\0';i++)
		{
			if(i==0 && buff[i]=='c')
			{
				x++;
			}
		}
	}
	return x;
}
int main()
{
	//read file
	FILE *fp;
	char buff[255];
	fp = fopen("assignment_6_input.txt","r");
	if(fp==NULL)
	{
		exit(EXIT_FAILURE);
	}
	size_t amountOfClient = getAmountOfClient(fp,buff);
	pthread_t threads[amountOfClient];
	if(pthread_mutex_init(&lock,NULL)!=0)
	{
		printf("mutex init falied\n");
		return 1;
	}
	fp = fopen("assignment_6_input.txt","r");
	while(fgets(buff,255,(FILE*)fp)!=NULL)//bigger loop to read whole file
	{
		int temp[255];
		int tempController=0;
		int value=0;
		
		for(int i=0;buff[i]!='\0';i++)//read line by line
		{
			if(buff[i]=='b')//set balance
			{
				for(i=i+2;buff[i]!='\n';i++)//start from next number
				{
					temp[tempController]=buff[i]-'0';
					value = value*10+temp[tempController];
					tempController++;
				}
				balance[balanceController]=value;	
				balanceController++;
			}
			else if(i==0 && buff[i]=='c')//figure out which client
			{
				tempController=0;
				value=0;
				for(i=i+1;buff[i]!=' ';i++)
				{
					temp[tempController]=buff[i]-'0';
					value=value*10+temp[tempController];
					tempController++;
				}
				client_num=value;
				i=i+1;//look for action character,d or w
			}
			if(buff[i]=='d')//deposit
			{
				tempController=0;
				value=0;
				for(i=i+3;buff[i]!=' ';i++)//figure out which account
				{
					temp[tempController]=buff[i]-'0';
					value=value*10+temp[tempController];
					tempController++;
				}
				account_num=value;
				tempController=0;
				value=0;
				for(i=i+1;buff[i]!='\n';i++)//figure out money amount
				{
					if(buff[i]==' '||buff[i]=='\0')
					{
						break;
					}
					temp[tempController]=buff[i]-'0';				        
	     				value=value*10+temp[tempController];
					tempController++;
				}
				money_amount=value;
				//start to deal with threads
				int err_thread = pthread_create(&threads[account_num-1], NULL, &deposit_money, NULL);//create thread for client
				if (err_thread != 0)
				{
					printf("\n Error creating thread ");
				}
				pthread_join(threads[account_num-1],NULL);
				
			}
			else if(buff[i]=='w')//withdraw
			{
				tempController=0;
				value=0;
				for(i=i+3;buff[i]!=' ';i++)//figure out which account
				{
					temp[tempController]=buff[i]-'0';
					value = value * 10 + temp[tempController];
					tempController++;
				}
				account_num=value;
				tempController=0;
				value=0;
				for(i=i+1;buff[i]!='\n';i++)//figure out money amount
				{
					if(buff[i]==' '||buff[i]=='\0')
					{
						break;
					}
					temp[tempController]=buff[i]-'0';
					value=value*10+temp[tempController];
					tempController++;
				}
				money_amount = value;
				//start to deal with threads
				int err_thread = pthread_create(&threads[account_num-1],NULL,&withdraw_money,NULL);
				if(err_thread !=0)
				{
					printf("\n Error creating thread ");
				}
				pthread_join(threads[account_num-1],NULL);
			}
		}
	}
	print();
	return 0;
}


