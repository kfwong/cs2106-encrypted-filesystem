#include "libefs.h"
#include <iostream>

using namespace std;

int main(int ac, char **av)
{
	if(ac != 3)
	{
		printf("\nUsage: %s <file to check out> <password>\n\n", av[0]);
		return -1;
	}
	
	initFS("part.dsk", av[2]);

	openFile(av[1], MODE_READ_ONLY);

	return 0;
}
