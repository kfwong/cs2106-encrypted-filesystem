#include "libefs.h"

// FS Descriptor
TFileSystemStruct *_fs;

// Open File Table
TOpenFile *_oft;

// Open file table counter
int _oftCount=0;

// Mounts a partition given in fsPartitionName. Must be called before all
// other functions
void initFS(const char *fsPartitionName, const char *fsPassword)
{
	mountFS(fsPartitionName, fsPassword);
	_fs = getFSInfo();
	_oft = new TOpenFile[1000]();
}

// Opens a file in the partition. Depending on mode, a new file may be created
// if it doesn't exist, or we may get FS_FILE_NOT_FOUND in _result. See the enum above for valid modes.
// Return -1 if file open fails for some reason. E.g. file not found when mode is MODE_NORMAL, or
// disk is full when mode is MODE_CREATE, etc.

int openFile(const char *filename, unsigned char mode)
{	
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
			// create new file
		}
	}
	int i = 0;
		
	// loop through all entries in open file table
	while (i < 1000)
	{
		if (_oft[i].available != 0)
		{
			// set oft values
			_oft[i].openMode = mode;
			_oft[i].blockSize = _fs->blockSize;
			_oft[i].inode = getInodeForFile(filename);
			_oft[i].inodeBuffer = makeInodeBuffer();
			_oft[i].buffer = makeDataBuffer();
			_oft[i].readPtr = *_oft->buffer;
			_oft[i].writePtr = *_oft->buffer;
			_oft[i].fileName = filename;
			_oft[i].available = 0;
				
			return i;
		}
		i++;
		
	}
	
}

// Write data to the file. File must be opened in MODE_NORMAL or MODE_CREATE modes. Does nothing
// if file is opened in MODE_READ_ONLY mode.
void writeFile(int fp, void *buffer, unsigned int dataSize, unsigned int dataCount)
{
}

// Flush the file data to the disk. Writes all data buffers, updates directory,
// free list and inode for this file.
void flushFile(int fp)
{
}

// Read data from the file.
void readFile(int fp, void *buffer, unsigned int dataSize, unsigned int dataCount)
{
	TOpenFile file = _oft[fp];
	
	FILE *outfile = fopen(filename, "w");

	loadInode(_oft->inodeBuffer, _oft->inode);

	unsigned long blockNum = _oft->inodeBuffer[0];

	readBlock(buffer, blockNum);

	fwrite(_oft->buffer, dataSize, dataCount, outfile);

	fclose(fp);

	free(_oft->inodeBuffer);
	free(_oft->buffer);
}

// Delete the file. Read-only flag (bit 2 of the attr field) in directory listing must not be set. 
// See TDirectory structure.
void delFile(const char *filename);

// Close a file. Flushes all data buffers, updates inode, directory, etc.
void closeFile(int fp);

// Unmount file system.
void closeFS(){
	unmountFS();
};

