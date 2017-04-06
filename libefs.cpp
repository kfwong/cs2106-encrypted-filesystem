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
	TFileSystemStruct *fs = getFSInfo();
}

// Opens a file in the partition. Depending on mode, a new file may be created
// if it doesn't exist, or we may get FS_FILE_NOT_FOUND in _result. See the enum above for valid modes.
// Return -1 if file open fails for some reason. E.g. file not found when mode is MODE_NORMAL, or
// disk is full when mode is MODE_CREATE, etc.

int openFile(const char *filename, unsigned char mode)
{		
    _oft->openMode = mode;
	_oft->blockSize = _fs->blockSize;
	
	unsigned int dirEntry = findFile(filename);
	switch (mode)
	{
		case MODE_NORMAL:		
			if (dirEntry == FS_FILE_NOT_FOUND)
			{
				return -1;
			}
			else 
			{
				// somehow update open file table
				unsigned long *inodeBuff = makeInodeBuffer();
				char *buffer = makeDataBuffer();
				unsigned int len = getFileLength(filename);
				
				_oft->inode = *inodeBuff;
				_oft->writePtr = *buffer;
				_oft->readPtr = *buffer;
				
				loadInode(inodeBuff, dirEntry);
				unsigned long blockNum = inodeBuff[0];
				readBlock(buffer, blockNum);
			}
			break;
		case MODE_CREATE:
			if (dirEntry == FS_FILE_NOT_FOUND)
			{
				
			}
			else
			{
				
			}
			break;
		case MODE_READ_ONLY:
			break;
		case MODE_READ_APPEND:
			break;
		default:
			return -1;
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
}

// Delete the file. Read-only flag (bit 2 of the attr field) in directory listing must not be set. 
// See TDirectory structure.
void delFile(const char *filename);

// Close a file. Flushes all data buffers, updates inode, directory, etc.
void closeFile(int fp);

// Unmount file system.
void closeFS();

