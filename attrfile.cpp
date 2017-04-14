#include "libefs.h"

int main(int ac, char **av)
{
	if(ac != 3)
	{
		printf("\nUsage: %s <file to check set attrbute> <attribute>\n", av[0]);
		printf("Attribute: 'R' = Read only, 'W' = Read/Write\n\n");
		return -1;
	}
	
	initFS("part.dsk", av[2]);

	unsigned int attr = getattr(av[1]);
	
	if (attr == FS_FILE_NOT_FOUND)
	{
		printf("File not found\n");
		return -1;
	}
	
	if (av[2] == "r" || av[2] == "R")
	{
		setattr(av[1], 2);
	}
	else if (av[2] == "w" || av[2] == "W")
	{
		setattr(av[1], 1);
	}
	else
	{
		printf("Invalid input\n");
		printf("Attribute: 'R' = Read only, 'W' = Read/Write\n\n");
		return -1;
	}
	
	
	return 0;
}
