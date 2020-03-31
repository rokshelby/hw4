#include "myGlobal.h"


int GenerateRandomNumber();

int main(int argc, char ** argv)
{

	int defaultNumProcesses = 18;
	int minusCounter = defaultNumProcesses;
	int maxTimeBetweenNewProcsNS = 0;
	int maxTimeBetweenNewProcesSecs = 0;
	int pid = 0;
	int pida = 0;
	int * pcb; 
	int status = 0;
	char ** argToPass = (char**)malloc(sizeof(char*)*2);
	int i = 0;
		
	key_t key = ftok(sharedKey, sharedInt);

	int shmid = shmget(key, 18*2* sizeof(int), 0666|IPC_CREAT);
	

	
	pcb = (int*)shmat(shmid, NULL, 0);
	pcb[0] = 0;
	shmdt(pcb);
	

	for(i = 0; i < 2; i++)
		argToPass[i] = (char*)malloc(sizeof(int)*8);
	i = 0;
	
	
	for(i = 0; i < defaultNumProcesses; i++)
	{
		pcb = (int*)shmat(shmid, NULL, 0);

		pid = fork();
	
		if(pid < 0)
		{	
			perror("Creation of child process was unsuccessful\n");
		}
		else if(pid == 0)
		{
			char * st;
			st = (char*)malloc(sizeof(char) * 8);
			sprintf(st, "%d", shmid);
			strcpy(argToPass[0],st);
			sprintf(st, "%d", i*2);
			strcpy(argToPass[1],st);
			execv("child", argToPass);
			exit(0);
		}
		else if(pid > 0)
		{
			printf("Sent off child to do stuff\n");
			pcb[i*2] = pid;
			pcb[(i*2)+1] = 5;
			shmdt(pcb);

		}
	}
	int exitFlag = 0;
	do
	{
		//everyone gets a turn
		pcb = (int*)shmat(shmid, NULL, 0);
		pcb[((defaultNumProcesses - minusCounter)*2)] = pcb[((defaultNumProcesses - minusCounter)*2)] - 1;
		shmdt(pcb);
		
		for(i = 0; i < defaultNumProcesses; i++)
		{
			pida = waitpid(pida, &status, WNOHANG);
			if(pida == -1)
			{

			}
			else if(pida == 0)
			{
				printf("child still running %d\n", pida);
			}
			else if(pida > 0)
			{
				printf("child is finished %d\n", pida);
			}
	
		}

		exitFlag = 1;
		for(i = 0; i < defaultNumProcesses && exitFlag == 1; i++)
		{
			if(pcb[((defaultNumProcesses - minusCounter)*2)+1] > 0)
				exitFlag = 0;
		}
		minusCounter = minusCounter - 1;

	}while(exitFlag == 0);

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
		procTime = (rand() % 1001);

	return procTime;	
}
