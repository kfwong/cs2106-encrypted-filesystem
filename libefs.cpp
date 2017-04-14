#include "libefs.h"


// FS Descriptor
TFileSystemStruct *_fs;

// Open File Table
TOpenFile *_oft;

// Open file table counter
int _oftCount=0;

const char *_filename;

unsigned long _len;

// Mounts a partition given in fsPartitionName. Must be called before all
// other functions
void initFS(const char *fsPartitionName, const char *fsPassword)
{
	mountFS(fsPartitionName, fsPassword);
	_fs = getFSInfo();
	_oft = (TOpenFile *) malloc (1000 * sizeof(TOpenFile));
}



// Opens a file in the partition. Depending on mode, a new file may be created
// if it doesn't exist, or we may get FS_FILE_NOT_FOUND in _result. See the enum above for valid modes.
// Return -1 if file open fails for some reason. E.g. file not found when mode is MODE_NORMAL, or
// disk is full when mode is MODE_CREATE, etc.

int openFile(const char *filename, unsigned char mode)
{	
	_filename = filename;

	unsigned int fileNdx = findFile(filename); 
	
	unsigned int len = getFileLength(filename);

	if(fileNdx == FS_FILE_NOT_FOUND)
	{
		if(mode == MODE_NORMAL || mode == MODE_READ_ONLY)
		{
			printf("FILE NOT FOUND\n");
			exit(-1);
		}else if(mode == MODE_CREATE || mode == MODE_READ_APPEND)
		{


			if(mode == MODE_CREATE){


				char *buffer;

				// Allocate the buffer for reading
				buffer = makeDataBuffer();

				writeFile(fileNdx, buffer, sizeof(char), 0);
				
			}
		}
	}else{
		if(mode == MODE_CREATE){
			printf("Duplicate file error");
			exit(-1);
		}
	}

	int i = 0;
		
	// loop through all entries in open file table
	while (i < 1000)
	{
		if (_oft[i].available != 1)
		{
			// set oft values
			_oft[i].openMode = mode;
			_oft[i].blockSize = _fs->blockSize;
			_oft[i].inode = getInodeForFile(filename);
			_oft[i].inodeBuffer = makeInodeBuffer();
			_oft[i].buffer = makeDataBuffer();
			_oft[i].readPtr = 0;
			_oft[i].writePtr = 0;
			_oft[i].fileName = filename;
			_oft[i].available = 1;
				
			_oftCount++;	
			return i;
		}

		i++;
	}
	
}

// Write data to the file. File must be opened in MODE_NORMAL or MODE_CREATE modes. Does nothing
// if file is opened in MODE_READ_ONLY mode.
void writeFile(int fp, void *buffer, unsigned int dataSize, unsigned int dataCount)
{
	//TOpenFile file = _oft[fp];
	/*
	if (file.openMode == MODE_READ_ONLY)
	{
		return;
	}
	*/

	printf("saving");

	FILE *fptr = fopen(_filename, "r");

	// Get the FS metadata
	TFileSystemStruct *fs = getFSInfo();

	char *_buffer;

	// Allocate the buffer for reading
	_buffer = makeDataBuffer();

	unsigned long len2 = fread(_buffer, sizeof(char), fs->blockSize, fptr);
	
	// Write the directory entry
	unsigned int dirNdx = makeDirectoryEntry(_filename, 0x0, len2);

	// Find a free block
	unsigned long freeBlock = findFreeBlock();

	// Mark the free block now as busy
	markBlockBusy(freeBlock);

	// Create the inode buffer
	unsigned long *inode = makeInodeBuffer();

	// Load the
	loadInode(inode, dirNdx);

	// Set the first entry of the inode to the free block
	inode[0]=freeBlock;

	// Write the data to the block
	writeBlock(_buffer, freeBlock);

	// Write the inode
	saveInode(inode, dirNdx);

	// Write the free list
	updateFreeList();

	// Write the diretory
	updateDirectory();

	// Free data and inode buffer
	free(_buffer);
	free(inode);
	
	
}

// Flush the file data to the disk. Writes all data buffers, updates directory,
// free list and inode for this file.
void flushFile(int fp)
{
	updateDirectory();
	updateFreeList();
}

// Read data from the file.
void readFile(int fp, void *buffer, unsigned int dataSize, unsigned int dataCount)
{
	TOpenFile file = _oft[fp];
	
	FILE *outfile = fopen(file.fileName, "w");

	unsigned int len = getFileLength(_oft[fp].fileName);

	loadInode(file.inodeBuffer, file.inode);

	unsigned long blockNum = file.inodeBuffer[0];

	readBlock(file.buffer, blockNum);

	fwrite(file.buffer, sizeof(char), len, outfile);

	fclose(outfile);

	free(file.inodeBuffer);
	free(file.buffer);
	
}

// Delete the file. Read-only flag (bit 2 of the attr field) in directory listing must not be set. 
// See TDirectory structure.
void delFile(const char *filename)
{
	unsigned int attr = getattr(filename);
	
	if (attr == 2)
	{
		printf("File is read only");
		return;
	}
	
	unsigned int fileNdx = findFile(filename);
	
	if(fileNdx == FS_FILE_NOT_FOUND)
	{
		printf("File not found");
		return;
	}
	
	// Create the inode buffer
	unsigned long *inode = makeInodeBuffer();

	// Load the inode
	loadInode(inode, fileNdx);
	
	unsigned long blockNum = inode[0];
	markBlockFree(blockNum);
	updateFreeList();
	
	delDirectoryEntry(filename);
	updateDirectory();
	
}	

// Close a file. Flushes all data buffers, updates inode, directory, etc.
void closeFile(int fp)
{
	TOpenFile file = _oft[fp];
	flushFile(fp);
	file.available = 0;

	updateFreeList();
	updateDirectory();
}

// Unmount file system.
void closeFS(){
	unmountFS();
};

