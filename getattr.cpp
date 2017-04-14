#include "libefs.h"

int main(int ac, char **av)
{
	if(ac != 2)
	{
		printf("\nUsage: %s <file to check>\n", av[0]);
		printf("Prints: 'R' = Read only, 'W' = Read/Write\n\n");
		return -1;
	}
	
	initFS("part.dsk", "pwd");
	
	unsigned int attr = getattr(av[1]);
	
	if (attr == 2)
	{
		printf("R\n");
	}
	else if (attr == FS_FILE_NOT_FOUND)
	{
		printf("File not found.\n");
	}
	else
	{
		printf("W\n");
	}
	

	return 0;
}
