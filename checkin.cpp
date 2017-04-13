#include "libefs.h"

int main(int ac, char **av)
{
	if(ac != 3)
	{
		printf("\nUsage: %s <file to check in> <password>\n\n", av[0]);
		return -1;
	}
	
	initFS("part.dsk", av[2]);
	
	int fp = openFile(av[1], 1);
	
	if (fp >= 0)
	{
		printf("Error: Duplicate file\n");
		return -1;
	}
	
	//printf("%c\n", _oft->openMode);
	
	//writeFile(1, , 0, 0);
	
	closeFile(1);
	
	closeFS();

	return 0;
}
