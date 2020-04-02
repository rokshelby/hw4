#include "myGlobal.h"


int GRN(char*);
int main(int argc, char ** argv)
{

	int defaultNumProcesses = 18;
	int minusCounter = defaultNumProcesses;
	int maxTimeBetweenNewProcsNS = 0;
	int maxTimeBetweenNewProcesSecs = 0;
	int timeDelays = 0;
	int timeDelayns = 0;
	int curTimes = 0;
	int curTimens = 0;	
	int enterFlag = 1;	
	int pid = 0;
	int pida = 0;
	int aliveChilds = 0;
	int maxCreated = 0;
	int * pcb; 
	int status = 0;
	char ** argToPass = (char**)malloc(sizeof(char*)*2);
	int i = 0;
		
	key_t key = ftok(sharedKey, sharedInt);

	unsigned int shmid = shmget(key, ((18*5)+2)* sizeof(unsigned int), 0666|IPC_CREAT);
	

	
	pcb = (int*)shmat(shmid, NULL, 0);
	pcb[0] = 0;
	shmdt(pcb);
	

	for(i = 0; i < 2; i++)
		argToPass[i] = (char*)malloc(sizeof(int)*8);
	i = 0;
	
	int newProcessFlag = 0;
	do
	{
		pcb = (int*) shmat(shmid, NULL, 0);
		curTimes = pcb[0];
		curTimens = pcb[1];
		shmdt(pcb);	
		if(timeDelayns >= curTimens)
		{

			if(timeDelayns >= curTimens)
			{
				enterFlag = 1;
			}

		}	
		
		
		
		if(maxCreated < defaultNumProcesses && enterFlag == 1)
		{
			printf("timeDelay %d, timeDelayns %d, enterFlag %d curTimes %d, curTimens %d \n", timeDelays, timeDelayns, enterFlag, curTimes, curTimens);
			pcb = (int*)shmat(shmid, NULL, 0);
			
			pid = fork();
	
			if(pid < 0)
			{	
				perror("Creation of child process was unsuccessful\n");
			}
			else if(pid == 0)
			{
				char * st;
				printf("Created child %d\n", getpid());
				st = (char*)malloc(sizeof(char) * 8);
				sprintf(st, "%d", shmid);
				strcpy(argToPass[0],st);
				sprintf(st, "%d", (i*5)+2);
				strcpy(argToPass[1],st);
				execv("child", argToPass);
				exit(0);
			}
			else if(pid > 0)
			{
				printf("OSS: Generating process with PID %d and putting it in queue %d at time %d:%d\n", pid, aliveChilds, pcb[0],pcb[1]);
				pcb[(i*5)+2] = pid;
				pcb[((i*5)+2)+1] = 15;
				shmdt(pcb);
				aliveChilds++;
				maxCreated++;
				newProcessFlag = 1;
				enterFlag = 0;
			}
		
		}
		
		//everyone gets a turn
		pcb = (int*)shmat(shmid, NULL, 0);
		//pcb[((defaultNumProcesses - minusCounter)*5)+1] = pcb[((defaultNumProcesses - minusCounter)*5)+1] - 1;
		shmdt(pcb);
		
		for(i = 0; i < defaultNumProcesses; i++)
		{
			pcb = (int*)shmat(shmid, NULL, 0);
			pida = waitpid(pida, &status, WNOHANG);
			shmdt(pcb);
			if(pida == -1)
			{
			
				
			}
			else if(pida == 0)
			{
				pcb = (int*)shmat(shmid, NULL, 0);
				//printf("child still running %d\n", pcb[i*5]);
				shmdt(pcb);
			}
			else if(pida > 0)
			{
				printf("child is finished %d\n", pida);
				aliveChilds--;
			}
		}

		pcb = (int*) shmat(shmid, NULL, 0);
		pcb[1] = pcb[1] + 1;
		
		if(pcb[1] > 100000000)
		{
			pcb[0] = pcb[0] + 1;			
		}

		if(newProcessFlag == 1)
		{
			
			timeDelayns = GRN("ns");
			timeDelays = GRN("");
			if(timeDelayns + pcb[1]> 100000000)
			{
				timeDelayns = timeDelayns - 100000000-pcb[1];
				timeDelays = timeDelays+1 + pcb[0];
				
			}
			else
			{
				timeDelayns = timeDelayns + pcb[1];	
				timeDelays = timeDelays + pcb[0] + 3;
			}
			newProcessFlag = 0;
			printf("timeDelayns %d, timeDelays %d\n", timeDelayns, timeDelays);
		}
		shmdt(pcb);
			

		//create delay.
		



	}while(aliveChilds > 0 || maxCreated < defaultNumProcesses);

	
	if(shmctl(shmid, IPC_RMID, NULL) < 0)
		fprintf(stderr, "Shared memory was not deallocated: remove it manually\n");

	

	return 0;
} 



int GRN(char * str)
{
	struct timeval t1;
	gettimeofday(&t1, NULL);
	srand(t1.tv_usec * t1.tv_sec);
	int procTime = 0;

	if(strcmp("ns",str))
	{
		procTime = (rand() % 3);
	}
	else
		procTime = 100 + (rand() % 9901);

	return procTime;	
}
