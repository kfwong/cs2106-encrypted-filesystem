#include "libefs.h"

int main(int ac, char **av)
{
	if(ac != 3)
	{
		printf("\nUsage: %s <file to check out> <password>\n\n", av[0]);
		return -1;
	}

	initFS("part.dsk", av[2]);

	int fp = openFile(av[1], MODE_READ_ONLY);

	readFile(fp, makeDataBuffer(), 0, 0);
	
	closeFS();
	
	return 0;
}
