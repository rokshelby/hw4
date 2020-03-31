#include "myGlobal.h"



int main(int argc, char ** argv)
{
	int id = 0;
	int index = 0;
	int * pcb;
	sscanf(argv[0], "%d", &id);
	sscanf(argv[1], "%d", &index);
	
	pcb = (int*) shmat(id, NULL, 0);

	int exitFlag = 0;
	
	while(exitFlag == 0)
	{
		pcb = (int*) shmat(id, NULL, 0);
		if(pcb[index+1] == 0)
		{
			exitFlag = 1;
		}
		shmdt(pcb);
	}

	return 0;
}
