/*Name: Shuaijie Xu
    ID: 251187747
  Date: 2021-11-06
 Title: CS 3305 Assignment #4*/

#include <stdio.h>
#include<stdbool.h>
#include <stdlib.h>

int timeConsume = 0;
int queueControler = 0;
int linkControler = 0;
int ArrivalTimeControler = 0;

typedef struct queueNode
{
	int processid;
	int arrivalTime;
	int burstTime;
}QUEUENODE;
typedef struct linkNode
{
	int processid;
	int arrivalTime;
	int burstTime;
	int startTime;
	int endTime;
	int leftBurstTime;
	int waitingTime;
}LINKNODE;

void enQueue(int processid, int arrivalTime, int burstTime, int quantumTime, QUEUENODE queue[])
{
	queue[queueControler].processid = processid;
	queue[queueControler].arrivalTime = arrivalTime;
	queue[queueControler].burstTime = burstTime;
	queueControler++;
	queue[queueControler].processid = -1;//end of array
}
QUEUENODE deQueue(QUEUENODE queue[])
{
	QUEUENODE firstNode = queue[0];//remember the first node
	for (int i = 0; queue[i].processid != -1; i++)
	{
		queue[i] = queue[i + 1];
	}
	queueControler--;
	return firstNode;
}
void executeRR(QUEUENODE queue[], int process[], int arrivalTime[], int burstTime[], LINKNODE link[], int quantumTime)
{
	//pop from ready queue->insert to link arrya->if finish(no:enqueue, yes:do nothing)
	QUEUENODE firstNode = deQueue(queue);//pop from the ready queue
	link[linkControler].processid = firstNode.processid;
	link[linkControler].arrivalTime = firstNode.arrivalTime;
	link[linkControler].burstTime = firstNode.burstTime;
	bool flag = 1;
	for (int i = 0; i < linkControler; i++)
	{
		if (link[i].processid == link[linkControler].processid)
		{
			flag = 0;
		}
	}
	if (linkControler == 0)//if the first link node
	{
		link[linkControler].startTime = 0;
		if (link[linkControler].burstTime <= quantumTime)
		{
			link[linkControler].endTime = 0 + quantumTime;
			timeConsume = link[linkControler].endTime;
			link[linkControler].leftBurstTime = 0;
		}
		else
		{
			link[linkControler].endTime = link[linkControler].startTime + quantumTime;
			timeConsume = link[linkControler].endTime;
			link[linkControler].leftBurstTime = link[linkControler].burstTime - quantumTime;
		}
	}
	else if (flag == 1)//fisrt process node
	{
		link[linkControler].startTime = link[linkControler - 1].endTime;;
		if (link[linkControler].burstTime <= quantumTime)
		{
			link[linkControler].endTime = link[linkControler].startTime + link[linkControler].burstTime;
			timeConsume = link[linkControler].endTime;
			link[linkControler].leftBurstTime = 0;
		}
		else
		{
			link[linkControler].endTime = link[linkControler].startTime + quantumTime;
			timeConsume = link[linkControler].endTime;
			link[linkControler].leftBurstTime = link[linkControler].burstTime - quantumTime;
		}
	}
	else//if occur before
	{
		int position = 0;
		for (int i = 0; i < linkControler; i++)//last occurence
		{
			if (link[i].processid == link[linkControler].processid)
			{
				position = i;
			}
		}
		if (link[position].leftBurstTime > quantumTime)
		{
			link[linkControler].startTime = link[linkControler - 1].endTime;
			link[linkControler].endTime = link[linkControler].startTime + quantumTime;
			timeConsume = link[linkControler].endTime;
			link[linkControler].leftBurstTime = link[position].leftBurstTime - (link[linkControler].endTime - link[linkControler].startTime);
		}
		else//link[linkControler].leftBurstTIme<=quantumTime
		{
			link[linkControler].startTime = link[linkControler - 1].endTime;
			link[linkControler].endTime = link[linkControler].startTime + link[position].leftBurstTime;
			timeConsume = link[linkControler].endTime;
			link[linkControler].leftBurstTime = 0;
		}
	}
	int notCurrentProcessTimeConsume = 0;
	for (int i = 0; i < linkControler; i++)
	{
		if (link[i].processid != link[linkControler].processid)//not current process time
		{
			notCurrentProcessTimeConsume = link[i].endTime - link[i].startTime + notCurrentProcessTimeConsume;
		}
	}
	link[linkControler].waitingTime = notCurrentProcessTimeConsume - link[linkControler].arrivalTime;

	for (int i = ArrivalTimeControler + 1; arrivalTime[i] != '\0'; i++)
	{

		if (arrivalTime[i] <= timeConsume)
		{
			enQueue(process[i], arrivalTime[i], burstTime[i], quantumTime, queue);
			ArrivalTimeControler++;
		}
	}
	if (link[linkControler].leftBurstTime > 0)//check if finish
	{
		enQueue(firstNode.processid, firstNode.arrivalTime, firstNode.burstTime, quantumTime, queue);
	}
	linkControler++;
	link[linkControler].processid = -1;
}


int getWaitingTime(LINKNODE link[],int processid)
{
	int position = 0;
	for (int i = 0; i < linkControler; i++)//find the last occurence
	{
		if (link[i].processid == processid)
		{
			position = i;
		}
	}
	return link[position].waitingTime;
}

int getTurnAroundTime(LINKNODE link[], int processid,int arrivalTime[])
{
	int position = 0;
	for (int i = 0; i<linkControler; i++)//find the last occurence
	{
		if (link[i].processid == processid)
		{
			position = i;
		}
	}
	return link[position].endTime-arrivalTime[processid-1];
}

void print(int process[],int arrivalTime[],int burstTime[], LINKNODE link[])
{
	int totalTurnAroundTime = 0;
	float totalWaitingTime = 0;
	int waitingTime = 0;
	int turnaroundTime = 0;
	for (int i = 0; process[i] != '\0'; i++)
	{
		waitingTime = getWaitingTime(link, i + 1);
		totalWaitingTime = totalWaitingTime + waitingTime;
		turnaroundTime = getTurnAroundTime(link, i + 1, arrivalTime);
		totalTurnAroundTime = totalTurnAroundTime + turnaroundTime;
		printf("Process: P%d Arrival Time: %d Burst time: %d ",process[i],arrivalTime[i],burstTime[i]);
		printf("waitting time: %d Turnaround Time: %d\n", waitingTime, turnaroundTime);
	}
	float counter = 0;
	for (int i = 0; process[i] != '\0'; i++)
	{
		counter++;
	}
	printf("\nTotal Turnaround Time: %d\nAverage waiting time = %.2fs\nAverage turnaround time = %.2f\n\n", totalTurnAroundTime, (float)totalWaitingTime/counter, (float)totalTurnAroundTime/counter);
}

int main()
{
	FILE *fp;
	char buff[255];
	int process[50];//will store process
	int arrivalTime[50];//will store coresponding process ariival time
	int burstTime[50];//will store coresponding process burst time
	/*P1 0 24 P2 1 3 P3 2 3 4
	  P1 0 10 P2 1 5 P3 2 8 P4 3 9 2
          
	  process[] = {1,2,3}
	  arrivalTime[] = {0,1,2}
	  burstTime[] = {24,3,3}

	  then after loop again

	  process[] = {1,2,3,4}
	  arrivalTime[] = {0,1,2,3}
	  burstTime[] = {10,5,8,9}
	 */
	fp = fopen("rr_input.txt", "r");
	int fileLineCounter=0;
	while (fgets(buff, 255, (FILE*)fp)!=NULL)
	{
		fileLineCounter++;
	}
	fclose(fp);
	
	fp = fopen("rr_input.txt", "r");
	for(int w = 0; w < fileLineCounter; w++)
	{
		fgets(buff,255,(FILE*)fp);//get line by line
		int quantumTime = 0;
		int counter = 0;
		for (int i = 0; buff[i] != '\n' && buff[i]!='\0'; i++)
		{
			counter++;
		}
		quantumTime = buff[counter - 2] - '0';

		int arrivalTimeControler = 0;
		int processControler = 0;
		int burstTimeControler = 0;
		for (int i = 0; i < counter - 2; i++)//this big for loop is assign value to process[], arrivalTime[], burstTime[]
		{
			for (int q = i + 1; buff[q] != ' '; q++)
			{
				process[processControler] = buff[q] - '0';
				processControler++;
				i = q + 1;
			}
			for (int j = i + 1; buff[j] != ' '; j++)
			{
				int digit_counter = 0;
				for (int jj = j; buff[jj] != ' '; jj++)
				{
					digit_counter++;
				}
				if (digit_counter == 1)
				{
					arrivalTime[arrivalTimeControler] = buff[j] - '0';
					arrivalTimeControler++;
					i = j + 1;
				}
				if (digit_counter == 2)
				{
					int temp1 = 0;
					int temp2 = 0;
					temp1 = (buff[j] - '0') * 10;
					temp2 = buff[j + 1] - '0';
					arrivalTime[arrivalTimeControler] = temp1 + temp2;
					arrivalTimeControler++;
					i = j + 2;
					j++;
				}
			}
			for (int u = i + 1; buff[u] != ' '; u++)
			{
				int digit_counter = 0;
				for (int uu = u; buff[uu] != ' '; uu++)
				{
					digit_counter++;
				}
				if (digit_counter == 1)
				{
					burstTime[burstTimeControler] = buff[u] - '0';
					burstTimeControler++;
					i = u + 1;
				}
				if (digit_counter == 2)
				{
					int temp1 = (buff[u] - '0') * 10;
					int temp2 = buff[u + 1] - '0';
					burstTime[burstTimeControler] = temp1 + temp2;
					burstTimeControler++;
					i = u + 2;
					u++;
				}
			}
		}
		process[processControler] = '\0';
		arrivalTime[arrivalTimeControler] = '\0';
		burstTime[burstTimeControler] = '\0';

		int totalBurstTime = 0;
		for (int i = 0; burstTime[i] != '\0'; i++)
		{
			totalBurstTime = burstTime[i] + totalBurstTime;
		}
		QUEUENODE queue[50];
		LINKNODE link[50];
		enQueue(process[0], arrivalTime[0], burstTime[0], quantumTime, queue);//enqueu the first process first
		while (timeConsume != totalBurstTime)//if time consume on CPU = sum of all processes burst time, then it means all process are done
		{
			executeRR(queue, process, arrivalTime, burstTime, link, quantumTime);
		}
		print(process, arrivalTime, burstTime, link);
		timeConsume = 0;//set time consume variable to 0 so that next case can use it
		queueControler = 0;//set to 0 so that next case can use it
		linkControler = 0;//set to 0 so that next case can use it
		ArrivalTimeControler = 0;//set to 0 so that next case can use it
	}
	fclose(fp);
	return 0;
}
