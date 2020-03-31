#include "myGlobal.h"


int GenerateRandomNumber();

int main(int argc, char ** argv)
{

	int defaultNumProcesses = 3;
	int minusCounter = defaultNumProcesses;
	int maxTimeBetweenNewProcsNS = 0;
	int maxTimeBetweenNewProcesSecs = 0;
	int pid = 0;
	int pida = 0;
	int aliveChilds = 0;
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
			printf("Created child %d\n", getpid());
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
			pcb[(i*2)+1] = 15;
			shmdt(pcb);
			aliveChilds++;

		}
	}
	int exitFlag = 0;
	do
	{
		//everyone gets a turn
		pcb = (int*)shmat(shmid, NULL, 0);
		pcb[((defaultNumProcesses - minusCounter)*2)+1] = pcb[((defaultNumProcesses - minusCounter)*2)+1] - 1;
		shmdt(pcb);
		
		for(i = 0; i < defaultNumProcesses; i++)
		{
			pcb = (int*)shmat(shmid, NULL, 0);
			pida = waitpid(pcb[i*2], &status, WNOHANG);
			shmdt(pcb);
			if(pida == -1)
			{

			}
			else if(pida == 0)
			{
				pcb = (int*)shmat(shmid, NULL, 0);
				printf("child still running %d\n", pcb[i*2]);
				shmdt(pcb);
			}
			else if(pida > 0)
			{
				printf("child is finished %d\n", pida);
				aliveChilds--;
					
			}
		}
	
		

		exitFlag = 1;
		for(i = 0; i < defaultNumProcesses && exitFlag == 1; i++)
		{
			pcb = (int*)shmat(shmid, NULL, 0);
			if(pcb[((defaultNumProcesses - minusCounter)*2)+1] > 0)
				exitFlag = 0;
			shmdt(pcb);
		}


		
		minusCounter = minusCounter - 1;
		if(minusCounter == 0)
		{
			minusCounter = defaultNumProcesses;
		}

	}while(aliveChilds > 0);

	
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
		procTime = (rand() % 1001);

	return procTime;	
}
