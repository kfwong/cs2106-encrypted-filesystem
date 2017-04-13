#include "libefs.h"

int main(int ac, char **av)
{
	if(ac != 3)
	{
		printf("\nUsage: %s <file to check in> <password>\n\n", av[0]);
		return -1;
	}
	
	initFS("part.dsk", av[2]);
	
	char *buffer;
	
	int fp = openFile(av[1], MODE_NORMAL);
	
	//writeFile(fp, buffer, len, 0);
	
	return 0;
}
