#include "myGlobal.h"



int main(int argc, char ** argv)
{
	int id = 0;
	int index = 0;
	int exitFlag = 0;
	int * pcb;
	sscanf(argv[0], "%d", &id);
	sscanf(argv[1], "%d", &index);
	
	pcb = (int*) shmat(id, NULL, 0);


	printf("time = %d\n",pcb[index+1]);
	shmdt(pcb);	
	while(exitFlag == 0)
	{
		pcb = (int*) shmat(id, NULL, 0);
		if(pcb[index+1] < 1)
		{
			exitFlag = 1;
		}
		
		printf("index %d pid %d  time%d\n", index, getpid(), pcb[index+1] );
		shmdt(pcb);
	}

	return 0;
}
