#include "libefs.h"

int main(int ac, char **av)
{
	if(ac != 3)
	{
		printf("\nUsage: %s <file to check set attrbute> <attribute>\n", av[0]);
		printf("Attribute: 'R' = Read only, 'W' = Read/Write\n\n");
		return -1;
	}
	
	initFS("part.dsk", "pwd");

	unsigned int attr = getattr(av[1]);
	
	if (attr == FS_FILE_NOT_FOUND)
	{
		printf("File not found\n");
		return -1;
	}
	
	if (strcmp(av[2], "r") == 0 || strcmp(av[2], "R") == 0)
	{
		setattr(av[1], 2);
		updateDirectory();
	}
	else if (strcmp(av[2], "w") == 0 || strcmp(av[2],"W") == 0)
	{
		setattr(av[1], 1);
		updateDirectory();
	}
	else
	{
		printf("Invalid input\n");
		printf("Attribute: 'R' = Read only, 'W' = Read/Write\n\n");
		return -1;
	}
	
	
	return 0;
}
